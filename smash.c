/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types>
#include <sys/wait>
#include <unistd> 
#include <stdio>
#include <stdlib>
#include <string>
#include <signal>
#include "commands.h"
#include "signals.h"
#include "classes.h"

const int MAX_LINE_SIZE 80;
const int MAXARGS 20;
using namespace std;

char* L_Fg_Cmd; //cpp? 
jobs_class jobs = NULL; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE]; 
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	   

	
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
		if(!ExeComp(lineSize)) continue; 
					// background command	
	 	if(!BgCmd(lineSize, jobs)) continue; 
					// built in commands
		ExeCmd(jobs, lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

