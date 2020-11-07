//
// Created by Nelson Morais on 30.10.20.
// Verteilte Systeme RPC Client Receiver Praktikum 04

#include <stdio.h>
#include <rpc/rpc.h>
#include <string.h>
#include "RPC_Protocol_files/pub_sub_deliv.h"

void * deliver_1_svc(postmessage *postmessage, struct svc_req *request){

    char *msg;
    msg = strtok(*postmessage,";");
    if(strcmp(msg,"127.0.0.1")==0){
        printf("Message from: Server\n");
    }else {
        printf("Message from: %s\n", msg);
    }
    msg = strtok(NULL,";");
    printf("-*- %s -*-\n",msg);
    msg = strtok(NULL,";");
    printf("Message topic: %s\n",msg);


}

