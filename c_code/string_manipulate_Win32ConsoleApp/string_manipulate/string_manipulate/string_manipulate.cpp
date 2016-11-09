// string_manipulate.cpp : Defines the entry point for the console application.
//
// Win32 console application

#include "stdafx.h"

#include <process.h>
#include <atlstr.h>
#include <iostream>
#include <string>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	// MicroSoft propriety CString in console must use wide
	CString str1("hello.mp4");
	wcout << "Filename: " << (const wchar_t*) str1 << endl;
	
	cout << endl;

	// look for location of extension period, keep only the part of the name left of the period (basename)
	int idx = str1.ReverseFind('.');
	if (idx < 0) {
		cout << "ERROR: extension period not found for HD / SD file renaming" << endl;
	}
	cout << "period index = " << idx << endl;
	CString basename(str1.Left(idx));
	wcout << "basename = " << (const wchar_t*) basename << endl;
	CString newname = basename + "-hd.mp4";
	wcout << "newname = " << (const wchar_t*) newname << endl;

	cout << endl;

	// use total length of string and assume extension is ".mpg", ie: extension is four characters long
	int len = str1.GetLength();
	cout << "total string length = " << len << endl;
	int periodAndExtnLen = 4;
	CString basename2(str1.Left(len-periodAndExtnLen));
	wcout << "basename2 = " << (const wchar_t*) basename2 << endl;
	CString newname2 = basename2 + "-hd.mp4";
	wcout << "newname2 = " << (const wchar_t*) newname2 << endl;

	//wcout << (const wchar_t*) str1.Right(idx) << endl;
	//CString str1_hd(str1.Right(str1.ReverseFind('.')));
	//wcout << (const wchar_t*) str1_hd << endl;

	system("pause");
	return 0;
}
