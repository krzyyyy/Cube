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
string ErrorLoger::errorlog = "";
ImageFileException::ImageFileException(int line, string path) {
	this->path = path;
	string mess = "I can't open " + path + ". It's at line number " + to_string(line) +
		" in configuration file\n";
	putMess(mess);
}
ConfigurationFileException::ConfigurationFileException(string path) {
	this->path = path;
	string mess = "I can't open configuration file:  " + path + "\n";
	this->putMess(mess);
}
ConfigurationFileException::ConfigurationFileException(){}

ImageModeException::ImageModeException(int line, string mode, string path) {
	this->mode = mode;
	this->path = path;
	string mess = "Mode " + mode + " is undefined. It's at line number " + to_string(line)+" in configuration file.\n" ;
	this->putMess(mess);
}
string ImageModeException::getPath() {
	return path;
}
TooShortConfigException::TooShortConfigException() {}
TooShortConfigException::TooShortConfigException(string path, int len):
	ConfigurationFileException(path) {
	this->len=len;
	string mess = "In " + path + " is: " + to_string(len) + " images. It's not enough.\n";
	this->putMess(mess);
}