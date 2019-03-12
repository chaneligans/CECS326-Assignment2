// The Probe B program created by Tyler Ton
// The program is used for Assignment 2 in CECS 326 
// Spring 2019 - Instructor: Ratana Ngo

/*
	This program will contain
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <time.h>
using namespace std;

int main()
{
	srand(time(NULL));		// call the seed fir random function
	int beta = 2049;		// prime number beta

	// initialize qid (match up with others)
	int qid = msgget(ftok(".", 'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[5]; // mesg content
        	int randInt;	// integer variable that stores valid random number
	};
	buf msg;
	int size = sizeof(msg) - sizeof(long);		// size of the message content (including char array and int variable)

	msg.mtype = 100;	// assign 100 to the mtype
    	strncpy(msg.greeting, "B", size);	// copy "B" to the char array in the message
    	msg.randInt = getpid();		// assign Probe B's pid to integer variable in the message
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);	// send the message contained the pid to the hub with the mtype 100
    	msg.mtype = 12;		// assign 12 to the mtype
	// the while loop that generates the random integer 
	// and send the valid one to the hub
	while (true)
	{
		msg.randInt = rand();		// generate a random integer and store it in the randInt variable in the message
		// check if the random integer is divisible into the beta number
		if (msg.randInt % beta == 0)
		{
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);	// send the valid random number to the hub
		}
	}
}
