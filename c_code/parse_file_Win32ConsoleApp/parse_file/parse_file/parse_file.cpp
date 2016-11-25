// parse_file.cpp : Defines the entry point for the console application.
//
// http://cs.dvc.edu/HowTo_Cparse.html ("c++ parse file")

#include "stdafx.h"

#include <afx.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <atlstr.h>
#include <vector>
#include <algorithm> 
#include <functional> 
#include <cctype>

#define ASSERT(f)          DEBUG_ONLY((void) ((f) || !::AfxAssertFailedLine(THIS_FILE, __LINE__) || (AfxDebugBreak(), 0)))
#define LW_ENABLE_DEBUG_TRACE_NOTIFY

typedef struct _LWProfileDef {
	CString name;		
	CString videoEncoding;
	CString audioEncoding;
} LWProfileDef_t;

// functions for splitting strings at a delimiter
static void StringSplit(const std::string &s, char delim, std::vector<std::string> &elems);
static std::vector<std::string> StringSplit(const std::string &s, char delim);
// functions for trimming spaces from the front and the end of a string
static inline std::string &StringTrimLeft(std::string &s);
static inline std::string &StringTrimRight(std::string &s);
static inline std::string &StringTrim(std::string &s);
// convert string to upper-case
static inline std::string ToUpper (const std::string& strIn);

bool readEncodingProfiles() {

	// open the encoding-profiles file
	std::ifstream fin;
	fin.open("C:\\ProgramData\\VisualSoft\\VisualWorks\\DVR\\QualityProfiles\\H264\\Hardware\\VitecEncodingProfiles.dat");
	
	// exit if file not found
	if (!fin.good()) {
#ifdef LW_ENABLE_DEBUG_TRACE_NOTIFY
		TRACE("Error: cannot access the Vitec Encoder Profiles file");
#endif
		ASSERT(FALSE);
		return false; 
	}

	// read the encoding-profiles file
	std::string lineFromFile;
	std::vector<std::string> commentSplit;
	std::string linesWithoutComments;
	std::vector<std::string> noComments;
	// grabbing each line from the encoding-profiles file
	while (std::getline (fin, lineFromFile)){
		// ignore empty lines
		if (lineFromFile.length() == 0)
			continue;
		// remove comments
		commentSplit = StringSplit(lineFromFile, '#');
		// ignore lines that are just comments
		if (commentSplit[0].length() == 0)
			continue;
		// ignore lines that don't specifically start with "name" (where "name" is case insensitive)
		if (ToUpper((StringTrim(commentSplit[0]))).find("NAME") != 0)
			continue;
		noComments.push_back(commentSplit[0]);
	}

	std::vector<std::string> commaTokens;
	std::string profileName;
	std::vector<LWProfileDef_t> encodingProfiles;
	for(int i=0; i<noComments.size(); i++){
		// split each line into tokens (delimited by commas)
		commaTokens = StringSplit(noComments[i],',');
		if (commaTokens.size() != 3) {
#ifdef LW_ENABLE_DEBUG_TRACE_NOTIFY
			TRACE("Error: line found in Vitec Encoder Profiles file without three comma separated tokens");
#endif
			ASSERT(FALSE);
			continue;
		}
		// remove white space from start and end of each token and store them in a temporary structure
		LWProfileDef_t tempProfile;
		tempProfile.name = (StringTrim(commaTokens[0])).c_str();
		tempProfile.videoEncoding = (StringTrim(commaTokens[1])).c_str();
		tempProfile.audioEncoding = (StringTrim(commaTokens[2])).c_str();
		// store the temporary structure
		encodingProfiles.push_back(tempProfile);
	}
	// close the encoding-profiles file
	fin.close();

	if (encodingProfiles.size() != 0){
		// copy the vector of structures to the "old" array (minimise changes for now)
		LWProfileDef_t *arr7440ProfileDef = new LWProfileDef_t[encodingProfiles.size()];
		for (int i=0; i<encodingProfiles.size(); i++)
			arr7440ProfileDef[i] = encodingProfiles[i];

		// debug
		for (int i=0; i<encodingProfiles.size(); i++){
			std::wcout << encodingProfiles[i].name.GetString() << std::endl;
			std::wcout << encodingProfiles[i].videoEncoding.GetString() << std::endl;
			std::wcout << encodingProfiles[i].audioEncoding.GetString() << std::endl;
			std::wcout << std::endl;
		}

		// tidy up
		delete [] arr7440ProfileDef;
		arr7440ProfileDef = NULL;
	}
	else {
#ifdef LW_ENABLE_DEBUG_TRACE_NOTIFY
		TRACE("Error: No profiles have been read from the Vitec Encoder Profiles file");
#endif
		ASSERT(FALSE);
	}

	// tidy up
	encodingProfiles.clear();

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool result = readEncodingProfiles();

	system("pause");
	return 0;
}

// split string at delimiter
void StringSplit(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

// split string at delimiter
std::vector<std::string> StringSplit(const std::string &s, char delim) {
    std::vector<std::string> elems;
	StringSplit(s, delim, elems);
    return elems;
}

// trim string from start
static inline std::string &StringTrimLeft(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim string from end
static inline std::string &StringTrimRight(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim string from both ends
static inline std::string &StringTrim(std::string &s) {
    return StringTrimLeft(StringTrimRight(s));
}

// string conversion to uppercase
static inline std::string ToUpper (const std::string& strIn){
	std::string tempStr(strIn);
	//temp.resize(strIn.length());
	std::transform(strIn.begin(), strIn.end(), tempStr.begin(), ::toupper);
	return tempStr;
}