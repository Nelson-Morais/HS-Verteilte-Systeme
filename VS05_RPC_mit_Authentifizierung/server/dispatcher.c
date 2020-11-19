//
// Created by Nelson Morais on 30.10.20.
// Verteilte Systeme RPC Dispatcher Praktikum 04


#include <arpa/inet.h>
#include <time.h>
#include <openssl/sha.h>

#include "RPC_Protocol_files/pub_sub.h"
#include "RPC_Protocol_files/return_codes.h"
#include "RPC_Protocol_files/pub_sub_deliv.h"
#include "session_list.h"

#define LOCALADDRESS "127.0.0.1"
#define MAXSUBS 128
#define IPV4LENGTH 16

//vorwärts deklaration
void init_session_list();

char *hash_two_params_int(int para1, char *para2);

char *hash_two_params(char *para1, char *para2);

char *hash_sha(char *str);

short *check_session(param *param);

char *current_topic;
//array für subscriptions
char subs[MAXSUBS][IPV4LENGTH];

//struct für session verwaltung
session sessions = NULL;

static int initList = 0;
static int counter = 0;

short *set_channel_1_svc(param *param, struct svc_req *request) {

    //return_code static declariert ( auf static angelet ) damit der return nicht die referenz aufm stack ist.
    static RET_CODE return_code;
    return_code = *check_session(param);
    if (return_code == OK) {

        if(current_topic == NULL){
        current_topic = malloc(sizeof(char) * TOPLEN);//TODO: malloc für topic what to do, when to free up ?
        }
        char *req_addr = inet_ntoa(request->rq_xprt->xp_raddr.sin_addr);

        //check ob es local ist
        if ((strcmp(req_addr, LOCALADDRESS)) == 0) {
            strcpy(current_topic, param->arg.argument_u.t);
            printf(" --- Setting param: %s --- \n", param->arg.argument_u.t);
            return_code = OK;
        } else {
            return_code = CANNOT_SET_TOPIC;
        }
    }
    return &return_code;
}

short *subscribe_1_svc(param *param, struct svc_req *request) {
    //return_code static declariert ( auf static angelet ) damit der return nicht die referenz aufm stack ist.
    static RET_CODE return_code;
    return_code = *check_session(param);
    if (return_code == OK) {
        char *req_addr = inet_ntoa(request->rq_xprt->xp_raddr.sin_addr);

        for (int i = 0; i < counter; i++) {
            if ((strcmp(subs[i], req_addr)) == 0) {
                printf(" --- Client %s already Registered --- \n", req_addr);
                return_code = CLIENT_ALREADY_REGISTERED;
            }
        }
        if (return_code != CLIENT_ALREADY_REGISTERED && counter < MAXSUBS) {
            strcpy(subs[counter++], req_addr);
            printf(" --- Registered client with IP: %s to the subscriber list --- \n", req_addr);

            printf(" --- IPs in the Subscriber list --- \n\n");
            for (int x = 0; x < counter; x++) {
                printf("%s\n", subs[x]);
            }
            printf("\n ---------------------------------- \n");
            return_code = OK;
        } else if (counter == MAXSUBS) {
            printf(" --- Client with IP: %s can't be added to Subscriber list, MAX Subscribers already reached. --- \n",
                   req_addr);
            return_code = CANNOT_REGISTER;
        }
    }
    return &return_code;
}

extern short *unsubscribe_1_svc(param *param, struct svc_req *request) {
    //return_code static declariert ( auf static angelet ) damit der return nicht die referenz aufm stack ist.
    static RET_CODE return_code;
    return_code = *check_session(param);
    if (return_code == OK) {
        char *req_addr = inet_ntoa(request->rq_xprt->xp_raddr.sin_addr);
        char tmp[MAXSUBS][IPV4LENGTH];

        if (counter > 0) {
            for (int i = 0; i < counter; i++) {
                if ((strcmp(subs[i], req_addr)) == 0) {
                    printf(" --- Unregistring client with IP: %s from the subscriber list --- \n", req_addr);
                    //wenn es letzter element ist.
                    if (i == MAXSUBS - 1) {
                        for (int k = 0; k < MAXSUBS - 1; k++) {
                            strcpy(tmp[k], subs[k]);
                        }
                        //wenn nicht letzter element
                    } else {
                        for (int j = i; j < counter; j++) {
                            strcpy(subs[i], subs[i + 1]);
                        }
                        for (int l = 0; l < counter - 1; l++) {
                            strcpy(tmp[l], subs[l]);
                        }
                    }
                    memcpy(subs, tmp, sizeof(subs));
                    counter--;
                    return_code = OK;
                    break;
                }
                return_code = CANNOT_UNREGISTER;
            }
            if (counter > 0) {
                printf("\n --- IPs in the Subscriber list --- \n\n");
                for (int x = 0; x < counter; x++) {
                    printf("%s\n", subs[x]);
                }
                printf("\n ---------------------------------- \n");
            } else {
                printf(" --- No IPs in the Subscriber list --- \n");
            }
        } else {
            printf(" --- No IPs in the Subscriber list --- \n");
            return_code = CANNOT_UNREGISTER;
        }
    }
    return &return_code;
}

extern short *publish_1_svc(param *param, struct svc_req *request) {
    //return_code static declariert ( auf static angelet ) damit der return nicht die referenz aufm stack ist.
    static RET_CODE return_code;
    return_code = *check_session(param);
    if (return_code == OK) {
        CLIENT *clnt;

        char *postmessage = malloc(sizeof(char) * POSTMESLEN);
        char *req_addr = inet_ntoa(request->rq_xprt->xp_raddr.sin_addr);

        if (counter > 0) {
            for (int i = 0; i < counter; i++) {
                //PUBSUBCLTPROG & PUBSUBCLTVER für delivery in pub_sub_deliv declariert
                clnt = clnt_create(subs[i], PUBSUBCLTPROG, PUBSUBCLTVERS, "tcp");
                snprintf(postmessage, POSTMESLEN, "%s;%s;%s", req_addr, param->arg.argument_u.m, current_topic);
                if (clnt != NULL) {
                    deliver_1(&postmessage, clnt);
                    return_code = OK;
                } else {
                    clnt_pcreateerror(subs[i]);
                    return_code = UNKNOWN_ERROR;
                }
                printf("Sending message to ip: %s\n", subs[i]);
            }
            printf("Message sent to all Subscribers: %s\n", postmessage);
        } else {
            printf("Message not sent. There are no clients subscribed.\n");
            return_code = UNKNOWN_ERROR;
        }
        free(postmessage);
    }

    return &return_code;
}

extern sessionid *get_session_1_svc(user *usr, struct svc_req *request) {

    static sessionid sid = 0;
    //init struct session verwaltung
    if (!initList) init_session_list();
    //taktzeit als ID für die session
    sid = clock();

    insert_session(sessions, sid, *usr);

    return &sid;
}

extern short *validate_1_svc(param *param, struct svc_req *request) {
    //return_code static declariert ( auf static angelet ) damit der return nicht die referenz aufm stack ist.
    static RET_CODE return_code;

    return_code = *check_session(param);
    if (return_code == OK) {
        validate_session(sessions, param->id);
    } else {
        delete_session(sessions, param->id);
    }

    return &return_code;
}

extern short *invalidate_1_svc(const sessionid *sid, struct svc_req *request) {
    static RET_CODE return_code;

    if (delete_session(sessions, *sid)) {
        return_code = OK;
    } else {
        return_code = NO_SESSION_FOUND;
    }
    return &return_code;
}

void init_session_list() {
    sessions = malloc(sizeof(session));
    sessions->next = NULL;
    initList = 1;
}


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

short *check_session(param *param) {
    //return_code static declariert ( auf static angelet ) damit der return nicht die referenz aufm stack ist.
    static RET_CODE return_code;
    return_code = NO_ACCOUNT_FOUND;
    FILE *file;
    file = fopen("hashes.txt", "r");
    if (file == NULL) {
        perror("Could not open the file.");
        exit(1);
    }

    session this_session = search_session(sessions, param->id);
    if (this_session == NULL) {
        return_code = NO_SESSION_FOUND;
        return &return_code;
    }
    char hash1[HASHLEN];
    char hash2[HASHLEN];

    char *ptrl = malloc(sizeof(char) * 256);//heap memory
    char *line = ptrl; //stack pointer to heap memory damit wir der pointer auf heap nicht mit strtok() verlieren und später freigeben können.

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

    while (fgets(line, 256, file)) {
        char *p;
        if ((p = strchr(line, '\n')) != NULL) {//check exist newline and trim
            *p = '\0';
        }
        line = strtok(line, " ");
        if (strcmp(line, this_session->user) == 0) {
            line = strtok(NULL, " ");
            strcpy(hash2, hash_two_params(hash1, line));

            if (strcmp(hash2, param->hash) == 0) {
                return_code = OK;
            } else {
                if (!this_session->validated) {
                    return_code = WRONG_PASSWORD;
                } else {
                    return_code = CORRUPTED_TRANSFER;
                }
            }
        }
        // line ptr zeigt wieder auf heap damit fgets() stack buffer nicht overflowt.
        line = ptrl;
    }
    fclose(file);
    free(ptrl);
    return &return_code;
}
