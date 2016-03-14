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
#include <vector>

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
	
	
	string temp;
	//string temp= "cat /proc/"+PID+"/maps";//output maps 
	//system(temp.c_str());
	
	cout<<"1)"<<endl;
	temp= "grep -i pid /proc/"+PID+"/status";//output PID
	system(temp.c_str());
	
	//need to output EUID, EGID, RUID, RGID, FSUID, FSGID
	cout<<endl<<"RUID, EUID, SSUID, FSUID"<<endl;
	temp= "grep -i UID /proc/"+PID+"/status";
	system(temp.c_str());
	
	cout<<endl<<"RGID, EGID, SSGID, FSGID"<<endl;
	temp= "grep Gid /proc/"+PID+"/status";
	system(temp.c_str());
	
	cout<<endl<<"2)"<<endl;
	temp= "grep -i state /proc/"+PID+"/status";	//output state	
	system(temp.c_str());
	
	cout<<endl<<"3)"<<endl;
	temp= "grep -i thread /proc/"+PID+"/status";	//output thread info	
	system(temp.c_str());
	
	//cout<<endl<<"NOTE: start counting at [1], not [0]!"<<endl;
	//cout<<"[14]: utime, [15]: stime, [16]: cutime, [17]: cstime, [18]: Priority number, [19]: Nice value"<<endl;
	//temp= "cat /proc/"+PID+"/stat";//priority and nice value
	
	//cout<<endl<<"[26]: startcode, [27]: endcode, [29]: ESP, [30]: EIP"<<endl;
	
	
	cout<<endl;
	cout<<"4) & 5) & 6)"<<endl;
	temp = "/proc/"+PID+"/stat";
	ifstream ifs(temp.c_str());
	vector<string> data;//stat file has 44 things
	string extract;
	int i = 0;
	
	while(ifs.good()){
		ifs>>extract;
		
		if(i == 13 || i == 14 || i == 15 || i == 16 || i == 17 || i == 18  || i == 25 || i == 26 || i == 28 || i == 29){
			data.push_back(extract);
		}
		i++;
	}
	for(int i=0;i<data.size();i++){
		switch(i){
			case 0:
				cout<<"utime: "<<data[i]<<endl;
				break;
			case 1:
				cout<<"stime: "<<data[i]<<endl;
				break;
			case 2:
				cout<<"cutime: "<<data[i]<<endl;
				break;
			case 3:
				cout<<"cstime: "<<data[i]<<endl;
				break;
			case 4:
				cout<<"Priority number: "<<data[i]<<endl;
				break;
			case 5:
				cout<<"Nice number: "<<data[i]<<endl;
				break;
			case 6:
				cout<<"startcode: "<<data[i]<<endl;
				break;
			case 7:
				cout<<"endcode: "<<data[i]<<endl;
				break;
			case 8:
				cout<<"ESP: "<<data[i]<<endl;
				break;
			case 9:
				cout<<"EIP: "<<data[i]<<endl;
				break;
		}
	}
	
	cout<<endl<<"7)"<<endl;
	cout<<"File handle:"<<endl;
	temp= "grep -i FDSize /proc/"+PID+"/status";	//output file handle	
	system(temp.c_str());
	
	cout<<endl<<"Context switches:"<<endl;
	temp= "grep -i switches /proc/"+PID+"/status";	//output context switches	
	system(temp.c_str());
	
	cout<<endl<<"8)"<<endl;
	cout<<"Processors:"<<endl;
	temp= "grep -i Cpus_allowed /proc/"+PID+"/status";	//output Processors	
	system(temp.c_str());
	cout<<endl;
	temp= "egrep 'heap|stack|vdso|vsyscall' /proc/"+PID+"/maps > mapsinfo.txt";//output maps to a file
	system(temp.c_str());
	
	cout<<"9)"<<endl;
	temp = "mapsinfo.txt";
	ifstream ifs2(temp.c_str());
	vector<string> mapinfo;//stat file has 44 things
	string mapdata;
	i = 0;
	
	while(ifs2.good()){
		ifs2>>mapdata;
		if(!ifs2.good()){break;}
		switch(i){
			case 0:
				cout<<"address range: "<<mapdata<<endl;
				break;
			case 1:
				cout<<"permissions: "<<mapdata<<endl;
				break;
			case 2:
				cout<<"offset: "<<mapdata<<endl;
				break;
			case 3:
				cout<<"dev: "<<mapdata<<endl;
				break;
			case 4:
				cout<<"inode: "<<mapdata<<endl;
				break;
			case 5:
				cout<<"path name: "<<mapdata<<endl<<endl;
				i = -1;
				break;
			
		}
		
		i++;
	}
	
	//context switch is in status, last two (voluntary and involuntary)
	//1 2 3 4 5 7
}