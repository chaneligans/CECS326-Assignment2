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
	srand(time(NULL));
	int beta = 257;		// needs to choose wisely

	// match up with others
	int qid = msgget(ftok(".", 'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[5]; // mesg content
        int randInt;
	};
	buf msg;
	int size = sizeof(msg) - sizeof(long);

	msg.mtype = 100;
    strncpy(msg.greeting, "B", size);
    msg.randInt = getpid();
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    msg.mtype = 12;
	while (true)
	{
		msg.randInt = rand();
		if (msg.randInt % beta == 0)
		{
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		}
	}
}
