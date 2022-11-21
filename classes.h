#ifndef _COMMANDS_H
#define _COMMANDS_H
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
    time_t get_runtime();
    bool get_is_stopped();

};

class jobs_list {       // The class
private:
   vector<job> jobs_list;
    //int job_num;
   //int max_job_id;
public:  
   // constructors
   jobs_list();
   jobs_list(vector<job> jobs_list);
    
    // destructor
    ~jobs_list();

    // getters:

    //method functions
    void insert_job(job new_job);
    void print_jobs();

   }
   
   //int remove_job(job job_to_remove){
   //      find(this.begin(), this.end(), job_to_remove); // OK. B::operator== exists.
   //    jobs_list.push_back (new_job);
   }


