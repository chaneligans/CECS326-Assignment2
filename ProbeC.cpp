//
//  ProbeC.cpp
//  CECS326_Assignment#2
//
//  Created by Sadiq Sarwar on 2/28/19.
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
    char greeting[50]; // mesg content
}; buf msg;

void sendMsg(buf msg, int qid, int randInt, int size){
    msg.mtype = 13;
    cout << "Probe C: " + randInt << endl;
    strcpy(msg.greeting, "Probe C: " + randInt);
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
}

int main() {
    // create my msgQ with key value from ftok()
    int qid = msgget(ftok(".",'u'), 0);
    
    buf msg;
    int size = sizeof(msg)-sizeof(long);
    
    msg.mtype = 13;
    strcpy(msg.greeting, "Probe C: EXIT");
    
    kill_patch(qid, &msg, size, 13);
    
    int loop = 0;
    srand(time(0));
    
    while(loop != 1){
        int randInt = rand();
        
        if(randInt % 251 == 0){
            cout << randInt << " Divisible!" << endl;
            sendMsg(msg, qid, randInt, size);
        }
    }
    
    exit(0);
}
