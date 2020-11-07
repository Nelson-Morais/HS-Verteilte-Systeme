//
// Created by Nelson Morais on 30.10.20.
// Verteilte Systeme RPC Dispatcher Praktikum 04


#include <arpa/inet.h>
#include <time.h>

#include "RPC_Protocol_files/pub_sub.h"
#include "RPC_Protocol_files/return_codes.h"
#include "RPC_Protocol_files/pub_sub_deliv.h"
#include "session_list.h"

#define LOCALADDRESS "127.0.0.1"
#define MAXSUBS 128
#define IPV4LENGTH 16

char *current_topic;
char subs[MAXSUBS][IPV4LENGTH];

session sessions = NULL;

static int flag = 0;
static int counter = 0;

short *set_channel_1_svc(topic *topic, struct svc_req *request) {
    //return_code static declariert ( auf static angelet ) damit der return nicht die referenz aufm stack ist.
    static RET_CODE return_code;
    current_topic = malloc(sizeof(char) * TOPLEN);
    char *req_addr = inet_ntoa(request->rq_xprt->xp_raddr.sin_addr);

    //check ob es local ist
    if ((strcmp(req_addr, LOCALADDRESS)) == 0) {
        strcpy(current_topic, *topic);
        printf(" --- Setting topic: %s --- \n", *topic);
        return_code = OK;
    } else {
        return_code = CANNOT_SET_TOPIC;
    }
    return &return_code;
}

short *subscribe_1_svc(void *ptr, struct svc_req *request) {
    //return_code static declariert ( auf static angelet ) damit der return nicht die referenz aufm stack ist.
    static RET_CODE return_code;

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
    return &return_code;

}

extern short *unsubscribe_1_svc(void *ptr, struct svc_req *request) {
    //return_code static declariert ( auf static angelet ) damit der return nicht die referenz aufm stack ist.
    static RET_CODE return_code;
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
    return &return_code;
}

extern short *publish_1_svc(message *msg, struct svc_req *request) {
    //return_code static declariert ( auf static angelet ) damit der return nicht die referenz aufm stack ist.
    static RET_CODE return_code;
    CLIENT *clnt;
    char *postmessage = malloc(sizeof(char) * POSTMESLEN);
    char *req_addr = inet_ntoa(request->rq_xprt->xp_raddr.sin_addr);

    if (counter > 0) {
        for (int i = 0; i < counter; i++) {
            //PUBSUBCLTPROG & PUBSUBCLTVER für delivery in pub_sub_deliv declariert
            clnt = clnt_create(subs[i], PUBSUBCLTPROG, PUBSUBCLTVERS, "tcp");
            snprintf(postmessage, POSTMESLEN, "%s;%s;%s", req_addr, *msg, current_topic);
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
    return &return_code;
}

extern  sessionid * get_session_1_svc(user * usr, struct svc_req * request){
    static sessionid sid = 0;


    //init struct session verwaltung
    if(!flag){
         sessions = malloc(sizeof(session));
         sessions->next = NULL;
         flag =1;
    }
    sid = clock();
    insert_session(sessions,sid,*usr);
    insert_session(sessions,78,"sadssda");
    insert_session(sessions,324234,"14134134");
    show_session_list(sessions);
    delete_session(sessions,78);
    show_session_list(sessions);

    return &sid;
}

extern  short * validate_1_svc(param *param, struct svc_req *request){

    return NULL;
}

extern  short * invalidate_1_svc(sessionid * sid, struct svc_req * request){

    return NULL;
}