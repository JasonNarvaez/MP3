/* 
    File: simpleclient.cpp

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2012/07/11

    Simple client main program for MP2 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <vector>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "reqchannel.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

double sDev(vector<double>v)
{
	double mean;
	for (int i = 0; i < v.size(); i++)
	{
		mean += v[i];
	}
	mean /= (double) v.size();
	
	double sd;
	for (int i = 0; i < v.size(); i++)
	{
		double dev = pow((v[i] - mean),2);
		sd += dev;
	}
	sd /= (double) v.size();
	return sqrt(sd);
}

double avg(vector<double>v)
{
	double mean;
	for (int i = 0; i < v.size(); i++)
	{
		mean += v[i];
	}
	return mean /= (double) v.size();
}

string local_send_request(const string & _request) {

  if (_request.compare(0, 5, "hello") == 0) {
    return "Hello back to you.";
  }
  else if (_request.compare(0, 4, "data") == 0) {
    return (int2string(rand() % 100));
  }
  else {
    return "What is this?";
  }

}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
  vector<double>sd;
  vector<double>sd1;
   pid_t child = fork();
  
 	if (child == 0)
	{
		execl("./dataserver", "", 0);
	}  

	else {
		cout << "CLIENT STARTED:" << endl;

		cout << "Establishing control channel... " << flush;
		RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
		cout << "done." << endl;

		/* -- Start sending a sequence of requests */

		timeval begin1, end1;
		gettimeofday(&begin1,NULL);
		string reply1 = chan.send_request("hello");
		gettimeofday(&end1,NULL);
		cout << "Reply to request 'hello' is '" << reply1 << "'" << endl;
		
		timeval begin2, end2; //function test
        gettimeofday(&begin2,NULL);
        string temp = local_send_request("hello");
        gettimeofday(&end2,NULL);
			
		timeval begin3, end3;
		gettimeofday(&begin3,NULL);
		string reply3 = chan.send_request("data Jane Smith");
		gettimeofday(&end3,NULL);
		cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << endl;

		for(int i = 0; i < 10000; i++)
		{
			timeval start, finish;
			(double)gettimeofday(&start, NULL);
			string request_string("data TestRandom" + int2string(i));
			string reply_string = local_send_request(request_string);
			cout << "reply to request " << i << ":" << reply_string << endl;;
			(double)gettimeofday(&finish, NULL);
			sd.push_back((double)(finish.tv_usec - start.tv_usec));
			
		/*	timeval start2, finish2;
			(double)gettimeofday(&start, NULL);
			string request_string1("data TestRandom" + int2string(i));
			string reply_string1 = chan.send_request(request_string1);
			cout << "reply to request " << i << ":" << reply_string << endl;;
			(double)gettimeofday(&finish, NULL);
			//sd1.push_back((double)(finish2.tv_usec - start2.tv_usec));*/
		}
			
		timeval begin4, end4;
		gettimeofday(&begin4,NULL);
		string reply4 = chan.send_request("quit");
		gettimeofday(&end4,NULL);
		cout << "Reply to request 'quit' is '" << reply4 << endl;
			
		//for (int i = 0; i < sd.size(); i++) {cout << "Process " << i+1  << ": " << sd[i] << "us" << endl;}
		//cout << (double)end1.tv_usec-begin1.tv_usec << " microseconds." << endl;

		cout << "Standard Deviation (function): " << sDev(sd) << " us" << endl;
		cout << "Average (function): " << avg(sd) << " us" << endl;
		cout << endl;
		//cout << "Standard Deviation (server): " << sDev(sd1) << " us" << endl;
		//cout << "Average (server): " << avg(sd1) << " us" << endl;
		
		usleep(100000);
		return 0;
	}
}

