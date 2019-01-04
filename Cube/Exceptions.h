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


class ConfigurationFileException :protected ErrorLoger {
private:
	string path;
public:
	ConfigurationFileException( string path);
};
class ImageModeException:ErrorLoger {
private:
	string mode;
public:;
	ImageModeException(int line, string mode);
};