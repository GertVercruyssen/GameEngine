#pragma once

#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <tchar.h>

using namespace std;
#ifdef _UNICODE	// extra unicode defines
	#define tstring wstring
	#define tcin wcin
	#define tcout wcout
	#define tstringstream wstringstream
	#define tofstream wofstream
	#define tifstream wifstream
	#define tfstream wfstream
	#define tstrtod	wcstod
	#define tof _wtof
#else
	#define tstring string
	#define tcin cin
	#define tcout cout
	#define tstringstream stringstream
	#define tofstream ofstream
	#define tifstream ifstream
	#define tfstream fstream
	#define	tstrtod strtod
	#define tof atof
#endif

#define UINT unsigned int