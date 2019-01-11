#pragma once
#include "stdafx.h"
#include <memory>
#include <fstream>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
using namespace std;
using namespace cv;

class Reader
{
public:
	Reader();
	Reader(string path);
	Reader(Reader& rd);
	virtual void load(vector <Mat>& images)=0;
	static bool makeFoto(vector <Mat>& imeges);
	static void gauss(Mat &imgIn, Mat &imgOut);
	static void thresholding(Mat &imgIn, Mat &imgOut);
	static void open(string path, unique_ptr<Reader> &ptr);
	Reader& operator= (Reader& rd);
	~Reader();

protected:
	string confpath;
	ifstream file;
	unsigned int linefile;
private:
	static void writeLog(string log, string type);

};
class ReaderXML :public Reader {
public:
	ReaderXML();
	ReaderXML(string path);
	ReaderXML(ReaderXML& rd);
	void load(vector <Mat>& images);
private:
	vector<char> buffer;
	rapidxml::xml_document<> xmlfile;
	rapidxml::xml_node<>* currentphoto;
};
class ReaderTXT :public Reader {
public:
	ReaderTXT();
	ReaderTXT(string path);
	ReaderTXT(ReaderTXT &rd);
	void load(vector <Mat>& images);
private:

};

