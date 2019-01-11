#pragma once
#include "stdafx.h"

using namespace std;

class TraceLogger {
private:
	static string start;
	static vector <string> done;
	static vector <string> actions;
public:
	TraceLogger();
	static void addLog(string a);
	static string toString();
	static void complite();
	static unsigned int getSize();
};