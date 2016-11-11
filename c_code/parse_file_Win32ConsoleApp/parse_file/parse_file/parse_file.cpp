// parse_file.cpp : Defines the entry point for the console application.
//
// http://cs.dvc.edu/HowTo_Cparse.html ("c++ parse file")

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

bool readEncodingProfiles() {
	ifstream fin;
	fin.open("F:\\dev\\general\\c_code\\parse_file_Win32ConsoleApp\\parse_file\\parse_file\\VitecEncodingProfiles.dat");
	
	if (!fin.good()) 
		return false; // exit if file not found

	// read each line from the encoding-profiles file
	string line;
	while (getline (fin, line)){
		// ignore empty lines
		if (line.length() == 0){
			continue;
		}

		// get each token (delimited by commas) from each line
		istringstream iss(line);
		string token;
		while (getline(iss, token, ',')){
			// skip leading white space
			iss >> ws;
			cout << token << endl;
		}
		cout << endl;
	}

	fin.close();
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool result = readEncodingProfiles();

	system("pause");
	return 0;
}

