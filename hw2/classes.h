#ifndef _CLASSES_H
#define _CLASSES_H

#include <iostream>
#include <cstring>
#include <list>
using namespace std;

// create shared data structure to hold users accounts - when usind this structure please manage a "readers writers" routine.
// (MAYBE WE SOULD DO IT HERE?)
// ********************************* account class: ******************************************************
class account {
    private:
    int account_id; // an int
    int password; // can be assumed that the password is a 4 digits number that never starts with zero
    int balance; // always greater or equal to zero 

    public: // maybe to be protected by readers writers!!!!!!! ->
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

}

// ********************************* accounts class: ******************************************************
class accounts {
    private: 
    vector<account> accounts_vector; // vector of account objects 
    int num_accounts;
    int max_account_id;
    int bank_balance;

    public:
    // constructors
   accounts();

    // destructor
    ~accounts();

    // getters:
   int get_num_accounts();
   int get_max_account_id(); 

   //method functions:
    int insert_account(account new_account); // return 0 by success and -1 if account id already exists
    int remove_account(int account_id, int password); // return 0 by success (correct password) and -1 if object was not found
    void print_accounts();

}

















#endif
