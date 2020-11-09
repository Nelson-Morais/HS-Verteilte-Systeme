//
// Created by Nelson Morais on 30.10.20.
// Verteilte Systeme RPC Client Praktikum 04

#include <stdlib.h>
#include <unistd.h>
#include <rpc/rpc.h>
#include <string.h>
#include <wait.h>
#include <openssl/sha.h>

#include "RPC_Protocol_files/pub_sub.h"
#include "RPC_Protocol_files/return_codes.h"

//Define max Input
#define MAXINPUT 512


//tmp
struct argument *arg;
struct param *para;
hashstring hashstring_user_psw;

//PID für receiver, global für Tiddy funktionen parameterübergabe
pid_t childPID;

// vorwärtsdeklarationen
void err_abort(char *str);

void menuNavigation(char *input);

void subscribeToDispatcher(CLIENT *clnt);

void unsubscribeFromDispatcher(CLIENT *clnt);

void sendMessage(CLIENT *clnt);

void setTopic(CLIENT *clnt);

char *hash_sha(char *str);

char *input_user_pwd(user *user);

void login_to_Server(CLIENT *clnt);

void init_argument_struct();

char *hash_two_params(char *, char *);

char *hash_two_params_int(int para1, char *para2);

void hash_param(param *param);

void logout_of_the_server(CLIENT *clnt);

int main(int argc, char *argv[]) {
    CLIENT *clnt;
    sessionid *sid;
    char *host;
    //Input for the menu navigation.
    char *input = malloc(sizeof(char) * MAXINPUT);
    //user

    //Hash
    hashstring_user_psw = malloc(sizeof(char) * HASHLEN);

    //param struct
    para = malloc(sizeof(struct param));

    //argument struct
    arg = malloc(sizeof(struct argument));

    init_argument_struct();


    //Parameter prüfung und zuweisung
    if (argc < 2) {
        err_abort("Parameter ungültig, use: ./RPC_client <HOST>");
    }
    host = argv[1];

    //Client handle erzeugen mit Hostadresse, prognumber ( in pub_sub.h definiert ),
    //progversion ( in pub_sub.h definiert ), protocol (udp kann nur bis 8KBytes abschicken)
    clnt = clnt_create(host, PUBSUBPROG, PUBSUBVERS, "tcp");
    //client kann nicht verbinden
    if (clnt == NULL) {
        //druckt fehler meldung aus mit begrundung warum client nicht verbinden kann, parameter char*.
        clnt_pcreateerror(host);
        exit(1);
    }

    //Perform login and vaidation.
    login_to_Server(clnt);

    while (1) {
        menuNavigation(input);

        if (strcmp(input, "sub") == 0) {
            subscribeToDispatcher(clnt);
        } else if (strcmp(input, "unsub") == 0) {
            unsubscribeFromDispatcher(clnt);
        } else if (strcmp(input, "publish") == 0) {
            sendMessage(clnt);
        } else if (strcmp(input, "topic") == 0) {
            setTopic(clnt);
        } else if (strcmp(input, "logout") == 0) {
            logout_of_the_server(clnt);
            login_to_Server(clnt);
        } else if (strcmp(input, "exit") == 0) {
            break;
        }
        memset(input, '\0', sizeof(&input));
    }

    printf("closing the application.\n");
    unsubscribeFromDispatcher(clnt);
    logout_of_the_server(clnt);
    free(input);
    clnt_destroy(clnt);
    exit(1);
}


//Ausgabe von Fehlermeldungen
void err_abort(char *str) {
    fprintf(stderr, "RPC client: %s\n", str);
    fflush(stdout);
    fflush(stderr);
    exit(1);
}

void menuNavigation(char *input) {
    printf("\n---choose an option---\n\nsub - subscribe the client to a dispatcher.\nunsub - unsubscribe the client from a dispatcher\n"
           "publish - publish a message to all subscribed clients\ntopic - set a message topic on the dispatcher. ONLY POSSBILE LOCALLY\n"
           "logout - logs the client out of the server\nexit - closes the application.\n\n"
           "input option: ");


    //fgets kann mit '\n' enden, FIX! ( Quelle Stackoverflow: https://stackoverflow.com/questions/38767967/clear-input-buffer-after-fgets-in-c )
    if (fgets(input, MAXINPUT, stdin)) {
        char *p;
        if ((p = strchr(input, '\n')) != NULL) {//check exist newline
            *p = '\0';
        } else {
            scanf("%*[^\n]");
            scanf("%*c");//clear upto newline
        }
    }
}

void subscribeToDispatcher(CLIENT *clnt) {
    RET_CODE *return_code;

    para->arg.topic_or_message = 9;
    para->arg.argument_u.t = "";
    hash_param(para);

    return_code = subscribe_1(para, clnt);
    //pub_sub_clnt.c subscribe_1 liefert NULL zurück wenn RPC nicht "success" zeigt, oder short von clnt_call in clnt.h definizert, 0-Successful/andere wert nicht Successful.
    if (return_code == NULL) {
        printf("%s\n", PUB_SUB_RET_CODE[UNKNOWN_ERROR]);
    } else {
        printf("Subscribe status: %s\n", PUB_SUB_RET_CODE[*return_code]);
    }
    if (*return_code == OK) {
        //ZUSATZAUFGABE
        //receivers childprocess
        printf("Starting the message receiver...\n");
        childPID = fork();
        char *args[] = {0};

        if (childPID == 0) {
            execvp("./RPC_Receiver", args);

        }
        if (childPID == -1)
            printf("Error beim starten des Receivers. Fork() Failed.\n");

    }
}

void unsubscribeFromDispatcher(CLIENT *clnt) {
    RET_CODE *return_code;

    para->arg.topic_or_message = 9;
    para->arg.argument_u.t = "";
    hash_param(para);

    return_code = unsubscribe_1(para, clnt);
    //gleicher konzept wie subscribe_1
    if (return_code == NULL) {
        printf("%s\n", PUB_SUB_RET_CODE[UNKNOWN_ERROR]);
    } else {
        printf("Unsubscribe status: %s\n", PUB_SUB_RET_CODE[*return_code]);
    }
    //ZUSATZAUFGABE
    if (childPID != -1 && childPID != 0 && *return_code == OK) {

        printf("Closing the message receiver");

        //Versuchen kind prozess zu terminieren mit SIGTERM ( gracefully termination )
        kill(childPID, SIGTERM);

        int childDead = 0;
        int status;
        //Das kind Prozess bekommt 5 Sekunden um gracefully beendet zu werden
        for (int i = 0; !childDead && i < 5; i++) {

            printf(".");
            //stdout buffer wird geflusht da keine \n vorhanden ist um ein "loading" zu erzeugen während das kind prozess schliesst,
            // jede weitere sekunde wird ein "." auf stdout buffer gepackt und durch flush sofort ausgedruckt.
            fflush(stdout);
            sleep(1);
            //wenn der state des kindprozesses sich geändert hat, wird die flag gesetzt zum abbrechen des loops.
            //an diese stelle ist das kind prozess mit SIGTERM geschlossen worden.

            if (waitpid(childPID, &status, WNOHANG) == childPID) {
                childDead = 1;
                printf("\n");
            }

        }
        //nach 5 sekunden wenn SIGTERM nicht erfolgreich gewesen ist, wird das prozess mit SIGKILL
        //"gewaltig" beendet. Hierdurch wird abgesichert das kein "zoombie" prozesse im system bleiben.
        if (!childDead)
            kill(childPID, SIGKILL);
    }
}

void sendMessage(CLIENT *clnt) {
    RET_CODE *return_code;
    //message definiert in pub_sub.h
    message msg = malloc(MESLEN * sizeof(char));

    printf("Enter message: ");
    fflush(stdout);
    //fgets kann mit '\n' enden, FIX! ( Quelle Stackoverflow: https://stackoverflow.com/questions/38767967/clear-input-buffer-after-fgets-in-c )
    if (fgets(msg, MESLEN, stdin)) {
        char *p;
        if ((p = strchr(msg, '\n')) != NULL) {//check exist newline
            *p = '\0';
        } else {
            scanf("%*[^\n]");
            scanf("%*c");//clear upto newline
        }
    }

    para->arg.topic_or_message = 1;
    para->arg.argument_u.t = msg;
    hash_param(para);
    return_code = publish_1(para, clnt);

    printf("Message status: %s\n", PUB_SUB_RET_CODE[*return_code]);

    free(msg);
}

void setTopic(CLIENT *clnt) {
    RET_CODE *return_code;
    //topic in pub_sub definiert
    topic topic = malloc(TOPLEN * sizeof(char));
    memset(topic, '\0', sizeof(*topic));

    printf("Enter topic: ");
    fflush(stdout);
    //fgets kann mit '\n' enden, FIX! ( Quelle Stackoverflow: https://stackoverflow.com/questions/38767967/clear-input-buffer-after-fgets-in-c )
    if (fgets(topic, TOPLEN, stdin)) {
        char *p;
        if ((p = strchr(topic, '\n')) != NULL) {//check exist newline
            *p = '\0';
        } else {
            scanf("%*[^\n]");
            scanf("%*c");//clear upto newline
        }
    }

    para->arg.topic_or_message = 0;
    para->arg.argument_u.t = topic;
    hash_param(para);
    return_code = set_channel_1(para, clnt);
    if (return_code == NULL) {
        printf("%s\n", PUB_SUB_RET_CODE[UNKNOWN_ERROR]);
    } else {
        printf("Topic status: %s\n", PUB_SUB_RET_CODE[*return_code]);
    }
    free(topic);
}

void login_to_Server(CLIENT *clnt) {
    RET_CODE *return_code = NULL;
    //input user and password and generate the hashvalue
    char *user = malloc(sizeof(char) * 256);
    hashstring_user_psw = input_user_pwd(&user);

    para->id = *(get_session_1(&user, clnt));
    para->arg = *arg;

    char hash1[HASHLEN];
    strcpy(hash1, hash_two_params_int(para->id, ""));
    char hash2[HASHLEN];
    para->hash = strcpy(hash2, hash_two_params(hash1, hashstring_user_psw));

    return_code = validate_1(para, clnt);

    if (*return_code != OK) {
        printf("Login failed: %s\n", PUB_SUB_RET_CODE[*return_code]);
    }
    while (*return_code != OK) {
        hashstring_user_psw = input_user_pwd(&user);

        para->id = *(get_session_1(&user, clnt));
        para->arg = *arg;
        strcpy(hash1, hash_two_params_int(para->id, ""));
        para->hash = strcpy(hash2, hash_two_params(hash1, hashstring_user_psw));
        return_code = validate_1(para, clnt);
        if (*return_code != OK) {
            printf("Login failed: %s\n", PUB_SUB_RET_CODE[*return_code]);
        }
    }
    free(user);
}

/*
 * Hash aus user und pwd bilden -> dieser muss im Server-Digest gespeichert
 * werden.
 */

char *hash_two_params_int(int para1, char *para2) {
    char str[256];
    sprintf(str, "%d;%s", para1, para2);
    return hash_sha(str);
}

char *hash_two_params(char *para1, char *para2) {
    char str[256];
    sprintf(str, "%s;%s", para1, para2);
    return hash_sha(str);
}

/* Hashfunktionen mit verschiedenen Parametern */
char *hash_sha(char *str) {
    unsigned char result[SHA256_DIGEST_LENGTH];
    SHA256(str, strlen(str), result);
    char hash_val[96];
    hash_val[0] = '\0';
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        char tmp[3];
        sprintf(tmp, "%02x", result[i]);
        strcat(hash_val, tmp);
    }
    // printf ("hashval: %s\n", hash_val);
    return strdup(hash_val);
}

char *input_user_pwd(user *user) {
    char pwd[MAXINPUT];
    char hash_val[HASHLEN];

    printf("** LOGIN **\n");
    printf("user: ");

    //fgets kann mit '\n' enden, FIX! ( Quelle Stackoverflow: https://stackoverflow.com/questions/38767967/clear-input-buffer-after-fgets-in-c )
    if (fgets(*user, USERLEN, stdin)) {
        char *p;
        if ((p = strchr(*user, '\n')) != NULL) {//check exist newline
            *p = '\0';
        } else {
            scanf("%*[^\n]");
            scanf("%*c");//clear upto newline
        }
    }
    fflush(stdout);
    printf("password: ");

    if (fgets(pwd, MAXINPUT, stdin)) {
        char *p;
        if ((p = strchr(pwd, '\n')) != NULL) {//check exist newline
            *p = '\0';
        } else {
            scanf("%*[^\n]");
            scanf("%*c");//clear upto newline
        }
    }

    fflush(stdout);
    strcpy(hash_val, hash_two_params(*user, pwd));
    return strdup(hash_val);
}

void init_argument_struct() {
    arg->argument_u.m = "";
    arg->argument_u.t = "";
}

void hash_param(param *param) {
    char hash1[HASHLEN];

    switch (param->arg.topic_or_message) {
        case 0:
            strcpy(hash1, hash_two_params_int(param->id, param->arg.argument_u.t));
            break;
        case 1:
            strcpy(hash1, hash_two_params_int(param->id, param->arg.argument_u.m));
            break;
        default:
            strcpy(hash1, hash_two_params_int(param->id, ""));
            break;
    }

    param->hash = hash_two_params(hash1, hashstring_user_psw);
}

void logout_of_the_server(CLIENT *clnt) {
    invalidate_1(&para->id, clnt);
    printf("Logging out...\n\n\n");
}