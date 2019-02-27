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
		char greeting[50]; // mesg content
	};

	// seed the rng
    srand(time(0));

    // initialize random number
    int randNum;
    randNum = rand();

    // probe A number
    int probeA;
    probeA = 997;

    // send msg again
    bool msgRecieved;
    msgRecieved = true;

    // msg to send
    buf msg;
	int size = sizeof(msg)-sizeof(long);

    msg.mtype = 111;

    // continue looping until the rand number is less than 100
    while (randNum >= 100) {
        if (randNum % probeA == 0) {
            strcpy(msg.greeting, "Probe A" + randNum);
            if (msgRecieved) {
                // send msg
	            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
                msgRecieved = false;

                // recieve msg
                msgrcv(qid, (struct msgbuf *)&msg, size, 314, 0); // reading
	            cout << getpid() << ": gets reply" << endl;
	            cout << "reply: " << msg.greeting << endl;
	            cout << getpid() << ": now exits" << endl;
                msgRecieved = true;
            }
        }
        randNum = rand();
    }
	exit(0);
}


