/*
Programming Assignment 3: 
TAS implementation
Roll no.: CO21BTECH1008

*/

#include <bits/stdc++.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <atomic>
#include <random>


using namespace std;

atomic<bool> flag(false);

int n,k,lambda1,lambda2;

//returns false if target is false and changes target to true and returns true if target is true
bool test_and_set(atomic<bool>& target) {
	bool expected = false;
	return !(target.compare_exchange_strong(expected, true));
}

void testCS(int id){
       
    	default_random_engine generator;
    
    	exponential_distribution<double> d1((1.0/lambda1));
    	exponential_distribution<double> d2((1.0/lambda2));
    	
	double t1 = d1(generator)*1e3;
	double t2 = d2(generator)*1e3;
	
	fstream output_tas;
  	
  	output_tas.open("output_tas.txt",ios::app);
  	if(!output_tas){
   		 cout << "File creation failed \n";
 	 }
	for (int i=0; i<k; i++){
	
		auto givemetime_request = chrono::system_clock::to_time_t(chrono::system_clock::now());
		output_tas << i+1 << "th CS Request" << " by thread " << id+1 << " at "<< ctime(&givemetime_request);
		
		//Entry Section
		while (test_and_set(flag))
			; /* do nothing */
		
		auto givemetime_entry = chrono::system_clock::to_time_t(chrono::system_clock::now());
		output_tas << i+1 << "th CS Entry" << " by thread " << id+1 <<" at "<< ctime(&givemetime_entry);
		
		sleep(t1); //Simulation of critical-section
		
		
		//Exit Section
		flag = false;

		auto givemetime_exit = chrono::system_clock::to_time_t(chrono::system_clock::now());
		output_tas << i+1 << "th CS Exit" << " by thread " << id+1 << " at "<< ctime(&givemetime_exit);
		
		sleep(t2); //Simulation of Reminder Section
    }
    output_tas.close();
}




int main(){
	
	fstream input;
        input.open("inp-params.txt", ios::in);

  	if (!input) {
    		cout << "File couldn't be opened\n";
 	 }
  	else{
  		//take input from input file n, k, λ1, λ2.
    		input >> n >> k >> lambda1 >> lambda2;
 	 }

	input.close();

	vector<thread> my_threads;
	
	for (int i=0; i<n ; i++){
		thread thd(testCS,i);
		my_threads.push_back(move(thd));
	}
	for(thread & th : my_threads){
		if(th.joinable()) th.join();
	}
	
	return 0;
}
