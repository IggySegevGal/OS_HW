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
   bool is_stopped;
public:  
   // constructors
   job();
   job(int pid,int job_id,string command,time_t entry_time,bool is_stopped);

    // destructor
    ~job();

    // getters:
    int get_pid(); 
    int get_job_id();
    string get_command();
    int get_time();
    bool get_is_stopped();

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
    void insert_job(job new_job);
    void print_jobs();
    int remove_job(int job_id); // return 0 by success and -1 if object was not found
    int get_pid_by_job_id(int job_id); // get job id and return pid, return -1 if failed
    string get_cmd_by_job_id(int job_id); // get job id and return job command, return -1 if failed
   };
   

#endif
