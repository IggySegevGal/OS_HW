// classes.c
// contains job & jobs_class classes
// contains the functions that each method should have

/*******************************************/
/* define job and jobs classes*/
#include "classes.h"
using namespace std;
// ---------------------- job class functions: -----------------------------------
	//constructors
	job::job() {
		this->pid = getpid();
    	this->job_id = 0;
		this->command = string("/0");
		this->entry_time = time(NULL);
		this->is_stopped = false;
	}

	job::job(int pid,int job_id,string command,time_t entry_time,bool is_stopped) {
		this->pid = pid;
    	this->job_id = job_id;
		this->command = command;
		this->entry_time = entry_time;
		this->is_stopped = is_stopped;
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

	bool job::get_is_stopped() {
		return is_stopped;
	}

  
  // ---------------------- jobs_class functions: -----------------------------------
	jobs_class::jobs_class() {
	}

	jobs_class::jobs_class(vector<job> jobs_vector) {
		this->jobs_vector = jobs_vector;
	}

	jobs_class::~jobs_class() {}
  
  // class methods

	// insert a new job to class - insert from the back of the vector to maintain job_id order
	void jobs_class::insert_job(job new_job) {
      jobs_vector.push_back(new_job); // insert new job to the end of the list
      //job_num = jobs_vector.size(); // update list size
    return;
	}

	//print method - print all jobs in class
  void jobs_class::print_jobs(){
      std::vector<job>::iterator it;
      for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
         //print:
         cout <<"[" << it->get_job_id()<<"]";//job id
         cout << it->get_command()<< " : ";// command :
         cout << it->get_pid();//pid
         cout << it->get_time() << " secs";//seconds elapsed 
         if (it->get_is_stopped()){
            cout << "(stopped)";}// stopped (only is stopped)
		cout << endl;
      }
  }

	// remove a job from jobs_class
   int jobs_class::remove_job(int job_id){ // return 0 by success and -1 if object was not found
      vector<job>::iterator it;
      int job_removed = -1;
      for (it = jobs_vector.begin() ; it != jobs_vector.end(); ++it){
          if (it->get_job_id() == job_id){
            jobs_vector.erase(it);
            job_removed = 0;
          }
      }
      return job_removed;
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
