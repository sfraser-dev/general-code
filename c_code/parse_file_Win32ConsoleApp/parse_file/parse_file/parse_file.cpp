// parse_file.cpp : Defines the entry point for the console application.
//
// http://cs.dvc.edu/HowTo_Cparse.html ("c++ parse file")

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

const int MAX_CHARS_PER_LINE = 2048;
const int MAX_TOKENS_PER_LINE = 20;
const char* const DELIMITER = ",";

bool getNextLine() {
	ifstream fin;
	fin.open("F:\\dev\\general\\c_code\\parse_file_Win32ConsoleApp\\parse_file\\parse_file\\VitecEncodingProfiles.dat");
	
	if (!fin.good()) 
		return false; // exit if file not found

	// read each line of the file
	while (!fin.eof()){
		// read an entire line into memory
		char buf[MAX_CHARS_PER_LINE];
		fin.getline(buf, MAX_CHARS_PER_LINE);
    
		// parse the line into blank-delimited tokens
		int n = 0; // for-loop index
		// array to store memory addresses of the tokens in buf
		const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0
    
		// parse the line
		token[0] = strtok(buf, DELIMITER); // first token
		if (token[0]) { // zero if line is blank
			for (n = 1; n < MAX_TOKENS_PER_LINE; n++){
				token[n] = strtok(0, DELIMITER); // subsequent tokens
				if (!token[n]) break; // no more tokens
			}
		}
		else {
			continue; // ignore blank lines
		}

		// process (print) the tokens
		for (int i = 0; i < n; i++){	// n = #of tokens
			cout << "Token[" << i << "] = " << token[i] << endl;
			string tokenStart = "";
			// grab the first seven characters of the token
			tokenStart += token[i][0];
			tokenStart += token[i][1];
			tokenStart += token[i][2];
			tokenStart += token[i][3];
			tokenStart += token[i][4];
			tokenStart += token[i][5];
			tokenStart += token[i][6];
			// remove blank spaces
			tokenStart.erase(remove_if(tokenStart.begin(), tokenStart.end(), isspace), tokenStart.end());
			cout << "str: " << tokenStart << endl;
		}
		cout << endl;
	}

	fin.close();
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool result;

	cout << "Test" << endl;
	result = getNextLine();

	system("pause");
	return 0;
}

