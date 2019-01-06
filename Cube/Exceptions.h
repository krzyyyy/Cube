#pragma once
#include "stdafx.h"
using namespace std;
using namespace cv;

class ErrorLoger {
private:
	static string errorlog;
	string message;
	void addLog(string log);
public:
	static string getLog();

	void putMess(string mess);
	string getMessage();
};


class ImageFileException :public ErrorLoger{
private:
	string path;
public:
	ImageFileException(int line, string path);
};


class ConfigurationFileException :public ErrorLoger {
private:
	string path;
public:
	ConfigurationFileException();
	ConfigurationFileException( string path);
};
class TooShortConfigException : public ConfigurationFileException {
private:
	int len;
public:
	TooShortConfigException();
	TooShortConfigException(string path, int len);
};
class ImageModeException: public ErrorLoger {
private:
	string mode;
	string path;
public:;
	ImageModeException(int line, string mode, string path);
	string getPath();
};