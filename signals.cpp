// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
using namespace std;

/* ctrl C signal handler */
// send SIGKILL to foreground proccess
void ctrl_c(int sig_num, int fg_pid) {
    // print:
    cout << "smash: caught ctrl-C" << endl;
    // check if there is any fg proccess running:
    if (fg_pid == -1){
      return;
    }
    // send signal
   if(kill(fg_pid, SIGKILL) == -1) {
      perror("smash error: kill failed");
      return;
   }
   //print to screen:
   cout << "smash: process "<< fg_pid<< " was killed" << endl;
   return;


}


/* ctrl Z signal handler */
// send SIGSTOP to foreground proccess
void ctrl_z(int sig_num, int fg_pid) {
    // print:
    cout << "smash: caught ctrl-Z" << endl;
    // check if there is any fg proccess running:
    if (fg_pid == -1){
      return;
    }
    // send signal
   if(kill(fg_pid, SIGSTOP) == -1) {
      perror("smash error: kill failed");
      return;
   }

   //print to screen:
   cout << "smash: process "<< fg_pid<< " was stopped" << endl;
   return;


}
