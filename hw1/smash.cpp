/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#include "classes.h"

#define MAX_LINE_SIZE 80
#define MAXARGS 20
using namespace std;

char* L_Fg_Cmd; //cpp? 
jobs_class jobs; //This represents the list of jobs
char lineSize[MAX_LINE_SIZE]; 
string prev_path = "no_prev";
int foreground_pid = -1; // the pid of the proccess running in Foreground - if none -> -1
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	 

	// change signal handler table
	/* set the INT (Ctrl-C) signal handler to 'catch_int' */
	struct sigaction act1;
	sigfillset(&act1.sa_mask);
	act1.sa_flags = SA_RESTART;
	act1.sa_handler = &ctrl_c;
	if (sigaction(SIGINT, &act1, NULL) == -1) {//sigaction failed
	perror("smash error: sigaction failed");
	};  

	/* set the INT (Ctrl-Z) signal handler to 'catch_int' */
	struct sigaction act2;
	sigfillset(&act2.sa_mask);
	act2.sa_flags = SA_RESTART;
	act2.sa_handler = &ctrl_z;
	if (sigaction(SIGTSTP, &act2, NULL) == -1) {//sigaction failed
	perror("smash error: sigaction failed");	
	};  

	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

	/************************************/
	// Init globals 


	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		// lineSize is the pointer to an array of chars where the strinf read is stored
		// we read the commend from stdin to linesize
		strcpy(cmdString, lineSize); 
		// here we copy the cmd from user to local mains parameter called cndstring
		cmdString[strlen(lineSize)-1]='\0';
					// perform a complicated Command
		//if(!ExeComp(lineSize)) continue; 
					// background command	
	 	//if(!BgCmd(lineSize, jobs)) continue; 
					// built in commands
		ExeCmd(jobs, lineSize, cmdString, foreground_pid);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

