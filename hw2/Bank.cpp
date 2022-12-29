// this is THE BANK! we hold accounts, and let users use ATMs to handle them.
// this is the main Bank function. this funtion gets N files to handle, and calls N treads (ATMs) to handle them.
#include <iostream>
#include <cstring>
#include <fstream>
#include <pthread.h>
#include "classes.h"
using namespace std;

/*globals*/
bool all_treads_finished;
/* init global bank accounts */
accounts bank_account;
fstream log_file;

/* handle_command function
gets a command line and thread data struct and executes command*/
void  handle_command(string curr_command, thread_data_t * data){
    /*ATM ID number*/
    int ATM_id = data->thread_id;

    /*create a copy of curr commend to erase by token*/
    string cpy_command = curr_command;

    /*initialize string array to save the commend*/
    int commend_arr[4];
    /*split commend by spaces*/
    string delimiter = " ";
    int i = 0;
    int start = 0;
    int end = cpy_command.find(delimiter);
    /*get letter command:*/
    string letter = cpy_command.substr(start, end - start);
    //
    start = end + delimiter.size();
    end = cpy_command.find(delimiter, start);
    /*get other numbers from input command*/
    while (end != -1) {
        try{ commend_arr[i] = stoi(cpy_command.substr(start, end - start));
        start = end + delimiter.size();
        end = cpy_command.find(delimiter, start);
        }
        catch(...){
            log_file << "Error "<< ATM_id << ": bad input ask lior" << endl;
			return;
        }
        i++;
    }
    try{commend_arr[i] = stoi(cpy_command.substr(start, end - start));} //last argument
    catch(...){
            log_file << "Error "<< ATM_id << ": bad input ask lior2" << endl;
			return;
    }
    //
    /*check if account exists - return error if not*/
    //if (!bank_account.account_exists(commend_arr[0]) && strcmp(letter.c_str(), "O") != 0) {
    //    log_file << "Error "<< ATM_id <<": Your transaction failed - account id "<< commend_arr[0] <<" does not exist" << endl;
    //    return;
    //}

    /*choose command*/
    if (!strcmp(letter.c_str(), "O")){
    /*open account: O <account> <password> <initial_amount> */
        int account_id = commend_arr[0];
        int password = commend_arr[1];
        int initial_amount = commend_arr[2];

        /*create new account */
        account new_account = account(account_id, password, initial_amount);

        /*try to insert account*/
        bank_account.insert_account(new_account,ATM_id)
    }
    else if (!strcmp(letter.c_str(), "D")){
    /*deposite to account: D <account> <password> <amount> */
        int account_id = commend_arr[0];
        int password = commend_arr[1];
        int amount = commend_arr[2];    
        /*call deposite */
        bank_account.deposite_amount(account_id,  password,  amount, ATM_id);
    }
    else if (!strcmp(letter.c_str(), "W")){
    /*withdraw from account: W <account> <password> <amount>*/
        int account_id = commend_arr[0];
        int password = commend_arr[1];
        int amount = commend_arr[2];    
        /*call  withdraw_amount */
        bank_account.withdraw_amount(account_id,  password,  amount, ATM_id);

    }
    else if (!strcmp(letter.c_str(), "B")){
    /*get balance B <account> <password>*/
        int account_id = commend_arr[0];
        int password = commend_arr[1];
        
        /*call check_balance and check return value */
        bank_account.check_balance(account_id, password, ATM_id);
    }
    else if (!strcmp(letter.c_str(), "Q")){
    /*remove account: Q <account> <password>*/
        int account_id = commend_arr[0];
        int password = commend_arr[1];
        
        /*call remove_account and check return value */
        bank_account.remove_account(account_id, password,ATM_id);
    }
    else if (!strcmp(letter.c_str(), "T")){
    /*transfer money to target account: T <account> <password> <target_account> <amount>*/
        int src_account_id = commend_arr[0];
        int password = commend_arr[1];
        int target_account_id = commend_arr[2];
        int amount = commend_arr[3];

        /*transfer amount to target account and check return value*/
        bank_account.transfer_amount(src_account_id, password, target_account_id, amount, ATM_id);
    }
    else{
        // illegal command - maybe print something <3 -------------------------------------------------------------------------------
        return;
    }
return;
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
            sleep(0.1);
        }
        ATMfile.close(); //close the file object.
    }
    else {
        fprintf(stderr, "Bank error: illegal arguments");
        exit(1);
    }
    pthread_exit(NULL);
}


/* commissions routine function
gets a pthread_data struct, and is responsible to collect commisions from each account*/
//void* commisions_routine(void* arg){
    /*cast input to thread data*/
//    thread_data_t *data= (thread_data_t *)arg;
    
    /*every 3 seconds, do:
    randomly select number between 1-5 %*/

    /* for each account - remove ((int)(account_balace * rand_num / 100)) and add to bank balance*/
    /* if (all_treads_finished) - global variable
    pthread_exit(NULL);*/
//return 0;
//}

/* print routine function
responsible for printing every half a second*/
//void* print_routine(void* arg){
    /*cast input to thread data*/
//    thread_data_t *data= (thread_data_t *)arg;
    
    /*every half second, do:
    print all numbers (it's ok to wait if accounts are blocked)*/

    /* if (all_treads_finished) - global variable
    pthread_exit(NULL);*/
//return 0;
//}


// main function:
// return 0 on success 1 on failure
int main(int argc, char *argv[])  // responsible for initializing threads and calling them
{
    /* open or create log txt file*/
    log_file.open("log.txt",ios::out);
    if(!log_file){
        fprintf(stderr, "Bank error: open failed"); /* ----------------------------------------------------------------check--------------------------------------------------------------------------*/
        exit(1);
    }

    /*initialize finish flag to false*/
    all_treads_finished = false;
    // ********** create Bank units using threads ***********
    // create ATMs: (each ATM is responsible to handle one input file) (argc = num files + 1)
    int files_num = argc - 1;
    if (files_num <= 0) { // no files passed to handle
        fprintf(stderr, "Bank error: illegal arguments");
        exit(1);
    }

    // create ATMs threads array for each file 
    pthread_t* atm_threads = new pthread_t[files_num];
    thread_data_t* atm_threads_data = new thread_data_t[files_num]; // ask lior about using new and initializing array with changing size
    
    // create commision thread 
    //pthread_t commision_thread; 
    //thread_data_t commision_thread_data;

    // create print thread 
    //pthread_t print_thread; 
    //thread_data_t print_thread_data;
    
    int i, rc;
    /* create threads */
    for (i = 0; i < files_num; ++i) {
        atm_threads_data[i].thread_id = i+1; // start from 1
        atm_threads_data[i].file_name = argv[i+1]; // ignore first argv - exe cmd // ask lior about [i] - dereference or pointer? 
        if ((rc = pthread_create(&atm_threads[i], NULL, ATM_routine , &atm_threads_data[i]))) {  // ask lior about &
            perror("Bank error: pthread_create failed");
            return 1;
        }
    }

    /* init commision_thread_data */
    //commision_thread_data.thread_id = files_num; // last id
    //commision_thread_data.file_name = ""; 
    
    /* create commision handler: (responsible to collects commision every 3 seconds) */
    //if ((rc = pthread_create(&commision_thread, NULL, ???routin func commisiom??? , &commision_thread_data))) { //ask lior &
    //        perror('Bank error: pthread_create failed');
    //        return 1;
    //}

    /* init print_thread_data */
    //print_thread_data.thread_id = files_num+1; // last id + 1
    //print_thread_data.file_name = ""; 

    /* create print handler: (responsible to collects commision every 3 seconds) */
    //if ((rc = pthread_create(&print_thread, NULL, ???routin func commisiom??? , &print_thread_data))) { //ask lior &
    //        perror('Bank error: pthread_create failed');
    //        return 1;
    //}

    /* block until all threads complete */
    for (int j = 0; j < files_num; ++j) {
        pthread_join(atm_threads[j], NULL);
    }
    
    /*set finish flag to true*/
    all_treads_finished = true;

    /*wait for print and commision threads to finish*/
    //pthread_join(print_thread, NULL);
    //pthread_join(commision_thread, NULL);

    /* close log txt file*/
    log_file.close();

    delete[] atm_threads;
    delete[] atm_threads_data;
    return 0;
    
}
