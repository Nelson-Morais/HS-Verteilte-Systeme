//
// Created by Nelson Morais on 30.10.20.
// Verteilte Systeme RPC Client Praktikum 04

#include <stdlib.h>
#include <unistd.h>
#include <rpc/rpc.h>
#include <string.h>
#include <wait.h>
#include "RPC_Protocol_files/pub_sub.h"
#include "RPC_Protocol_files/return_codes.h"

//Define max Input
#define MAXINPUT 512

//return_codes als short int wie in .h definiert umd die Rückgabewerte der funktionen zu speichern und prüfen/ausgeben
//als globale variable, sichtbar für das ganze program: Tiddy funktionen parameterübergabe


//PID für receiver, global für Tiddy funktionen parameterübergabe
pid_t childPID;

// vorwärtsdeklarationen
void err_abort(char *str);

void menuNavigation(char *input);

void subscribeToDispatcher(CLIENT *clnt);

void unsubscribeFromDispatcher(CLIENT *clnt);

void sendMessage(CLIENT *clnt);

void setTopic(CLIENT *clnt);

int main(int argc, char *argv[]) {
    CLIENT *clnt;
    char *host;
    //Input for the menu navigation.
    char *input = malloc(sizeof(char) * MAXINPUT);


    //Parameter prüfung und zuweisung
    if (argc < 2) {
        err_abort("Parameter ungültig, use: ./RPC_client <HOST>");
    }
    host = argv[1];

    //Client handle erzeugen mit Hostadresse, prognumber ( in pub_sub.h definiert ),
    //progversion ( in pub_sub.h definiert ), protocol (udp kann nur bis 8KBytes abschicken) /* TCP BREAKS PROGRAM */.
    clnt = clnt_create(host, PUBSUBPROG, PUBSUBVERS, "udp");
    //client kann nicht verbinden
    if (clnt == NULL) {
        //druckt fehler meldung aus mit begrundung warum client nicht verbinden kann, parameter char*.
        clnt_pcreateerror(host);
        exit(1);
    }

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

        } else if (strcmp(input, "exit") == 0) {
            printf("closing the application.\n");
            unsubscribeFromDispatcher(clnt);
            free(input);
            clnt_destroy(clnt);
            exit(1);
        }
        memset(input, '\0', sizeof(&input));
    }
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
           "exit - closes the application.\n\n"
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
    return_code = subscribe_1(NULL, clnt);
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
    return_code = unsubscribe_1(NULL, clnt);
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
    //gleicher konzept wie subscribe_1, ohne OK
    return_code = publish_1(&msg, clnt);

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
    //gleicherkonzept wie subscribe_1
    return_code = set_channel_1(&topic, clnt);
    if (return_code == NULL) {
        printf("%s\n", PUB_SUB_RET_CODE[UNKNOWN_ERROR]);
    } else {
        printf("Topic status: %s\n", PUB_SUB_RET_CODE[*return_code]);
    }
    free(topic);
}