	#include <iostream>
	#include <fstream>
	#include <sstream>
	#include <stdlib.h>
	#include <stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
	if(argc < 3){
		cerr << "Please provide an input and output file." << endl;
		return 1;
	}
	ifstream input(argv[1]);
	ofstream output(argv[2]);

	int *numsubjects; //an array used to track number of sunjects in each experiment
	string **history;
	string line;
	string curr;

	bool started=false;
	int experiments;

	//for each experiment, you will need to keep track of the number of subjects; and for each subject, of their history

	//loop to read The Rani's logbook.
	if(!input.is_open()) cout << "Input File Not Found" << endl;
	while(getline(input, curr)) {

		line = curr;
		stringstream ss;
		ss << curr;
		ss >> curr;
		size_t found = line.find_first_of(".");
		if(found!=std::string::npos){
			output << "Error - incorrect command: " << line << endl;
			output << "Parameter should be an integer" << endl;
		} else if (curr == "START") {
			started=true;
			// Hint: This does not cover all errors yet.
			int n;
			experiments=1;
			numsubjects = new int[1];
			ss >> n;
			if (ss.fail()) {
				output << "Error - incorrect command: " << line << endl;
				output << "Parameter should be an integer" << endl;
			}
			else if (n < 0) {
				output << "Error - incorrect command: " << line << endl;
				output << "Number out of range" << endl;
			}
			else {
				history = new string*[experiments];
				history[0] = new string[n];
				numsubjects[0] = n;
			}
		}
		else if (!started){
			output << "Error - no subjects yet" << endl;
		}
		else if (curr == "ADD") {
			//copy history into temp so that another experiment column can be added to history
			string **temp = new string*[experiments+1];
			for(int i=0; i<experiments; i++){
				temp[i] = new string[numsubjects[i]];
				for(int j=0; j<numsubjects[i]; j++){
					temp[i][j] = history[i][j];
				}
				temp[experiments] = new string[0];
				delete [] history[i];
			}

			delete [] history;
			experiments++;
			history = new string*[experiments];
			for(int i=0; i<experiments; i++){
				history[i] = new string[numsubjects[i]];
				for(int j=0; j<numsubjects[i]; j++){
					history[i][j]=temp[i][j];
				}
			}
				//reallocate numsubjects with an extra spot
			int *tempSubjects = new int[experiments];
			for(int i=0; i<experiments-1; i++){
				tempSubjects[i]=numsubjects[i];
			}
			tempSubjects[experiments-1]=0;//when copied, the added experiment has 0 subjs
			delete [] numsubjects;
			numsubjects = new int[experiments]; 
			for(int i=0; i<experiments; i++){
				numsubjects[i]=tempSubjects[i];
			}
			for(int i=0; i<experiments; i++){
				delete [] temp[i];
			}
			delete [] temp;
			delete [] tempSubjects;
		}
		else if (curr == "MOVE") {
			bool failed=false;
			int x=-1,y=-1,a=-1,b=-1;
			stringstream tempSS;
			ss >> x;
			ss >> y;
			ss >> a;
			ss >> b;
			if(ss.fail()) failed=true;
			if(failed){
				output << "Error - incorrect command: " << line<<endl;
				if(x==-1 || y==-1 || a==-1 || b==-1){
					output << "Too few parameters" << endl;
				} else {
					output << "Parameter should be an integer" << endl;
				}
			} else if(b>numsubjects[x] || a<0 || x<0 || x>=experiments || y<0 || y>=experiments) {
				output << "Number out of range"<<endl;
			} else {
				
				tempSS <<y<< " ";
				string *tempOldExperiment = new string[numsubjects[x]-(b-a+1)];
				string *tempNewExperiment = new string[numsubjects[y]+(b-a+1)];
				for(int i=0; i<a; i++){
				tempOldExperiment[i]=history[x][i];
				}
				for(int i=0; i<numsubjects[y]; i++){
					tempNewExperiment[i]=history[y][i];
				}
				for(int i=a; i<=b; i++){
					tempNewExperiment[numsubjects[y]+i-a]= history[x][i]+tempSS.str(); 	
				}
				for(int i=b+1; i<numsubjects[x]; i++){
					tempOldExperiment[i-(b-a+1)]=history[x][i];
				}
				numsubjects[x]-=(b-a+1);
				numsubjects[y]+=(b-a+1);
				//reallocate new and old history

				delete [] history[x];
				history[x] = tempOldExperiment;
				delete [] history[y];
				history[y] = tempNewExperiment;
			}
		}
		else if (curr == "QUERY") {
			int x=-1, subNum=-1;
			ss >> x >> subNum;
			bool failed = false;
			if(ss.fail()) failed = true;
			if(failed){
				output << "Error - incorrect command: " << line;
				if (x==-1 || subNum==-1) {
					output << "Too few parameters" <<endl;
				} else {
					output << "Parameter should be an integer"<<endl;
				}
			} else if (x<0 || x>=experiments || subNum<0 || subNum>numsubjects[x]){
				output << "Number out of range" << endl;
			} else{
				output << (history[x][subNum]) << endl;
			}
		}
		else {
			output << "Command does not exist"<<endl;	
		}
	}

	for(int i=0; i<experiments; i++){
		delete [] history[i];
	}
	delete [] history;
	delete [] numsubjects;
	return 0;
}
