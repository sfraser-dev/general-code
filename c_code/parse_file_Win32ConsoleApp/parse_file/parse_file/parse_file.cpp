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
//using namespace std;

typedef struct _LWProfileDef {
	CString name;		
	CString videoEncoding;
	CString audioEncoding;
} LWProfileDef_t;

bool readEncodingProfiles() {
	std::ifstream fin;
	LWProfileDef_t arrProfiles[20];

	// open the encoding-profiles file
	fin.open("F:\\dev\\general\\c_code\\parse_file_Win32ConsoleApp\\parse_file\\parse_file\\VitecEncodingProfiles.dat");
	
	if (!fin.good()) 
		return false; // exit if file not found

	// read each line from the encoding-profiles file
	std::string line;
	unsigned short it_line = 0;
	LWProfileDef_t tempProfile;
	std::vector<LWProfileDef_t> vecProfiles;
	while (std::getline (fin, line)){
		// ignore empty lines
		if (line.length() == 0){
			continue;
		}

		// get each token (delimited by commas) from each line
		std::istringstream iss(line);
		std::string token;
		std::string arrTokens[3];
		unsigned short it_token = 0;
		while (getline(iss, token, ',')){
			// skip leading white space
			iss >> std::ws;
			// todo:	check that no comments or unknown characters!
			//			only checking for empty lines right now
			arrTokens[it_token++] = token;
		}
		
		// convert the std:strings to CStrings
		arrProfiles[it_line].name	= arrTokens[0].c_str();
		arrProfiles[it_line].videoEncoding = arrTokens[1].c_str();
		arrProfiles[it_line].audioEncoding = arrTokens[2].c_str();

		tempProfile.name = arrTokens[0].c_str();
		tempProfile.videoEncoding = arrTokens[1].c_str();
		tempProfile.audioEncoding = arrTokens[2].c_str();

		vecProfiles.push_back(tempProfile);

		std::wcout << "array: " << arrProfiles[it_line].name.GetString() << std::endl;
		std::wcout << "array: " << arrProfiles[it_line].videoEncoding.GetString() << std::endl;
		std::wcout << "array: " << arrProfiles[it_line].audioEncoding.GetString() << std::endl;
		std::wcout << std::endl;

		it_line++;
	}
	// close the encoding-profiles file
	fin.close();
	
	std::cout << "Printing the vector: " << std::endl;
	for (int i=0; i<vecProfiles.size(); i++){
		std::wcout << vecProfiles[i].name.GetString() << std::endl;
		std::wcout << vecProfiles[i].videoEncoding.GetString() << std::endl;
		std::wcout << vecProfiles[i].audioEncoding.GetString() << std::endl;
		std::wcout << std::endl;
	}

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool result = readEncodingProfiles();

	system("pause");
	return 0;
}