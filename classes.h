#ifndef _CLASSES_H
#define _CLASSES_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <iostream>
#include <cstring>
using namespace std;
// classes job and jobs_list:
class job { 
private:
   int pid;
   int job_id;        
   string command;
   time_t entry_time;
   string job_status;
public:  
   // constructors
   job();
   job(int pid,int job_id,string command,time_t entry_time,string job_status);

    // destructor
    ~job();

    // getters:
    int get_pid(); 
    int get_job_id();
    string get_command();
    int get_time();
    string get_job_status();

    // setters
    void set_job_status(string job_status);
 void set_entry_time(time_t entry_time);
};

class jobs_class { 
private:
   vector<job> jobs_vector;
   int num_jobs;
   int max_job_id;
public:  
   // constructors
   jobs_class();
   jobs_class(vector<job> jobs_vector);
    
    // destructor
    ~jobs_class();

    // getters:
   int get_num_jobs();
   int get_max_job_id();
   
    //method functions
    void remove_ended_jobs();
    void insert_job(job new_job);
    void print_jobs();
    int remove_job(int job_id); // return 0 by success and -1 if object was not found
    int get_pid_by_job_id(int job_id); // get job id and return pid, return -1 if failed
    string get_cmd_by_job_id(int job_id); // get job id and return job command, return NULL if failed
    void set_status_by_job_id(int job_id,string job_status);// get job id and change job status
    int get_max_stopped_id(); // return max stopped job id, return -1 if no stopped
    string get_job_status_by_job_id(int job_id);//get job id and return job status, return NULL if failed
    int get_job_id_by_pid(int job_pid); // get job pid and return id, return -1 if failed
    bool job_exists(int job_pid); // get a boolian = true if job is in list, and false if not
    void kill_all_jobs(); // kill all proccesses in jobs vector
void set_entry_time_by_id(int job_id,time_t entry_time); // set entry time by job id
   };
   

#endif
