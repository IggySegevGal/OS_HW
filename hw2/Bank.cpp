// this is THE BANK! we hold accounts, and let users use ATMs to handle them.
// this is the main Bank function. this funtion gets N files to handle, and calls N treads (ATMs) to handle them.
#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

/*globals*/
bool all_treads_finished;
/* init global bank accounts */
 accounts bank_account;


/* handle_command function
gets a command line and thread data struct and executes command*/
 void  handle_command(string curr_command, thread_data_t * data){

 }

/* ATM routine function
get a file, and start executing commands line by line using handle_command */
void* ATM_routine(void* arg){ 
    /*cast input to thread data*/
    thread_data_t *data= (thread_data_t *)arg;

    /*read ATMfile line by line*/
    string file_name = data->file_name;
    fstream ATMfile;
    ATMfile.open(file_name,ios::in); //open a file to perform read operation using file object
    if (ATMfile.is_open()){   //checking whether the file is open
        string curr_command;
        while(getline(ATMfile, curr_command)){ //read data from file object and put it into string.
            handle_command(curr_command, data);
        }
        ATMfile.close(); //close the file object.
    }
    else {
        fprintf(stderr, "Bank error: open failed");
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}


/* commissions routine function
gets a pthread_data struct, and is responsible to collect commisions from each account*/
void* commisions_routine(void* arg){
    /*cast input to thread data*/
    thread_data_t *data= (thread_data_t *)arg;
    
    /*every 3 seconds, do:
    randomly select number between 1-5 %*/

    /* for each account - remove ((int)(account_balace * rand_num / 100)) and add to bank balance*/
    /* if (all_treads_finished) - global variable
    pthread_exit(NULL);*/
}

/* print routine function
responsible for printing every half a second*/
void* print_routine(void* arg){
    /*cast input to thread data*/
    thread_data_t *data= (thread_data_t *)arg;
    
    /*every half second, do:
    print all numbers (it's ok to wait if accounts are blocked)*/

    /* if (all_treads_finished) - global variable
    pthread_exit(NULL);*/
}

/* create thread argument struct for thr_func() */
typedef struct _thread_data_t {
    int thread_id;
    string file_name;
} thread_data_t;


// main function:
// return 0 on success 1 on failure
int main(int argc, char *argv[])  // responsible for initializing threads and calling them
{
    /*initialize finish flag to false*/
    all_treads_finished = false;
    // ********** create Bank units using threads ***********
    // create ATMs: (each ATM is responsible to handle one input file) (argc = num files + 1)
    files_num = argc - 1;
    if (files_num <= 0) { // no files passed to handle
        fprintf(stderr, "Bank error: illegal arguments");
        return 1;
    }

    // create ATMs threads array for each file 
    pthread_t* atm_threads = new pthread_t[files_num];
    thread_data_t* atm_threads_data = new thread_data_t[files_num]; // ask lior about using new and initializing array with changing size
    
    // create commision thread 
    pthread_t commision_thread; 
    thread_data_t commision_thread_data;

    // create print thread 
    pthread_t print_thread; 
    thread_data_t print_thread_data;
    
    int i, rc;
    /* create threads */
    for (i = 0; i < files_num; ++i) {
        atm_threads_data[i]->thread_id = i;
        atm_threads_data[i]->file_name = argv[i+1]; // ignore first argv - exe cmd // ask lior about [i] - dereference or pointer? 
        if ((rc = pthread_create(&atm_threads[i], NULL, ???routin func??? , atm_threads_data[i]))) {  // ask lior about &
            perror('Bank error: pthread_create failed');
            return 1;
        }
    }

    /* init commision_thread_data */
    commision_thread_data.thread_id = files_num; // last id
    commision_thread_data.file_name = ""; 
    
    /* create commision handler: (responsible to collects commision every 3 seconds) */
    if ((rc = pthread_create(&commision_thread, NULL, ???routin func commisiom??? , &commision_thread_data))) { //ask lior &
            perror('Bank error: pthread_create failed');
            return 1;
    }

    /* init print_thread_data */
    print_thread_data.thread_id = files_num+1; // last id + 1
    print_thread_data.file_name = ""; 

    /* create print handler: (responsible to collects commision every 3 seconds) */
    if ((rc = pthread_create(&print_thread, NULL, ???routin func commisiom??? , &print_thread_data))) { //ask lior &
            perror('Bank error: pthread_create failed');
            return 1;
    }

    /* block until all threads complete */
    for (int j = 0; j < files_num; ++j) {
        pthread_join(atm_threads[j], NULL);
    }
    
    /*set finish flag to true*/
    all_treads_finished = true;

    /*wait for print and commision threads to finish*/
    pthread_join(print_thread, NULL);
    pthread_join(commision_thread, NULL);

    delete[] atm_threads;
    delete[] atm_threads_data;
    return 0;
    
}