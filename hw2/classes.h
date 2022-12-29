#ifndef _CLASSES_H
#define _CLASSES_H

#include <iostream>
#include <cstring>
#include <vector>
#include <pthread.h>
#include <unistd.h>
using namespace std;

// create shared data structure to hold users accounts - when usind this structure please manage a "readers writers" routine.
// (MAYBE WE SOULD DO IT HERE?)


/*reader writers class*/
class readers_writers {
    private:
        pthread_mutex_t m;
        pthread_mutex_t writers;
        int readers;
    public:
        // constructor 
        readers_writers();

        // destructor
        ~readers_writers();
        
        // method functions
        void enter_reader();
        void enter_writer();
        void leave_reader();
        void leave_writer();
};


// ********************************* account class: ******************************************************
class account {
    private:
    int account_id; // an int
    int password; // can be assumed that the password is a 4 digits number that never starts with zero
    int balance; // always greater or equal to zero 
    

    public: // maybe to be protected by readers writers!!!!!!! ->
    readers_writers readers_writers_account;
    // constructors
   account(int account_id,int password,int balance);

    // destructor
    ~account();

    //getters:
    int get_account_id();
    int get_password();
    int get_balance();
    
    //setters:
    void set_account_id(int new_account_id);
    void set_password(int new_password);
    void set_balance(int new_balance);

};

// ********************************* accounts class: ******************************************************
class accounts {
    private: 
    vector<account> accounts_vector; // vector of account objects 
    int num_accounts;
    int max_account_id;
    int bank_balance;

    public:
    readers_writers readers_writers_bank_accounts;
    // constructors
   accounts();

    // destructor
    ~accounts();

    // getters:
    int get_num_accounts();
    int get_max_account_id(); 
    int get_bank_balance(); 
   //method functions:
    int insert_account(account new_account); // return 0 by success and -1 if account id already exists
    int remove_account(int account_id, int password); // return 0 by success (correct password) and -1 if object was not found
    void print_accounts();
    bool account_exists(int account_id); // get a boolian = true if account is in list, and false if not
    int deposite_amount(int account_id, int password, int amount); // return balace by success (correct password)  -1 if password not correct
    int transfer_amount(int src_account_id, int password, int target_account_id, int amount, int ATM_id); // return new balace by success
    int withdraw_amount(int account_id, int password, int amount); // subtract amount to account, if enough balance and correct password return balace , if not enough balance return -2 , if password not correct return -1
    int check_balance(int account_id, int password); // return account balance by success (correct password)  -1 if password not correct
    //void take_commission();

};

/* create thread argument struct for thr_func() */
typedef struct _thread_data_t {
    int thread_id;
    string file_name;
} thread_data_t;

#endif
