#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>

using namespace std;

int main (int argc, char* argv[]){
	cout << "start" << endl;
	string fileName;
	int len;
	char *backwards;
	ifstream toBackwards;
	toBackwards.open(argv[1]);
	toBackwards >> len;
	cout << len << endl;
	backwards=new char[len];
	char c;
	for(int i=0; i<=len; i++) {
		toBackwards.get(c);
		backwards[i]=c;
	}
	for(int i=len-1; i>=0; i--) cout << backwards[i];
	delete [] backwards;
}
