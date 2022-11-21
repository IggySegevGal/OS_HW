#ifndef _CLASSES_H
#define _CLASSES_H
#include <unistd> 
#include <stdio>
#include <time>
#include <stdlib>
#include <signal>
#include <string>
#include <sys/types>
#include <sys/wait>

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
    time_t get_time();
    bool get_is_stopped();

};

class jobs_class { 
private:
   vector<job> jobs_vector;
    //int job_num;
   //int max_job_id;
public:  
   // constructors
   jobs_list();
   jobs_list(vector<job> jobs_vector);
    
    // destructor
    ~jobs_list();

    // getters:

    //method functions
    void insert_job(job new_job);
    void print_jobs();
    int remove_job(int job_id); // returnד 0 by success and -1 if object was not found
    int get_pid_by_job_id(int job_id) // get job id and return pid, return -1 if failed

   };
   


