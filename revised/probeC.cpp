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


// Declaring Message Buffer
struct buf {
    long mtype; // Queue channel to hub
    char greeting[5]; // Recognition message
    int randInt; // Randomly generated conditional value
}; // Block End - Message Buffer Struct


// Function designed to send message buffer to the hub
void sendMsg(buf msg, int qid, int size){
    
    // Declaring mtype to be 13
    msg.mtype = 13;
    
    // If condition holds true, send message buffer to queue
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    
} // Block End - sendMsg(...) Function


int main() {
    
    // Creating queue connection to hub
    int qid = msgget(ftok(".",'u'), 0);
    
    // Creating local reference to message buffer
    buf msg;
    
    // Calculating the size of the message content
    int size = sizeof(msg)-sizeof(long);
    
    // Declaring mtype to be 13
    msg.mtype = 13;
    
    // Assigning termination message
    strncpy(msg.greeting, "EndC", size);
    
    // Assigning termination when function
    kill_patch(qid, (msgbuf *) &msg, size, 13);
    
    // Otherwise, assigning probe recognition message for the hub
    strncpy(msg.greeting, "C", size);
    
    // Assigning a seed to the random function
    srand(time(0));
    
    // Infinite looping until program is manually terminated
    while(true){
        
        // Assigning randomly generated integer to message buffer variable
        msg.randInt = rand();
        
        // Checking condition with regards to divisibility
        if(msg.randInt % 2011 == 0){
            
            // If condition holds true, call sendMsg(...) Function
            sendMsg(msg, qid, size);
        
        } // Block End - if(msg.randInt % 2011 == 0)
        
    } // Block End - while(true)
    
    exit(0);
}
