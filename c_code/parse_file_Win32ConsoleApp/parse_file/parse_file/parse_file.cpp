// parse_file.cpp : Defines the entry point for the console application.
//
// http://cs.dvc.edu/HowTo_Cparse.html ("c++ parse file")

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <atlstr.h>
#include <vector>

typedef struct _LWProfileDef {
	CString name;		
	CString videoEncoding;
	CString audioEncoding;
} LWProfileDef_t;

bool readEncodingProfiles() {
	std::ifstream fin;

	// open the encoding-profiles file
	fin.open("F:\\dev\\general\\c_code\\parse_file_Win32ConsoleApp\\parse_file\\parse_file\\VitecEncodingProfiles.dat");
	
	if (!fin.good()) 
		return false; // exit if file not found

	// read the encoding-profiles file
	std::string line;
	std::vector<LWProfileDef_t> vecProfiles;
	// grabbing each line from the encoding-profiles file
	while (std::getline (fin, line)){
		// ignore empty lines
		if (line.length() == 0){
			continue;
		}

		// from each line, get the three tokens (1) name (2) videoEncoding (3) audioEncoding (which are delimited by commas)
		std::istringstream iss(line);
		std::string token;
		std::string arrTokens[3];
		unsigned short it_token = 0;
		// parse each line
		while (getline(iss, token, ',')){
			// skip leading white space
			iss >> std::ws;
			// todo:	check that no comments or unknown characters!
			//			only checking for empty lines right now
			arrTokens[it_token++] = token;
		}
		
		// store tokens in a temporary ProfileDefinition structure (converting std:strings to CStrings)
		LWProfileDef_t tempProfile;
		tempProfile.name = arrTokens[0].c_str();
		tempProfile.videoEncoding = arrTokens[1].c_str();
		tempProfile.audioEncoding = arrTokens[2].c_str();
		// store the ProfileDefinition structures in a vector
		vecProfiles.push_back(tempProfile);
	}
	// close the encoding-profiles file
	fin.close();
	
	// test by printing to screen
	for (int i=0; i<vecProfiles.size(); i++){
		std::wcout << vecProfiles[i].name.GetString() << std::endl;
		std::wcout << vecProfiles[i].videoEncoding.GetString() << std::endl;
		std::wcout << vecProfiles[i].audioEncoding.GetString() << std::endl;
		std::wcout << std::endl;
	}

	// done
	vecProfiles.clear();
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool result = readEncodingProfiles();

	system("pause");
	return 0;
}