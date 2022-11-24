//		commands.c
//********************************************
#include "commands.h"
#DEFINE MAX_PATH 4096
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(jobs_class jobs, char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}

/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
		//check parameters
		if (num_arg > 1) 
			cout << "smash error: cd: too many arguments" << endl;
			return 1;
		else if (args[1] == "-")
			if (prev_path != NULL)
				path = prev_path; // "-" means go to prev dir
			else
				cout << "smash error: cd: OLDPWD not set" << endl;
				return 1;
			
		else 
			path = args[1]; // first argument is the path
		// change dir and check if successful
		if (chdir(path) != 0) {
			perror("smash error: chdir failed");
			return 1;
		}
	return 0;
		
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		char cwd[MAX_PATH];
		if (getcwd(cwd,sizeof(cwd)) == NULL) {
			perror("smash error: getcwd failed");
			return 1;
		}
		else{
			cout << cwd << endl;
		}
		return 0;

	}
	
	/*************************************************/
	//else if (!strcmp(cmd, "mkdir"))
	//{
 		
	//}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
		// ignore arguments 
		// TBD - add parameter check and maybe change illegal_cmd = TRUE;
		// remove_jobs() ????????????????
		jobs.print_jobs() //a function inside jobs_class
		return 0;
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		// ignore arguments
		// TBD - add parameter check and maybe change illegal_cmd = TRUE;
		cout << "smash pid is " << getpid() << endl;
		return 0;
		
	}
	/*************************************************/
	else if (!strcmp(cmd, "kill")) 
	{
		if (num_arg != 2 || args[1][0] != '-'){ // wrong num arg
			cout << "smash error: kill: invalid arguments" << endl;
			return 1;
		}
		else {
			string input_signal = args[1];
			input_signal.erase(0,1); //remove '-'
			int job_id = stoi(args[2]);
			int job_pid = jobs.get_pid_by_job_id(job_id);
			if (job_pid == -1){ // id not found
				cout << "smash error: kill: job-id " << job_id << " does not exist"<<endl;
				return 1;
			}
			else { // everything is ok, send signal
				if(kill(job_pid, stoi(input_signal)) == -1) {
					perror("smash error: kill failed");
					return 1;
				}
			return 0;
			}
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{
   		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "diff"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	//if (illegal_cmd == TRUE)
	//{
		//printf("smash error: > \"%s\"\n", cmdString);
		//return 1;
	//}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					
					/* 
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					
					/* 
					your code
					*/
			
			default:
                	// Add your code here
					
					/* 
					your code
					*/
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
/* int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
/* 	} 
	return -1;
} */ //*/
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		/* 
		your code
		*/
		
	}
	return -1;
}
