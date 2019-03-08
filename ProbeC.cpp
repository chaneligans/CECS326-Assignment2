//
//  main.cpp
//  Probe_C
//
//  Created by Sadiq Sarwar on 2/26/19.
//  Copyright © 2019 Sadiq Sarwar. All rights reserved.
//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
#include "kill_patch.h"
using namespace std;


// declare my message buffer
struct buf {
    long mtype; // required
    char greeting[5]; // msg content
    int randInt;
}; buf msg;

void sendMsg(buf msg, int qid, int size){
    msg.mtype = 13;
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
}

int main() {
    // create my msgQ with key value from ftok()
    int qid = msgget(ftok(".",'u'), 0);
    
    buf msg;
    int size = sizeof(msg)-sizeof(long);
    
    msg.mtype = 13;
    strncpy(msg.greeting, "C", size);
    strncpy(msg.greeting, "EndC", size);
    
    kill_patch(qid, &msg, size, 13);
    
    int loop = 0;
    
    srand(time(0));
    while(loop != 1){
        msg.randInt = rand();
        
        if(msg.randInt % 251 == 0){
            sendMsg(msg, qid, size);
        }
    }
    
    exit(0);
}
