/* Program HUB */
// Hub mtype -14
// Probe A mtype 11 , Hub replies to Probe A mtype 15
// Probe B mtype 12
// Probe C mtype 13

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "force_patch.h"
using namespace std;

int main()
{
   cout << "HUB's PID is " << getpid() << endl;

   // create my msgQ with key value from ftok()
   int qid = msgget(ftok(".", 'u'), IPC_EXCL | IPC_CREAT | 0600);

   // declare my message buffer
   struct buf
   {
      long mtype;        // required
      char greeting[5]; // mesg content
      int randInt;         // random number
   };

   buf msg;
   int size = sizeof(msg) - sizeof(long);

   //Boolean variable to keep track of which probe has been terminated
   bool endA = false;
   bool endB = false;
   bool endC = false;

   //Message counter
   int counter = 0;

   //Gets the pid of B
   pid_t pidB;
   msgrcv(qid, (struct msgbuf *)&msg, size, 100, 0);
   pidB = msg.randInt;

   while (endA == false && endB == false && endC == false)
   {
      msgrcv(qid, (struct msgbuf *)&msg, size, -14, 0);
      counter += 1;
      //Check if the message from Probe A
      if (msg.greeting == "A" || msg.greeting == "EndA")
      {
         cout << "Probe A sends a message to Hub: "<< msg.randInt << endl;
         if (msg.greeting == "EndA")
         {
            cout << "Probe A exits" << endl;
            endA = true;
         } // Probe A has been terminated
         msg.mtype = 15;
         msg.randInt = 0;
         strncpy(msg.greeting, "Hub replies", size);
         cout << "Hub sends a message to A: " << msg.greeting << endl;
         msgsnd(qid, (struct msgbuf *)&msg, size, 0);
      }

      if (counter == 10000)
      {
         force_patch(pidB); // Force_path file
         endB = true;
      } // Force B to exit
      if (msg.greeting == "EndC")
      {
         endC = true;
      } // Probe C has been terminated by the user
   }

   cout << "Hub is terminating" << endl;

   msgctl(qid, IPC_RMID, NULL);
   exit(0);
}