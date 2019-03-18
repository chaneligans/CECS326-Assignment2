/* Program HUB */
// Hub mtype -14
// Probe A mtype 11 , Hub replies to Probe A mtype 15
// Probe B mtype 12
// Probe C mtype 13

/*
Created by Dinh Luong
CECS 326 - Assignment #2
File name: hub.cpp
Description: The hub will receive data from 3 probes. Hub will
             display the message and random number sent from a
             probe (A, B or C). Hub ends when 3 probes have been
             ended
*/

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
{ // Print out Hub's PID
   cout << "HUB's PID is " << getpid() << endl;

   // create my msgQ with key value from ftok()
   int qid = msgget(ftok(".", 'u'), IPC_EXCL | IPC_CREAT | 0600);

   // declare my message buffer
   struct buf
   {
      long mtype;       // required
      char greeting[5]; // message content
      int randInt;      // random number
   };

   buf msg;                               // create msg
   int size = sizeof(msg) - sizeof(long); // define size

   //Boolean variable to keep track of which probe has been terminated
   bool endA = false;
   bool endB = false;
   bool endC = false;

   //Message counter: counts the message received by Hub
   int counter = 0;

   //Gets the pid of B
   pid_t pidB;
   msgrcv(qid, (struct msgbuf *)&msg, size, 100, 0);
   pidB = msg.randInt;

   while (endA == false || endB == false || endC == false) // Hub exits when all 3 probes exited
   {
      msgrcv(qid, (struct msgbuf *)&msg, size, -14, 0); // receive message from a probe
      counter += 1; // increase message counter
      cout << msg.greeting << ": " << msg.randInt << " " << counter << endl; // Print out the message and random number

      //Check if the message from Probe A
      if (strncmp(msg.greeting, "A", 1) == 0 || strncmp(msg.greeting, "EndA", 4) == 0)
      {
         cout << "Probe A sends a message to Hub: " << msg.randInt << endl;

         // Check if probe A exited
         if (strncmp(msg.greeting, "EndA", 4) == 0)
         {
            cout << "Probe A exits" << endl;
            endA = true;
            continue;
         } // Probe A has been terminated

         // Hub replies probe A before receiving next message
         msg.mtype = 15;
         msg.randInt = 0;
         strncpy(msg.greeting, "Hub replies", size);
         cout << "Hub sends a message to A: " << msg.greeting << endl;
         msgsnd(qid, (struct msgbuf *)&msg, size, 0);
      }

      // Check if message counter reaches 10000. 
      // If yes, force probe B to exit, set endB to true
      if (counter == 10000)
      {
         cout << counter << endl;
         force_patch(pidB); // Force_path file
         endB = true;
      }

      // Check if probe C exited. If yes, set endC to true
      if (strncmp(msg.greeting, "EndC", 4) == 0)
      {
         endC = true;
      }
   }

   cout << "Hub is terminating" << endl;

   msgctl(qid, IPC_RMID, NULL); // deallocate the queue
   exit(0); 
}
