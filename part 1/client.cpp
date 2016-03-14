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
#include <fstream>

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
	mean /= v.size();
	
	double sd;
	for (int i = 0; i < v.size(); i++)
	{
		double dev = pow((v[i] - mean),2);
		sd += dev;
	}
	sd /= v.size();
	return sqrt(sd);
}

double avg(vector<double>v)
{
	double sum;
	for (int i = 0; i < v.size(); i++)
	{
		sum += v[i];
	}
	double mean = sum/v.size();
	return mean;
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

		//====================SERVER TEST=======================
		
		timeval begin1, end1;
		gettimeofday(&begin1,NULL);
		string reply1 = chan.send_request("hello");
		gettimeofday(&end1,NULL);
		cout << "Reply to request 'hello' is '" << reply1 << "'" << endl;
		cout << "Server Delay: " << (double)(end1.tv_usec - begin1.tv_usec) << " us" << endl;
		
		timeval begin2, end2; //function test
        gettimeofday(&begin2,NULL);
        string reply2 = chan.send_request("data Joe Smith");
        gettimeofday(&end2,NULL);
		cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << endl;
		cout << "Server Delay: " << (double)(end2.tv_usec - begin2.tv_usec) << " us" << endl;
		
		timeval begin3, end3;
		gettimeofday(&begin3,NULL);
		string reply3 = chan.send_request("data Jane Smith");
		gettimeofday(&end3,NULL);
		cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << endl;
		cout << "Server Delay: " << (double)(end3.tv_usec - begin3.tv_usec) << " us" << endl;
		
		cout << endl;
		
			//===============LOCAL FUNCTION TEST==============
			
		timeval begin11, end11;
		gettimeofday(&begin11,NULL);
		string reply11 = local_send_request("hello");
		gettimeofday(&end11,NULL);
		cout << "Reply to request 'hello' is '" << reply11 << "'" << endl;
		cout << "Process Delay: " << (double)(end11.tv_usec - begin11.tv_usec) << " us" << endl;
		
		timeval begin21, end21; //function test
        gettimeofday(&begin21,NULL);
        string reply21 = local_send_request("data Joe Smith");
        gettimeofday(&end21,NULL);
		cout << "Reply to request 'data Joe Smith' is '" << reply21 << "'" << endl;
		cout << "Process Delay: " << (double)(end21.tv_usec - begin21.tv_usec) << " us" << endl;
		
		timeval begin31, end31;
		gettimeofday(&begin31,NULL);
		string reply31 = local_send_request("data Jane Smith");
		gettimeofday(&end31,NULL);
		cout << "Reply to request 'data Jane Smith' is '" << reply31 << "'" << endl;
		cout << "Process Delay: " << (double)(end31.tv_usec - begin31.tv_usec) << " us" << endl;
		
		cout << endl;
		
		for(int i = 0; i < 10000; i++)
		{
			timeval start, finish;
			(double)gettimeofday(&start, NULL);
			string request_string("data TestRandom" + int2string(i));
			string reply_string = chan.send_request(request_string);
			cout << "reply to request " << i << ":" << reply_string << endl;;
			(double)gettimeofday(&finish, NULL);	//server delay
			sd.push_back((double)(finish.tv_usec - start.tv_usec));
			
			timeval start2, finish2;
			(double)gettimeofday(&start2, NULL);
			string request_string1("data TestRandom" + int2string(i));
			string reply_string1 = local_send_request(request_string1);
			cout << "reply to request " << i << ":" << reply_string << endl;;
			(double)gettimeofday(&finish2, NULL);
			sd1.push_back((double)(finish2.tv_usec - start2.tv_usec));
		}
			
		timeval begin4, end4;
		gettimeofday(&begin4,NULL);
		string reply4 = chan.send_request("quit");
		gettimeofday(&end4,NULL);
		cout << "Reply to request 'quit' is '" << reply4 << endl;

		cout << "Standard Deviation (server): " << sDev(sd) << " us" << endl;
		cout << "Average (server): " << avg(sd) << " us" << endl;
		cout << endl;
		cout << "Standard Deviation (function): " << sDev(sd1) << " us" << endl;
		cout << "Average (function): " << avg(sd1) << " us" << endl;
		
		
		usleep(100000);
		return 0;
	}
}

