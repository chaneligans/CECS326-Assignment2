/* 

This is a 

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

    // declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[5]; // mesg content
        int randInt;
	};

	// seed the rng
    srand(time(0));

    // initialize random number
    int randNum;
    randNum = rand();

    // probe A number
    int probeA;
    probeA = 2063;

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
            
            // recieve msg
            msgrcv(qid, (struct msgbuf *)&msg, size, 15, 0); // reading
//	        cout << getpid() << ": gets reply" << endl;
//	        cout << "reply: " << msg.greeting << endl;
//	        cout << getpid() << ": now exits" << endl;
            msg.mtype = 11;
        }
        randNum = rand();
	cout << randNum << endl;
    }

    //Notify Hub that probe A ending
    strncpy(msg.greeting, "EndA",size);
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    cout << "Probe A is terminating"<< endl;
	exit(0);
}


