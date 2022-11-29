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
		this->remove_ended_jobs();
		return num_jobs;
	}
   	
	int jobs_class::get_max_job_id(){
		this->remove_ended_jobs();
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
	  this->remove_ended_jobs();
      int job_removed = -1;
      for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
          if (it->get_job_id() == job_id){
            jobs_vector.erase(it);
            job_removed = 0;
			num_jobs = jobs_vector.size(); // update list size
			if(job_id == max_job_id){ //removing max job id
				vector<job>::iterator last_job = jobs_vector.end();
				max_job_id = last_job->get_job_id();
			 }
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
         cout <<"[" << it->get_job_id()<<"]";//job id
         cout << it->get_command()<< " : ";// command :
         cout << it->get_pid();//pid
         cout << it->get_time() << " secs";//seconds elapsed 
         if (strcmp(it->get_job_status().c_str(),"stopped") == 0){ // job is stopped
            cout << "(stopped)";}// stopped (only is stopped)
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

   int jobs_class::get_max_stopped_id(){ // return max stopped job id, return -1 if no stopped
    vector<job>::iterator it;
	this->remove_ended_jobs();
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

   	// remove ended jobs from jobs_class
    void jobs_class::remove_ended_jobs() { ///// ********************** when finished writing add to header
		vector<job>::iterator it;
		for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
			int st = waitpid(it->get_pid(), NULL, WNOHANG);
			if(st == -1) { // there is no child process present at all - error
				perror("smash error: waitpid failed");
				jobs.remove_job(it->get_job_id());
			}
			if (st > 0){ // job has exited in the past, but the return value was not yet collected (a so-called zombie process)
				jobs.remove_job(it->get_job_id());
			}
		}
		return;
   }