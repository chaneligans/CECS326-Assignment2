/* 
Dinh Luong, Chanel Mendoza, Sadiq Sarwar, Tyler Ton
March 12, 2019
CECS 326 - 05 Ratana Ngo

File name: probeA.cpp

Description: This program is one of three probes that sends data to a hub. This probe
continuously sends randomly generated numbers to the hub. What makes this probe unique
is that after it sends a message to the hub, it waits for a return message / 
acknowledgement from the hub before sending another message. Finally, this probe 
stops sending messages when it produces a random integer that is less than 100.

*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
using namespace std;

int main() {

    int qid = msgget(ftok(".",'u'), 0);

    // declare the message buffer
	struct buf {
		long mtype; // required
		char greeting[5]; // msg content
        int randInt;
	};

	// seed the rng
    srand(time(0));

    // initialize random number
    int randNum;
    randNum = rand();

    // probe A number
    int probeA;
    probeA = 997;

    // msg to send
    buf msg;
	int size = sizeof(msg)-sizeof(long);

    msg.mtype = 11;

    // continue looping until the rand number is less than 100
    while (randNum >= 100) {
        if (randNum % probeA == 0) {
            strncpy(msg.greeting, "A",size);
            msg.randInt = randNum;
            // send msg
	        msgsnd(qid, (struct msgbuf *)&msg, size, 0);
            
            // receive msg
            msgrcv(qid, (struct msgbuf *)&msg, size, 314, 0); // reading
	        cout << getpid() << ": gets reply" << endl;
	        cout << "reply: " << msg.greeting << endl;
	        cout << getpid() << ": now exits" << endl;
                       
        }
        randNum = rand();
    }

    //Notify Hub that probe A is ending
    strncpy(msg.greeting, "EndA",size);
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    cout << "Probe A is terminating"<< endl;
	exit(0);
}


