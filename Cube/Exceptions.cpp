#include "stdafx.h"
#include "Exceptions.h"




void ErrorLoger::addLog(string log) {
	errorlog += log;
}
string ErrorLoger::getLog() {
	return errorlog;
}
void ErrorLoger::putMess(string mess){
	this->message = mess;
	addLog(mess);
}
string ErrorLoger::getMessage() {
	return message;
}
ImageFileException::ImageFileException(int line, string path) {
	this->path = path;
	string mess = "I can't open " + path + ". It's at line number " + to_string(line) +
		" in configuration file\n";
	putMess(mess);
}
ConfigurationFileException::ConfigurationFileException(string path) {
	this->path = path;
	string mess = "I can't open configuration file:  " + path + "\n";
	putMess(mess);
}
string ErrorLoger::errorlog = "";

ImageModeException::ImageModeException(int line, string mode, string path) {
	this->mode = mode;
	string mess = "Mode " + mode + " is undefined. It's at line number " + to_string(line)+" in configuration file.\n" ;
	putMess(mess);
}
string ImageModeException::getPath() {
	return path;
}