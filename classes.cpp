// classes.c
// contains job & jobs_class classes
// contains the functions that each method should have

/*******************************************/
/* define job and jobs classes*/
#include "classes.h"
#include "commands.h"
using namespace std;
// ---------------------- job class functions: -----------------------------------
	//constructors
	job::job() { // TBD - Do WE NEED THIS CONSTRUCTOR ??!!! ****************************
		// MAYBE USE FORK
		this->pid = getpid();
    	this->job_id = 0;
		this->command = string("/0");
		this->entry_time = time(NULL);
		this->job_status = ""; // TBD!! 
	}

	job::job(int pid,int job_id,string command,time_t entry_time,string job_status) {
		// MAYBE USE FORK
		this->pid = pid;
    	this->job_id = job_id;
		this->command = command;
		this->entry_time = entry_time;
		this->job_status = job_status;
	}
	
	//destructor
	job::~job() {}
  
  	// class getters
	int job::get_pid() {
		return pid;
	}

	int job::get_job_id() {
		return job_id;
	}

	string job::get_command() {
		return command;
	}

	int job::get_time() {
		return time(NULL)-entry_time;
	}

	string job::get_job_status() {
		return job_status;
	}

	//setters
    void job::set_job_status(string job_status){
		this->job_status = job_status;
	}
	
    void job::set_entry_time(time_t entry_time){
		this->entry_time = entry_time;
	}


  
  // ---------------------- jobs_class functions: -----------------------------------
	jobs_class::jobs_class() {
		num_jobs = 0;
		max_job_id = 0;
	}

	// assumming last element is max id -----------------------------
	jobs_class::jobs_class(vector<job> jobs_vector) {
		this->jobs_vector = jobs_vector;
		num_jobs = jobs_vector.size();
		vector<job>::iterator last_job = jobs_vector.end();
		max_job_id = last_job->get_job_id();
	}

	jobs_class::~jobs_class() {}
  
    // getters:
   	int jobs_class::get_num_jobs(){
		return num_jobs;
	}
   	
	int jobs_class::get_max_job_id(){
		return max_job_id;
	}

  	// class methods
	// insert a new job to class - insert from the back of the vector to maintain job_id order
	void jobs_class::insert_job(job new_job) {
		this->remove_ended_jobs();
      //jobs_vector.push_back(new_job); // insert new job to the end of the list
		if(max_job_id < new_job.get_job_id()){
			jobs_vector.push_back(new_job);
			max_job_id = new_job.get_job_id();
		}
		else {
			vector<job>::iterator it;
			for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
				if(new_job.get_job_id() < it->get_job_id()){
					jobs_vector.insert(it,new_job);
					break;
				}
			
			}
		}
		num_jobs = jobs_vector.size(); // update list size
    	return;
	}

	// remove a job from jobs_class
   int jobs_class::remove_job(int job_id){ // return 0 by success and -1 if object was not found
      vector<job>::iterator it;
      int job_removed = -1;
      for (int i=0 ; i < num_jobs; i++){
          if (jobs_vector[i].get_job_id() == job_id){
            jobs_vector.erase(jobs_vector.begin()+i);
            job_removed = 0;
			num_jobs = jobs_vector.size(); // update list size
			if(job_id == max_job_id ){ //removing max job id
				
				if (num_jobs == 0) {max_job_id = 0;}
				else {
				max_job_id = jobs_vector.back().get_job_id();
				}
			 }
	  return job_removed;
          }
	
      }
      return job_removed;
   }

	//print method - print all jobs in class
  void jobs_class::print_jobs(){
      std::vector<job>::iterator it;
	  this->remove_ended_jobs(); 
      for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
         //print:
         cout <<"[" << it->get_job_id()<<"] ";//job id
         cout << it->get_command()<< " : ";// command :
         cout << it->get_pid() << " ";//pid
         cout << it->get_time() << " secs";//seconds elapsed 
         if (strcmp(it->get_job_status().c_str(),"stopped") == 0){ // job is stopped
            cout << " (stopped)";}// stopped (only is stopped)
		cout << endl;
      }
  }

	// get job id and return pid, return -1 if failed 
   int jobs_class::get_pid_by_job_id(int job_id) {
    vector<job>::iterator it;
	int pid = -1;
      for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
          if (it->get_job_id() == job_id){
			pid = it->get_pid();
          }
      }
	return pid;
   }

	// get job id and return job cmd, return -1 if failed 
    string jobs_class::get_cmd_by_job_id(int job_id) {
    vector<job>::iterator it;
	string cmd = "NULL";
      for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
          if (it->get_job_id() == job_id){
			cmd = it->get_command();
          }
      }
	return cmd;
   }

	// get job id and change status
   void jobs_class::set_status_by_job_id(int job_id,string job_status) {
    vector<job>::iterator it;
      for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
          if (it->get_job_id() == job_id){
			it->set_job_status(job_status);
          }
      }
	return;
   }

	void jobs_class::set_entry_time_by_id(int job_id,time_t entry_time){
    	vector<job>::iterator it;
      	for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
          if (it->get_job_id() == job_id){
			it->set_entry_time(entry_time);
          }
      	}
	return;
	}

   int jobs_class::get_max_stopped_id(){ // return max stopped job id, return -1 if no stopped
    vector<job>::iterator it;
	int job_id = -1;
      for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
          if (it->get_job_id() > job_id && strcmp(it->get_job_status().c_str(),"stopped") == 0 ){
			job_id = it->get_job_id();
          }
      }
	return job_id;
   }

   	// get job id and return job status, return -1 if failed 
    string jobs_class::get_job_status_by_job_id(int job_id) {
    vector<job>::iterator it;
	string status = "-1";
      for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
          if (it->get_job_id() == job_id){
			status = it->get_job_status();
          }
      }
	return status;
   }



	// get job pid and return id, return -1 if failed 
   int jobs_class::get_job_id_by_pid(int job_pid) {
    vector<job>::iterator it;
	int job_id = -1;
      for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
          if (it->get_pid() == job_pid){
			job_id = it->get_job_id();
          }
      }
	return job_id;
   }



	// get job pid and return id, return -1 if failed 
   bool jobs_class::job_exists(int job_pid) {
    vector<job>::iterator it;
	bool exists = false;
      for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
          if (it->get_pid() == job_pid){
			exists = true;
          }
      }
	return exists;
   }


   	// remove ended jobs from jobs_class
    void jobs_class::remove_ended_jobs() { ///// ********************** when finished writing add to header
		for (int i=0 ; i < num_jobs; i++){
			int st = waitpid(jobs_vector[i].get_pid(), NULL, WNOHANG);
			if(st == -1) { // there is no child process present at all - error
				perror("smash error: waitpid failed");
				if(job_exists(jobs_vector[i].get_pid())){remove_job(jobs_vector[i].get_job_id());}				
			}
			if (st > 0){ // job has exited in the past, but the return value was not yet collected (a so-called zombie process)
				if(job_exists(jobs_vector[i].get_pid())){remove_job(jobs_vector[i].get_job_id());}
			}
		}
		return;
   }


    void jobs_class::kill_all_jobs(){ // kill all proccesses in jobs vector
		this->remove_ended_jobs();
		/////// ******************** free memory !!!
				int curr_pid;
				int curr_id;
				string curr_cmd;
				jobs_class list_copy = *this;
		  		for (int i=0 ; i < list_copy.num_jobs; i++){
					curr_pid = list_copy.jobs_vector[i].get_pid();
					curr_id = list_copy.jobs_vector[i].get_job_id();
					curr_cmd = list_copy.jobs_vector[i].get_command();
					
					// -------------------------------------------------------------------
					// send SIGTERM
					cout <<"["<<curr_id <<"] " << curr_cmd <<" - Sending SIGTERM..." << flush;
					if(kill(curr_pid, SIGTERM) == -1) {
						perror("smash error: kill failed");
						return;
					}
					
					// wait 5 secs to see if SIGTERM succeeded
					sleep(5);
					
					// check if SIGTERM succeeded (proccess terminated)
					int status;
					int waitpid_return_value = waitpid(curr_pid, &status, WNOHANG);
					if (waitpid_return_value == -1) { // waitpid() failed
						perror("smash error: waitpid failed");
						return;
					}
					
					if ((WIFEXITED(status) || WIFSIGNALED(status) )&& waitpid_return_value==curr_pid ) { // job terminated
						cout << "Done." << endl;
						remove_job(curr_id);
						continue;
					}
					// -----------------------------------------------------------
					// kill proccess if not yet ended
					cout << "(5 sec passed) Sending SIGKILL..." << flush;

					// send SIGKILL
					if(kill(curr_pid, SIGKILL) == -1) {
						perror("smash error: kill failed");
						return;
					}
					
					// check if SIGKILL succeeded (proccess terminated)
					int status2;
					waitpid_return_value = waitpid(curr_pid, &status2, WNOHANG);
					if (waitpid_return_value == -1) { // waitpid() failed
						perror("smash error: waitpid failed");
						return;
					}
					
					if (WIFEXITED(status) || WIFSIGNALED(status)) { // job terminated
						cout << "Done." << endl;
						remove_job(curr_id);
						continue;
					}
					
				}
		return;

	}
