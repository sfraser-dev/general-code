// parse_file.cpp : Defines the entry point for the console application.
//
// http://cs.dvc.edu/HowTo_Cparse.html ("c++ parse file")

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <atlstr.h>
using namespace std;

typedef struct _LWProfileDef {
	CString name;		
	CString videoEncoding;
	CString audioEncoding;
} LWProfileDef_t;

bool readEncodingProfiles() {
	ifstream fin;
	LWProfileDef_t arrProfiles[20];
	string arrTokens[3];

	// open the encoding-profiles file
	fin.open("F:\\dev\\general\\c_code\\parse_file_Win32ConsoleApp\\parse_file\\parse_file\\VitecEncodingProfiles.dat");
	
	if (!fin.good()) 
		return false; // exit if file not found

	// read each line from the encoding-profiles file
	string line;
	unsigned short it_line = 0;
	while (getline (fin, line)){
		// ignore empty lines
		if (line.length() == 0){
			continue;
		}

		// get each token (delimited by commas) from each line
		istringstream iss(line);
		string token;
		unsigned short it_token = 0;
		while (getline(iss, token, ',')){
			// skip leading white space
			iss >> ws;
			// todo:	check that no comments or unknown characters!
			//			only checking for empty lines right now
			arrTokens[it_token++] = token;
		}
		
		// convert the std:strings to CStrings
		arrProfiles[it_line].name	= arrTokens[0].c_str();
		arrProfiles[it_line].videoEncoding = arrTokens[1].c_str();
		arrProfiles[it_line].audioEncoding = arrTokens[2].c_str();

		wcout << arrProfiles[0].name.GetString() << endl;
		wcout << arrProfiles[0].videoEncoding.GetString() << endl;
		wcout << arrProfiles[0].audioEncoding.GetString() << endl;
		wcout << endl;

		it_line++;
	}
	// close the encoding-profiles file
	fin.close();
	
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool result = readEncodingProfiles();

	system("pause");
	return 0;
}