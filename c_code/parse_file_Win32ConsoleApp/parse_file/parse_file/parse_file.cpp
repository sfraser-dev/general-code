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
#include <algorithm> 
#include <functional> 
#include <cctype>


typedef struct _LWProfileDef {
	CString name;		
	CString videoEncoding;
	CString audioEncoding;
} LWProfileDef_t;

// split line at delimiter
void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

// split line at delimiter
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
	split(s, delim, elems);
    return elems;
}

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

bool readEncodingProfiles() {

	// open the encoding-profiles file
	std::ifstream fin;
	fin.open("F:\\dev\\general\\c_code\\parse_file_Win32ConsoleApp\\parse_file\\parse_file\\VitecEncodingProfiles.dat");
	
	// exit if file not found
	if (!fin.good()) 
		return false; 

	// read the encoding-profiles file
	std::string line;
	std::vector<LWProfileDef_t> vecProfiles;
	// grabbing each line from the encoding-profiles file
	while (std::getline (fin, line)){
		// ignore empty lines
		if (line.length() == 0)
			continue;
		// remove comments
		std::vector<std::string> vectorCommentSplit = split(line, '#');
		std::string lineNoComment = vectorCommentSplit[0];
		// ignore lines that are just comments
		if (lineNoComment.length() == 0)
			continue;

		// from each line, get the three tokens delimited by commas
		std::vector<std::string> vectorCommaSplit = split(line, ',');
		// expecting three comma separated strings (1) "name=...", (2) "video=..." and (3) "audio=..."
		if (vectorCommaSplit.size() != 3)	
			return false;
		
		// store tokens in a temporary ProfileDefinition structure
		// convert std:strings to CStrings
		// trim empty space from the start and end of the strings
		LWProfileDef_t tempProfile;
		tempProfile.name = (trim(vectorCommaSplit[0])).c_str();
		tempProfile.videoEncoding = (trim(vectorCommaSplit[1])).c_str();
		tempProfile.audioEncoding = (trim(vectorCommaSplit[2])).c_str();
		
		// store the ProfileDefinition structures in a vector
		vecProfiles.push_back(tempProfile);
	}
	// close the encoding-profiles file
	fin.close();
	
	// store the structures in an array (not a vector) to fit in easier with the current DVR code
	LWProfileDef_t *arr7440ProfileDef = new LWProfileDef_t[vecProfiles.size()];
	// test by printing to screen
	for (int i=0; i<vecProfiles.size(); i++){
		arr7440ProfileDef[i] = vecProfiles[i];
		std::wcout << arr7440ProfileDef[i].name.GetString() << std::endl;
		std::wcout << arr7440ProfileDef[i].videoEncoding.GetString() << std::endl;
		std::wcout << arr7440ProfileDef[i].audioEncoding.GetString() << std::endl;
		std::wcout << std::endl;
	}

	// tidy up
	vecProfiles.clear();
	delete [] arr7440ProfileDef;

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool result = readEncodingProfiles();

	system("pause");
	return 0;
}