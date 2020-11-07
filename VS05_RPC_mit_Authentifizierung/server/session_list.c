//
// Created by Nelson Morais on 07.11.20.
// Praktikum 05 Verteilte Systeme Sessions verwaltung

#include "session_list.h"

//session einfügen ( session -> session -> new session )
session insert_session(session session_list, sessionid sid, user user) {
    session new_session = malloc(sizeof(struct session_list));
    new_session->sid = sid;
    new_session->user = strdup(user);
    new_session->next = NULL;
    while (session_list->next != NULL) {
        session_list = session_list->next;
    }
    session_list->next = new_session;
    return new_session;
}

//session löschen (s1 löschen: head -> s1 -> s2 // head -> s2 )
//session delete_session(session session_list){
//    session temp = session_list->next;
//    session_list->next = session_list->next->next;
//    free(temp);
//    return session_list;
//}

session search_session(session session_list, sessionid sid) {
    while (session_list != NULL) {
        if (session_list->sid == sid) {
            return session_list;
        }
        session_list = session_list->next;
    }
    return NULL;
}

session delete_session(session session_list, sessionid sid) {
    session temp;
    while (session_list != NULL) {
        if (session_list->next->sid == sid) {
            temp = session_list->next;
            session_list->next = session_list->next->next;
            free(temp);
            return session_list->next;
        }
        session_list = session_list->next;
    }
    return NULL;
}

void show_session_list(session session_list) {
    int counter = 0;
    while (session_list != NULL) {
        if (session_list->user != NULL) {
            printf("Session %d - user: %s sid: %d\n", ++counter, session_list->user, session_list->sid);
        }
        session_list = session_list->next;
    }

}