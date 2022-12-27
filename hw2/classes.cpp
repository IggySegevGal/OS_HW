#include "classes.h"


// ---------------------- account class functions: -----------------------------------
	//constructors
	account::account(int account_id,int password,int balance) { 
		this->account_id = account_id;
		this->password = password;
		this->balance = balance;
        this->readers_writers_account.init();
	}

    // destructor
    account::~account() {}


    //getters:
    int account::get_account_id(){
        readers_writers_account.enter_reader();
        int rt = account_id;
        readers_writers_account.leave_reader();
        return rt;
    }
    int account::get_password(){
        readers_writers_account.enter_reader();
        int rt = password;
        readers_writers_account.leave_reader();
        return rt;
    }
    int account::get_balance(){
        readers_writers_account.enter_reader();
        int rt = balance;
        readers_writers_account.leave_reader();
        return rt;
    }
    //setters:
    void account::set_account_id(int new_account_id){
        readers_writers_account.enter_writer();
        this->account_id = new_account_id;
        readers_writers_account.leave_writer();
    }
    void account::set_password(int new_password){
        readers_writers_account.enter_writer();
        this->password = new_password;
        readers_writers_account.leave_writer();
    }
    void account::set_balance(int new_balance){
        readers_writers_account.enter_writer();
        this->balance = new_balance;
        readers_writers_account.leave_writer();
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

        // if id is bigger than current max id - add it to the end of the list and update max id:
		if(max_account_id < new_account.get_account_id()){
			accounts_vector.push_back(new_account);
			max_account_id = new_account.get_account_id();
		}
		else {
            
            // first check if account id already exists in vector
            vector<account>::iterator it;
			for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
				if(new_account.get_account_id() == it->get_account_id()){
					return -1;
				}
			}

            // if account id does not exist, and id is not bigger than max - add id where it belongs in the list:
			for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
				if(new_account.get_account_id() < it->get_account_id()){
					accounts_vector.insert(it,new_account);
					break;
				}
			}
		}

		num_accounts = accounts_vector.size(); // update list size
    	return 0;

    }

    int accounts::remove_account(int account_id, int password) { // return 0 by success and -2 if object was not found or -1 if password was not correct
      vector<account>::iterator it;
      int return_balance = -2;
      for (int i=0 ; i < num_accounts; i++){
          if (accounts_vector[i].get_account_id() == account_id){
            
            // check if password is correct - if not return -1
            if (accounts_vector[i].get_password() != password) {
                return -1;
            }
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
	        return return_balance;
          }
      }
      return return_balance;
    }

    void accounts::print_accounts(){
      //print first line:
      cout << "Current Bank Status" << endl;
      vector<account>::iterator it;
      // print loop
      for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
         //print:
        cout << "Account " << it->get_account_id(); // account id
        cout << ": Balance - " << it->get_balance() << " $, "; // balance
        cout << "Account Password - " << it->get_password() << endl; // password
      }
      
      //print first line:
      cout << "The Bank has " << bank_balance << " $" << endl;
    }

    bool accounts::account_exists(int account_id){ // get a boolian = true if account is in list, and false if not
        vector<account>::iterator it;
        bool exists = false;
        for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
            if (it->get_account_id() == account_id){
                exists = true;
            }
        }
        return exists;
    }


    int accounts::deposite_amount(int account_id, int password, int amount){ // return balace by success (correct password)  -1 if password not correct
        // find account:
        vector<account>::iterator it;
        for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
            if(account_id == it->get_account_id()){
                if (it->get_password() != password){
                    return -1;
                }
                int curr_balance = it->get_balance();
                it->set_balance(curr_balance + amount);
                return curr_balance + amount;
            }
        }
        return -1; // maybe return -2 (account not found)

    }

    int accounts::transfer_amount(int account_id, int amount){ // return balace or -1 if account not found
        // find account:
        vector<account>::iterator it;
        for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
            if(account_id == it->get_account_id()){
                int curr_balance = it->get_balance();
                it->set_balance(curr_balance + amount);
                return curr_balance + amount;
            }
        }
        return -1; // maybe return -2 (account not found)

    }

    int accounts::withdraw_amount(int account_id, int password, int amount){ // subtract amount to account, if enough balance and correct password return balace , if not enough balance return -2 , if password not correct return -1
        // find account:
        vector<account>::iterator it;
        for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
            if(account_id == it->get_account_id()){
                if (it->get_password() != password){
                    return -1;
                }
                int curr_balance = it->get_balance();
                /* check if there is enough balance - if not, return 1*/
                if (amount > curr_balance) {
                    return -2;
                }
                it->set_balance(curr_balance - amount);
                return curr_balance - amount;
            }
        }
        return -1; // maybe return -2 (account not found)
    }
    int accounts::get_balance(int account_id, int password){ // return account balance by success (correct password)  -1 if password not correct
        // find account:
        vector<account>::iterator it;
        for (it = accounts_vector.begin() ; it != accounts_vector.end(); ++it){
            if(account_id == it->get_account_id()){
                if (it->get_password() != password){
                    return -1;
                }
                int curr_balance = it->get_balance();
                return curr_balance;
            }
        }
        return -1; // maybe return -2 (account not found)
    }
