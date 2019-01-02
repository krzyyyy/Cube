#pragma once
#include "stdafx.h"
using namespace std;
using namespace cv;

class ErrorLoger {
private:
	static string errorlog;
public:
	string getLog();
	void addLog(string log);
};
void ErrorLoger::addLog(string log) {
	errorlog += log;
}
string ErrorLoger::getLog() {
	return errorlog;
}
class ImageFileException :protected ErrorLoger{
private:
	string message;
public:
	ImageFileException(int line, string path);

};
ImageFileException::ImageFileException(int line, string path) {
	message = "I can't open " + path + ". It's at line number " + line +
		"in configuration file\n";
	addLog(message);
}
class ConfigurationFileException :protected ErrorLoger {
private:
	string message;
public:
	ConfigurationFileException( string path);
};
ConfigurationFileException::ConfigurationFileException( string path) {
	message = "I can't open configuration file:  " + path + "\n";
	addLog(message);
}