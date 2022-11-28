//		commands.c
//********************************************
#include "commands.h"
#define MAX_PATH 4096
using namespace std;
extern string prev_path;
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
	char const * delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	char* curr_dir_pointer = get_current_dir_name();
	string curr_path = curr_dir_pointer;
	free(curr_dir_pointer);
	bool illegal_cmd = false; // illegal command
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
		if (num_arg > 1) {
			cout << "smash error: cd: too many arguments" << endl;
			return 1;
		}
		else if (!strcmp(args[1],"-")){ // if equal to "-"
			if (!strcmp(prev_path.c_str(),"no_prev")){ // if equal - no prev_path
				cout << "smash error: cd: OLDPWD not set" << endl;
				return 1;	
			}
			else{// "-" means go to prev dir
				// change dir and check if successful
				if (chdir(prev_path.c_str()) != 0) {
					perror("smash error: chdir failed");
					return 1;
				}
				prev_path = curr_path;
				return 0;
			}
		}
		else{ 
			// change dir and check if successful
			if (chdir(args[1]) != 0) {
				perror("smash error: chdir failed");
				return 1;
			}
		}
	prev_path = curr_path;
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
		jobs.print_jobs(); //a function inside jobs_class
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
				else{//signal was send, update job status
					if (stoi(input_signal) == 19 || stoi(input_signal) == 20){ //job stopped
						jobs.set_status_by_job_id(job_id,"stopped");
					}
					else if (stoi(input_signal) == 18){ // SIGCONT without wait is background
						jobs.set_status_by_job_id(job_id,"background");
					}
				}
			return 0;
			}
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{
		int curr_job_id = 0;
		//invalid arguments
		if (num_arg == 0) {
			//check if list empry
			if(jobs.get_num_jobs() == 0 ){
				cout << "smash error: fg: jobs list is empty" << endl;
				return 1;
			}
			else{
				curr_job_id = jobs.get_max_job_id();
			}
		}
		//assumming arg[1] is int
		else if(num_arg == 1){ 
			 curr_job_id = stoi(args[1]);
			if(jobs.get_pid_by_job_id(curr_job_id) == -1 ){
				cout << "smash error: fg: job-id "<< curr_job_id << " does not exist" << endl;
				return 1;

			}
		}

		else {
			cout << "smash error: fg: invalid arguments" << endl;
			return 1;
		}

		//everything is ok :)))))))))))
		int curr_pid = jobs.get_pid_by_job_id(curr_job_id);
		string curr_job_cmd = jobs.get_cmd_by_job_id(curr_job_id);
		//print the cmd line and pid
		cout << curr_job_cmd << " : " << curr_pid << endl;
		//sending SIGCONT to current pid
		if(kill(curr_pid, SIGCONT) == -1) {
			perror("smash error: kill failed");
			return 1;
		}
		// TBD remove_job();
		int status;
		// add something to handle ctrl z,c 
		int waitpid_return_value = waitpid(curr_pid, &status, WUNTRACED);
		if(waitpid(curr_pid, &status, WUNTRACED) != curr_pid) { // TBD ********************** maybe not NULL to handle STOP from keyboard
			perror("smash error: waitpid failed");
			return 1;
		}
		if (waitpid_return_value == curr_pid && WIFSTOPPED(status)){ // child was stopped
			jobs.set_status_by_job_id(curr_job_id,"stopped");
			return 0;
		}
		if (waitpid_return_value == curr_pid && WIFEXITED(status)){ // child terminated
			jobs.remove_job(curr_job_id);
			return 0;
		}
		// if ctrl z,c return job to list (ask lior where to put)
		return 0;
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		//input check
		int curr_job_id = 0;
		int max_stopped_job_id = jobs.get_max_stopped_id();
		if (num_arg == 0) {// get max stopped job id
			if (max_stopped_job_id == -1){ // no stopped jobs in list
				perror("smash error: bg: there are no stopped jobs to resume");
				return 1;
			}
			else{ // there is a stopped job in list
				curr_job_id = max_stopped_job_id;
			}

		}
		
		else if(num_arg == 1){
			curr_job_id = stoi(args[1]);
			if (jobs.get_pid_by_job_id(curr_job_id) == -1 ){ // job id doesnt exist
				cout << "smash error: bg: job-id " << curr_job_id << " does not exist" << endl;
				return 1;
			}
			else if(strcmp(jobs.get_job_status_by_job_id(curr_job_id).c_str(),"stopped") != 0){// job isnt stopped
				cout << "smash error: bg: job-id " << curr_job_id << " is already running in the background" << endl;
				return 1;
			}
		}	
			// job exists and stopped
		else { //invalid arguments
			cout << "smash error: bg: invalid arguments" << endl;
			return 1;
		}
		// everything is ok :)))))
		//move job to background:
		int curr_job_pid = jobs.get_pid_by_job_id(curr_job_id);
		//sent SIGCONT to job
		if (kill(curr_job_pid, SIGCONT) == 0){ // success
			//print job 
			cout << jobs.get_cmd_by_job_id(curr_job_id) << " : " << curr_job_pid << endl;
			// return to running
			jobs.set_status_by_job_id(curr_job_id,"background");
		}
		else { // kill failed
			perror("smash error: kill failed");
			return 1;
		}

		return 0;
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
	if (illegal_cmd == true)
	{
		// MAYBE USE IF COMMAND NOT FOUND
		//printf("smash error: > \"%s\"\n", cmdString);
		//return 1;
	}
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
    		case -1:{}
			
        	case 0:{
			setpgrp();// Child Process		
		}

		default:{}

	}
	
}
//**************************************************************************************

//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char const * delimiters = " \t\n";
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

