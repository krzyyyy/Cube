#pragma once
#include "stdafx.h"

using namespace std;

class TraceLogger {
private:
	static string start;
	static vector <string> classnames;
	static vector <string> methodnames;
	static vector <string> actions;
public:
	TraceLogger();
	static void addLog(string a, string m);
	static string toString();
	
};