//
// Created by Nelson Morais on 07.11.20.
// Praktikum 05 Verteilte Systeme Sessions verwaltung

#include "session_list.h"

//session einfügen ( session -> session -> new session )
session insert_session(session session_list, sessionid sid, user user) {
    session new_session = malloc(sizeof(struct session_list));
    new_session->sid = sid;
    new_session->user = strdup(user);
    new_session->validated = FALSE;
    new_session->next = NULL;
    while (session_list->next != NULL) {
        session_list = session_list->next;
    }
    session_list->next = new_session;
    return new_session;
}


int validate_session(session session_list , sessionid sid){

    session session_tmp = search_session(session_list, sid);
    if(session_tmp != NULL){
        session_tmp->validated=TRUE;
        return TRUE;
    }
    return FALSE;
}

session search_session(session session_list, sessionid sid) {

    session session_tmp = session_list;

    while (session_tmp != NULL) {
        if (session_tmp->sid == sid) {
            return session_tmp;
        }
        session_tmp = session_tmp->next;
    }
    return NULL;
}

int delete_session(session session_list, sessionid sid) {
    session temp;
    while (session_list != NULL) {
        if (session_list->next->sid == sid) {
            temp = session_list->next;
            session_list->next = session_list->next->next;
            free(temp);
            return TRUE;
        }
        session_list = session_list->next;
    }
    return FALSE;
}

void show_session_list(session session_list) {
    int counter = 0;
    while (session_list != NULL) {
        if (session_list->user != NULL) {
            printf("Session %d - user: %s sid: %d validated: %d\n", ++counter, session_list->user, session_list->sid, session_list->validated);
        }
        session_list = session_list->next;
    }

}