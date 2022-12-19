// this is THE BANK! we hold accounts, and let users use ATMs to handle them.
// this is the main Bank function. this funtion gets N files to handle, and calls N treads (ATMs) to handle them.
#include <iostream>
#include <cstring>

// ATM start routine: (everytime an ATM is created, this is the start routine)


// parse input - this function gets a legal ATM command, and returns ??????

    /* create thread argument struct for thr_func() */
    typedef struct _thread_data_t {
    int thread_id;
    string file_name;
    accounts* bank_account_pt;
    } thread_data_t;

// main function:
// return 0 on success 1 on failure
int main(int argc, char *argv[])  // responsible for initializing threads and calling them
{
    /* init bank accounts */
    accounts bank_accounts; 


    // ********** create Bank units using threads ***********
    // create ATMs: (each ATM is responsible to handle one input file) (argc = num files + 1)
    files_num = argc - 1;
    if (files_num <= 0) { // no files passed to handle
        fprintf(sderr, "Bank error: illegal arguments");
        return 1;
    }

    // create ATMs threads array for each file 
    pthread_t* atm_threads = new pthread_t[files_num];
    thread_data_t* atm_threads_data = new thread_data_t[files_num];
    
    // create commision thread 
    pthread_t commision_thread; 
    thread_data_t commision_thread_data;
    
    int i, rc;
    /* create threads */
    for (i = 0; i < files_num; ++i) {
        atm_threads_data[i]->thread_id = i;
        atm_threads_data[i]->file_name = argv[i+1]; // ask lior 
        atm_threads_data[i]->bank_account_pt = *bank_accounts; // check if * or &
        if ((rc = pthread_create(&atm_threads[i], NULL, ???routin func??? , atm_threads_data[i]))) { // ignore first argv - exe cmd // ask lior
            perror('Bank error: pthread_create failed');
            return 1;
        }
    }

    /* init commision_thread_data */
    commision_thread_data.thread_id = files_num; // last id
    commision_thread_data.file_name = "";
    commision_thread_data.bank_account_pt = *bank_accounts; // check if * or &

    /* create commision handler: (responsible to collects commision every 3 seconds) */
    if ((rc = pthread_create(&commision_thread, NULL, ???routin func commisiom??? , &commision_thread_data))) { 
            perror('Bank error: pthread_create failed');
            return 1;
    }

    /* block until all threads complete */
    for (int j = 0; j < files_num; ++j) {
        pthread_join(atm_threads[j], NULL);
    }
    
    pthread_join(commision_thread, NULL);
    return 0;
    
}