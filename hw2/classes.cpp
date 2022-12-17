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
        //mutex
        return account_id;
    }
    int account::get_password(){
        return password;
    }
    int account::get_balance(){
        return balance;
    }
    //setters:
    void account::set_account_id(int new_account_id){
        this->account_id = new_account_id;
    }
    void account::set_password(int new_password){
        this->password = new_password;
    }
    void account::set_balance(int new_balance){
        this->balance = new_balance;
    }
// ---------------------- accounts class functions: -----------------------------------

	//constructors
	accounts::accounts() { 
    int num_accounts = 0;
    int max_account_id = 0;
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

   // **************** method functions: **************************
   	
    // insert a new account to class - insert from the back of the vector to maintain account_id order
    int insert_account(account new_account){ // return 0 by success and -1 if account id already exists

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

    int remove_account(int account_id, int password) { // return 0 by success and -1 if object was not found or 1 if password was not correct

      vector<account>::iterator it;
      int account_removed = -1;
      for (int i=0 ; i < num_accounts; i++){
          if (accounts_vector[i].get_account_id() == account_id){
            
            // check if password is correct - if not return 1
            if (accounts_vector[i].get_password() != password) {
                return 1;
            }

            accounts_vector.erase(accounts_vector.begin()+i);
            account_removed = 0;
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
	        return account_removed;
          }
      }
      return account_removed;

    }

    void print_accounts(){

    }



    


