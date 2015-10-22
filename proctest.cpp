#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <iomanip>
#include <fstream>

#include <errno.h>
#include <unistd.h>
#include <ctime>
using namespace std;
string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int main(int argc, char ** argv) {

	unsigned long long pid = 0;
	int opt;
	//char input[256];
	while((opt = getopt(argc, argv, "p::")) != -1){
		
		switch (opt){
			case 'p':{
				if(argv[2]) pid = atoi(argv[2]);
				else pid=0;
				//printf("b: %s \n",argv[2]);
				break;
			}
			
			default:{
				fprintf(stderr, "Usage: %s [-p pid]\n",argv[0]);
				exit(EXIT_FAILURE);
			}
		}
		
	}
	
	printf("using values:\np: %d \n",pid);
	string PID = int2string(pid);
	
	//system("");
	//system();
	
	
	
	string temp= "cat /proc/"+PID+"/maps";//output maps 
	system(temp.c_str());
	
	
	temp= "grep -i 'pid' /proc/"+PID+"/status";//output PID
	system(temp.c_str());
	
	//need to output EUID, EGID, RUID, RGID, FSUID, FSGID
	cout<<"RUID, EUID, SSUID, FSUID"<<endl;
	temp= "grep -i 'UID' /proc/"+PID+"/status";
	system(temp.c_str());
	
	cout<<"RGID, EGID, SSGID, FSGID"<<endl;
	temp= "grep 'Gid' /proc/"+PID+"/status";
	system(temp.c_str());
	
	temp= "grep -i 'state' /proc/"+PID+"/status";	//output state	
	system(temp.c_str());
	
	temp= "grep -i 'thread' /proc/"+PID+"/status";	//output thread info	
	system(temp.c_str());
	
	cout<<endl<<"NOTE: start counting at [1], not [0]!"<<endl;
	cout<<"[14]: utime, [15]: stime, [16]: cutime, [17]: cstime, [18]: Priority number, [19]: Nice value"<<endl;
	temp= "cat /proc/"+PID+"/stat";//priority and nice value
	system(temp.c_str());
	
	cout<<endl<<"[26]: startcode, [27]: endcode, [29]: ESP, [30]: EIP"<<endl;
	system(temp.c_str());
	
	cout<<endl<<"File handle:"<<endl;
	temp= "grep -i 'FDSize' /proc/"+PID+"/status";	//output file handle	
	system(temp.c_str());
	
	cout<<endl<<"Context switches:"<<endl;
	temp= "grep -i 'switches' /proc/"+PID+"/status";	//output context switches	
	system(temp.c_str());
	
	cout<<endl<<"Processors:"<<endl;
	temp= "grep -i 'Cpus_allowed' /proc/"+PID+"/status";	//output Processors	
	system(temp.c_str());
	// ifstream ifs(temp.c_str());
	// while(good){
		
		
	// }
	//context switch is in status, last two (voluntary and involuntary)
	//1 2 3 4 5 7
}
