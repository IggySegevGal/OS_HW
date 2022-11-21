// classes.c
// contains job & job_list classes
// contains the functions that each method should have

/*******************************************/
/* define job and jobs classes*/
#include "classes.h"

// ---------------------- job class functions: -----------------------------------
	job::job() {
		this->pid = getpid();
    this->job_id = 0;
		this->command = string("/0");
		this->entry_time = time(NULL);
		this->is_stopped = FALSE;
	}
	job::job(int pid,int job_id,string command,time_t entry_time,bool is_stopped) {
		this->pid = pid;
    this->job_id = job_id;
		this->command = command;
		this->entry_time = entry_time;
		this->is_stopped = is_stopped;
	}

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

	bool job::get_is_stopped() {
		return is_stopped;
	}

  
  // ---------------------- jobs_class functions: -----------------------------------
	jobs_class::jobs_class() {
	this->jobs_list=NULL;
	}

	jobs_class::jobs_class(vector<job> jobs_vector) {
		this->pid = pid;
    this->job_id = job_id;
		this->command = command;
		this->entry_time = entry_time;
		this->is_stopped = is_stopped;
	}

	jobs_class::~jobs_class() {}
  
  // class methods
	void jobs_class::insert_job(job new_job) {
      jobs_vector.push_back(new_job); // insert new job to the end of the list
      job_num = jobs_vector.size(); // update list size
    return;
	}

  void jobs_class::print_jobs(){
      vector<job>::iterator it;
      for (it = jobs_list.begin() ; it != jobs_list.end(); ++it){
         //print:
         cout << it->get_job_id();//job id
         cout << it->get_command()<< ":";// command :
         cout << it->get_pid();//pid
         cout << it->get_time();//seconds elapsed 
         if (get_is_stopped()){
            cout << it->job_id;}// stopped (only is stopped)
      };
  };

   int remove_job(int job_id){ // return 0 by success and -1 if object was not found
      vector<job>::iterator it;
      int job_removed = -1
      for (it = jobs_list.begin() ; it != jobs_list.end(); ++it){
          if (it->job_id == job_id){
            jobs_list.erase(it);
            job_removed = 0;
          }
      };
      return job_removed;
   };
