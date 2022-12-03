//		commands.c
//********************************************
#include "commands.h"
#define MAX_PATH 4096
using namespace std;
extern string prev_path;
extern char* L_Fg_Cmd;
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(jobs_class &jobs, char* lineSize, char* cmdString, int &foreground_pid)
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
	// remove ended jobs
	jobs.remove_ended_jobs();

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
				else{//signal was send, update job status ************************************************* handle other signals!!!! like SIGSTOP and SIGCONT
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
				cout << "in fg function - max_job_id is " << curr_job_id << endl;
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
		foreground_pid = curr_pid;
		int status;
		// add something to handle ctrl z,c 
		int waitpid_return_value = waitpid(curr_pid, &status, WUNTRACED);
		if(waitpid_return_value != curr_pid) { // TBD ********************** maybe not NULL to handle STOP from keyboard
			perror("smash error: waitpid failed");
			return 1;
		}
		if (waitpid_return_value == curr_pid && WIFSTOPPED(status)){ // child was stopped
			jobs.set_status_by_job_id(curr_job_id,"stopped");
			foreground_pid = -1;
			return 0;
		}
		//if(waitpid_return_value == curr_pid && WTERMSIG(status) == SIGKILL){ // child was killed and handeld
		//foreground_pid = -1;
		//	return 0;
		//}
		if (waitpid_return_value == curr_pid && WIFEXITED(status)){ // child terminated -- what about child killed by ctrl - c ?? is it zombie ?!
			   if (jobs.job_exists(curr_pid)) {
     				 jobs.remove_job(curr_job_id);
			    }			
			foreground_pid = -1;
			return 0;
		}
		
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
		// check if kill routine needed: (if needed, handle inside if)
		if(num_arg > 0){ // quit and maybe also kill
			if(strcmp(args[1], "kill") == 0){//if second argument is kill
			// handle kill:
				jobs.kill_all_jobs();
			}
		}
		// free memory
		free(L_Fg_Cmd);
		// handle quit smash:
		exit(0);
	} 
	/*************************************************/
	else if (!strcmp(cmd, "diff"))
	{
   		if (num_arg != 2){//input check
			cout << "smash error: diff: invalid arguments" << endl;
			return 1;
		}
		int zero_if_equal = 0;
		// save files paths
		string file1_path = args[1];
		string file2_path = args[2];

		fstream f1,f2;
		//open files
		f1.open(file1_path,ios::in);
		f2.open(file2_path,ios::in);

		// check if fopen succeeded
    	if(f1==NULL || f2==NULL){
			perror("smash error: fopen failed");
			return 1;
		}
        
		char c1,c2;

		while(1){
			c1=f1.get();
			c2=f2.get();
			if(c1!=c2){
				zero_if_equal=1;
				break;
			}
			if((c1==EOF)||(c2==EOF))
				break;
		}
		f1.close();
		f2.close();
		cout << zero_if_equal << endl;
		return 0;

	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(jobs,args, cmdString, num_arg,foreground_pid);
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
int ExeExternal(jobs_class &jobs,char *args[MAX_ARG], char* cmdString, int num_arg, int &foreground_pid)
{
	int pID;
    	switch(pID = fork()) // we have a bug!!!! if child execv fails, father keeps going without knowing
	{
    		case -1:{ // fork failed
				perror("smash error: fork failed");
				return 1;
			}
			
        	case 0:{// Child Process
				setpgrp();	//change group ID
				execv(args[0], args);// execute the needed process
				perror("smash error: execv failed"); // Handle execv error if you got to this line
				exit(1);	
			}

			default:{ // father proccess - check if background or forground command
				if (strcmp(args[num_arg],"&") == 0) {//if last arg is & - the proccess should run in background
					//create new job
					job new_job = job(pID, jobs.get_max_job_id()+1, cmdString, time(NULL),"background");
					//insert job to jobs list
					jobs.insert_job(new_job);
					// the proccess is running in the child code
					cout << "got here" << endl;					
					return 0;
				}
				else { // the process should run in foreground
					int status;					

					foreground_pid = pID;
					int waitpid_return_value = waitpid(pID, &status, WUNTRACED);
					if(waitpid_return_value != pID) { 
						perror("smash error: waitpid failed");
						return 1;
					}
					
					if (waitpid_return_value == pID && WIFSTOPPED(status)){ // child was stopped
						//create new job
						job new_job = job(pID, jobs.get_max_job_id()+1, cmdString, time(NULL),"stopped");
						//insert job to jobs list
						jobs.insert_job(new_job);
						foreground_pid = -1;

						return 0;
					}
					if (waitpid_return_value == pID && WIFEXITED(status)){ // child terminated
						//terminated before entering jobs class - do nothing
						foreground_pid = -1;

						return 0;
					}
				}

			}

	}
	return 0;
	
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

