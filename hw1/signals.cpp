// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
extern int foreground_pid;
extern jobs_class jobs;
using namespace std;

/* ctrl C signal handler */
// send SIGKILL to foreground proccess
void ctrl_c(int sig_num) {
    // print:
    cout << "smash: caught ctrl-C" << endl;
    // check if there is any fg proccess running:
    if (foreground_pid == -1){

      return;
    }
    // send signal
   if(kill(foreground_pid, SIGKILL) == -1) { // child is killed and has to be removes from jobs list !!! also handle fg if
      perror("smash error: kill failed");
      return;
   }
   //remove job from list
   int job_id = jobs.get_job_id_by_pid(foreground_pid);
   if (jobs.job_exists(foreground_pid)) {
      jobs.remove_job(job_id);
   }

   //print to screen:
   cout << "smash: process "<< foreground_pid<< " was killed" << endl;
foreground_pid = -1;
   return;


}


/* ctrl Z signal handler */
// send SIGSTOP to foreground proccess
void ctrl_z(int sig_num) {
    // print:
    cout << "smash: caught ctrl-Z" << endl;
    // check if there is any fg proccess running:
    if (foreground_pid == -1){
	    
	return;
    }
    // send signal
   if(kill(foreground_pid, SIGSTOP) == -1) {
      perror("smash error: kill failed");
      return;
   }

   //print to screen:
   cout << "smash: process "<< foreground_pid<< " was stopped" << endl;
   foreground_pid = -1;
   return;


}
