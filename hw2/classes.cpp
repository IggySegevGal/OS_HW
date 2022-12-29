#include "classes.h"


// ---------------------- account class functions: -----------------------------------
	//constructors
	account::account(int account_id,int password,int balance) { 
		this->account_id = account_id;
		this->password = password;
		this->balance = balance;
	}

    // destructor
    account::~account() {}


    //getters:
    int account::get_account_id(){
        //readers_writers_account.enter_reader();
        int rt = account_id;
        //readers_writers_account.leave_reader();
        return rt;
    }
    int account::get_password(){
        //readers_writers_account.enter_reader();
        int rt = password;
        //readers_writers_account.leave_reader();
        return rt;
    }
    int account::get_balance(){
        //readers_writers_account.enter_reader();
        int rt = balance;
        //readers_writers_account.leave_reader();
        return rt;
    }
    //setters:
    void account::set_account_id(int new_account_id){
        //readers_writers_account.enter_writer();
        this->account_id = new_account_id;
        //readers_writers_account.leave_writer();
    }
    void account::set_password(int new_password){
        //readers_writers_account.enter_writer();
        this->password = new_password;
        //readers_writers_account.leave_writer();
    }
    void account::set_balance(int new_balance){
        //readers_writers_account.enter_writer();
        this->balance = new_balance;
        //readers_writers_account.leave_writer();
    }
// ---------------------- accounts class functions: -----------------------------------

	//constructors
	accounts::accounts() { 
        num_accounts = 0;
        max_account_id = 0;
        bank_balance = 0;
	}

    // destructor
    accounts::~accounts() {}

    // getters:
   int accounts::get_num_accounts(){
        return num_accounts;
   }
   int accounts::get_max_account_id(){
        return max_account_id;
   }
    int accounts::get_bank_balance(){
        return bank_balance;
   }
   // **************** method functions: **************************
   	
    // insert a new account to class - insert from the back of the vector to maintain account_id order
    int accounts::insert_account(account new_account){ // return 0 by success and -1 if account id already exists
/*----------ask lior--------*/
        readers_writers_bank_accounts.enter_writer();
        // if id is bigger than current max id - add it to the end of the list and update max id:
		if(max_account_id < new_account.get_account_id()){
			accounts_vector.push_back(new_account);
			max_account_id = new_account.get_account_id();
		}
		else {
            
            // first check if account id already exists in vector
            vector<account>::iterator it;
			for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
                // account already exists - error and return:
				if(new_account.get_account_id() == it->get_account_id()){
                    sleep(1);
                    readers_writers_bank_accounts.leave_writer();
					return -1;
				}
			}

            /*success:*/
            // if account id does not exist, and id is not bigger than max - add id where it belongs in the list:
			for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
				if(new_account.get_account_id() < it->get_account_id()){
					accounts_vector.insert(it,new_account);
					break;
				}
			}
		}

		num_accounts = accounts_vector.size(); // update list size
        sleep(1);
        readers_writers_bank_accounts.leave_writer();
    	return 0;

    }

    int accounts::remove_account(int account_id, int password) { // return 0 by success and -2 if object was not found or -1 if password was not correct
    /*-------ask lior------*/
      readers_writers_bank_accounts.enter_writer();
      vector<account>::iterator it;
      int return_balance = -2;
      for (int i=0 ; i < num_accounts; i++){
          if (accounts_vector[i].get_account_id() == account_id){
            
            // check if password is correct - if not return -1
            if (accounts_vector[i].get_password() != password) {
                sleep(1);
                readers_writers_bank_accounts.leave_writer();
                return -1;
            }
            
            /*success:*/
            return_balance = accounts_vector[i].get_balance();
            accounts_vector.erase(accounts_vector.begin()+i);
			num_accounts = accounts_vector.size(); // update list size
            
            // update max account id if needed (removed old max)
			if(account_id == max_account_id ){ 
				if (num_accounts == 0) {
                    max_account_id = 0;
                }
				else {
				    max_account_id = accounts_vector.back().get_account_id();
				}
			 }
             sleep(1);
             readers_writers_bank_accounts.leave_writer();
	        return return_balance;
          }
      }
      /*account not found - return:*/
      readers_writers_bank_accounts.leave_writer();
      sleep(1);
      return return_balance;
    }

    void accounts::print_accounts(){ /*wait for forum and maybe changeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee*/
    vector<account>::iterator it;
      readers_writers_bank_accounts.enter_reader(); // this func doesn't change accounts members - only reads
      // this is here to make sure the print is updated to last state and no changes while printing
      /*enter all accounts as reader*/
      for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
        it->readers_writers_account.enter_reader();
      }
      //print first line:
      cout << "Current Bank Status" << endl;
      // print loop
      for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
         //print:
        cout << "Account " << it->get_account_id(); // account id
        cout << ": Balance - " << it->get_balance() << " $, "; // balance
        cout << "Account Password - " << it->get_password() << endl; // password
      }
      
      //print last line:
      cout << "The Bank has " << bank_balance << " $" << endl;

      /*leave all accounts as reader*/
        for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
        it->readers_writers_account.leave_reader();
      }
      //sleep(0.5); // check if here or inside print thread
      readers_writers_bank_accounts.leave_reader();
      return;
    }

    bool accounts::account_exists(int account_id){ // get a boolian = true if account is in list, and false if not
        vector<account>::iterator it;
        bool exists = false;
        readers_writers_bank_accounts.enter_reader(); // this func doesn't change accounts members - only reads
        for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
            if (it->get_account_id() == account_id){ // no account protection because account_id can't be changed
                exists = true;
            }
        }
        readers_writers_bank_accounts.leave_reader();
        return exists;
    }


    int accounts::deposite_amount(int account_id, int password, int amount){ // return balace by success (correct password)  -1 if password not correct
        // find account:
        vector<account>::iterator it;
        readers_writers_bank_accounts.enter_reader(); // this func doesn't change accounts members - only reads
        for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
            if(account_id == it->get_account_id()){
                if (it->get_password() != password){
                    /*incorrect password - return;*/
                    sleep(1);
                    readers_writers_bank_accounts.leave_reader();
                    return -1;
                }

                /*success:*/
                it->readers_writers_account.enter_writer();
                int curr_balance = it->get_balance();
                it->set_balance(curr_balance + amount);
                sleep(1);
                it->readers_writers_account.leave_writer();
                readers_writers_bank_accounts.leave_reader();
                return curr_balance + amount;
            }
        }
         /*account not found - return;*/
        sleep(1);
        readers_writers_bank_accounts.leave_reader();
        return -1; // maybe return -2 (account not found)

    }

    int accounts::transfer_amount(int account_id, int amount){ // return balace or -1 if account not found
        // find account:
        vector<account>::iterator it;
        readers_writers_bank_accounts.enter_reader(); // this func doesn't change bank accounts members - only reads
        for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
            if(account_id == it->get_account_id()){// account_id doesn't change
                /*success:*/
                it->readers_writers_account.enter_writer();
                int curr_balance = it->get_balance();
                it->set_balance(curr_balance + amount);
                sleep(1);
                it->readers_writers_account.leave_writer();
                readers_writers_bank_accounts.leave_reader();
                return curr_balance + amount;
            }
        }
        
        /*account not found - return;*/
        sleep(1);
        readers_writers_bank_accounts.leave_reader();
        return -1; // maybe return -2 (account not found)
    }

    int accounts::withdraw_amount(int account_id, int password, int amount){ // subtract amount to account, if enough balance and correct password return balace , if not enough balance return -2 , if password not correct return -1
        // find account:
        vector<account>::iterator it;
        readers_writers_bank_accounts.enter_reader(); // this func doesn't change bank accounts members - only reads
        for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
            if(account_id == it->get_account_id()){ 
                if (it->get_password() != password){ 
                    /*incorrect password - return;*/
                    sleep(1);
                    readers_writers_bank_accounts.leave_reader();
                    return -1;
                }
                it->readers_writers_account.enter_writer(); // enter writer to account
                int curr_balance = it->get_balance();
                /* check if there is enough balance - if not, return 1*/
                if (amount > curr_balance) {
                    sleep(1);
                    it->readers_writers_account.leave_writer(); // leave writer from account
                    readers_writers_bank_accounts.leave_reader(); 
                    return -2;
                }

                /*success:*/
                it->set_balance(curr_balance - amount);
                sleep(1);
                it->readers_writers_account.leave_writer(); // leave writer from account
                readers_writers_bank_accounts.leave_reader();
                return curr_balance - amount;
            }
        }
        /*account not found - return;*/
        sleep(1);
        readers_writers_bank_accounts.leave_reader();
        return -1; // maybe return -2 (account not found)
    }
    int accounts::check_balance(int account_id, int password){ // return account balance by success (correct password)  -1 if password not correct
        // find account:
        vector<account>::iterator it;
        readers_writers_bank_accounts.enter_reader(); // this func doesn't change bank accounts members - only reads
        for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
            if(account_id == it->get_account_id()){
                // wrong password - return 
                if (it->get_password() != password){
                    sleep(1);
                    readers_writers_bank_accounts.leave_reader();
                    return -1;
                }
                /*success:*/
                it->readers_writers_account.enter_reader(); // enter reader to account
                int curr_balance = it->get_balance();
                sleep(1);
                it->readers_writers_account.leave_reader(); // leave reader to account
                readers_writers_bank_accounts.leave_reader();
                return curr_balance;
            }
        }
        // account not found: (not supposed to get here)
        sleep(1);
        readers_writers_bank_accounts.leave_reader();
        return -1; // maybe return -2 (account not found)
    }


// ---------------------- readers writers class functions: -----------------------------------
        // constructor
        readers_writers::readers_writers() {
            readers = 0;
            pthread_mutex_init(&m,NULL);
            pthread_mutex_init(&writers,NULL); /********************************check init value*************************************/
        }
        // destructor
        readers_writers::~readers_writers() {
            pthread_mutex_destroy(&m);
            pthread_mutex_destroy(&writers);
        }
        void readers_writers::enter_reader() {
            pthread_mutex_lock(&m);
            readers++;
            if(readers == 1){
                pthread_mutex_lock(&writers);
            }    
            pthread_mutex_unlock(&m);
            return;
        }
        void readers_writers::enter_writer() {
            pthread_mutex_lock(&writers);
            return;
        }
        void readers_writers::leave_reader() {
            pthread_mutex_lock(&m);
            readers --;
            if(readers == 0){
                pthread_mutex_unlock(&writers);
            }
            pthread_mutex_unlock(&m);
            return;
        }
        void readers_writers::leave_writer() {
            pthread_mutex_unlock(&writers);
            return;
        }