
#include "stdafx.h"
#include "TraceLogger.h"

TraceLogger::TraceLogger() {}
void TraceLogger::addLog( string a, string m) {
	stringstream strictname(m);
	string classnam, methodnam;
	getline(strictname, classnam, ':');
	getline(strictname, methodnam);
	methodnames.push_back(methodnam);
	classnames.push_back(classnam);
	actions.push_back(a);
}
string TraceLogger::toString() {
	string score = "";
	for (unsigned int i = 0; i < actions.size(); i++) {
		score += actions[i] + methodnames[i] + classnames[i] + "true\n";
	}
	return score;
}
string TraceLogger::start = "";
vector <string> TraceLogger::classnames = vector <string>();
vector <string> TraceLogger::methodnames = vector <string>();
vector <string> TraceLogger::actions = vector <string>();
