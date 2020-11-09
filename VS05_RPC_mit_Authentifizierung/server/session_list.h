//
// Created by Nelson Morais on 07.11.20.
// Praktikum 05 Verteilte Systeme Sessions verwaltung

#ifndef VS05_RPC_MIT_AUTHENTIFIZIERUNG_SESSION_LIST_H
#define VS05_RPC_MIT_AUTHENTIFIZIERUNG_SESSION_LIST_H


#include "RPC_Protocol_files/pub_sub.h"

struct session_list {
    sessionid sid;
    user user;
    int validated;
    struct session_list *next;
};
typedef struct session_list *session;

session insert_session(session, sessionid, user);

session search_session(session , sessionid);

int delete_session(session, sessionid);

void show_session_list(session);

int validate_session(session session_list , sessionid sid);

#endif //VS05_RPC_MIT_AUTHENTIFIZIERUNG_SESSION_LIST_H
