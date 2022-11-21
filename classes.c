// classes.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* define job and jobs classes*/
#include "classes.h"

class job {       // The class
  public:             // Access specifier
   int pid;
   int job_id;        
   string command;
   time_t entry_time;
   bool is_stopped;
   job(){

   } // constructor,should initialize parameters

    //bool operator< (const job &other) const { // < operator to sort jobs list
    //  return job_id < other.job_id;
    //}
    //bool operator==(const& job_to_cmp) const { return this->pid == job_to_cmp.pid;}

};

class jobs_list {       // The class
  public:             // Access specifier
   vector<job> jobs_list;
   //int job_num;
   //int max_job_id;
   
   void insert_new_job(job new_job){
       jobs_list.push_back(new_job); // insert new job to the end of the list
       job_num = jobs_list.size(); // update list size
   }

   void print_jobs(){
      vector<job>::iterator it;
      for (it = jobs_list.begin() ; it != jobs_list.end(); ++it){
         //print:
         //job id
         // command :
         //pid
         //seconds elapsed (using timediff())
         // stopped (only is stopped)
         
         //cout << // print all the thing needed for "jobs" built in functin
         
         //cout << ' ' << *it;
         //cout << '\n';}


   }
   
   //int remove_job(job job_to_remove){
   //      find(this.begin(), this.end(), job_to_remove); // OK. B::operator== exists.
   //    jobs_list.push_back (new_job);
   }
