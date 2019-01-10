
#include "stdafx.h"
#include "TraceLogger.h"

TraceLogger::TraceLogger() {
}
void TraceLogger::addLog( string a) {
	actions.push_back(a);
	done.push_back("fail");
}
string TraceLogger::toString() {
	string score = start;
	for (unsigned int i = 0; i < actions.size(); i++) {
		score += actions[i]+ "  "+done[i]+"\n";
	}
	return score;
}
void TraceLogger::complite() {
	done[done.size() - 1] = "completed succesfull";
}
string TraceLogger::start = "";
vector <string> TraceLogger::done = vector <string>();
vector <string> TraceLogger::actions = vector <string>();
