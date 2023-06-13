/*
Programming Assignment 3: 
CAS implementation
Roll no.: CO21BTECH1008

*/

#include<bits/stdc++.h>
#include<fstream>
#include<thread>
#include <chrono>
#include <ctime>
#include<atomic>

using namespace std;

atomic<int> flag(0);

int n,k,lambda1,lambda2;

bool compare_and_swap(atomic<int>& target, int expected, int desired) {
	return !(target.compare_exchange_strong(expected, desired));
}

void testCS(int id){
	default_random_engine generator;
    
    	exponential_distribution<double> d1((1.0/lambda1));
    	exponential_distribution<double> d2((1.0/lambda2));
    	
	double t1 = d1(generator)*1e3;
	double t2 = d2(generator)*1e3;
	fstream output_cas;
  	
  	output_cas.open("output_cas.txt",ios::app);
  	if(!output_cas){
   		 cout << "File creation failed \n";
 	 }
	for (int i=0; i<k; i++){
	
		auto givemetime_request = chrono::system_clock::to_time_t(chrono::system_clock::now());
		output_cas << i+1 << "th CS Request" << " by thread " << id+1 << " at "<< ctime(&givemetime_request);
		
		//Entry Section
		while (compare_and_swap(flag,0,1)!=0)
			; /* do nothing */
		
		auto givemetime_entry = chrono::system_clock::to_time_t(chrono::system_clock::now());
		output_cas << i+1 << "th CS Entry" << " by thread " << id+1 <<" at "<< ctime(&givemetime_entry);
		
		sleep(t1); //Simulation of critical-section
		
		
		//Exit Section
		flag = 0;

		auto givemetime_exit = chrono::system_clock::to_time_t(chrono::system_clock::now());
		output_cas << i+1 << "th CS Exit" << " by thread " << id+1 << " at "<< ctime(&givemetime_exit);
		
		sleep(t2); //Simulation of Reminder Section
    }
    output_cas.close();
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
