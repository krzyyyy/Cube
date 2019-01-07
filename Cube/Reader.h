#pragma once
#include "stdafx.h"
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
	virtual void load(vector <Mat>& images)=0;
	static bool makeFoto(vector <Mat>& imeges);
	static void gauss(Mat &imgIn, Mat &imgOut);
	static void thresholding(Mat &imgIn, Mat &imgOut);
	static Reader* open(string path);
	~Reader();

protected:
	string confpath;
	ifstream file;
	unsigned int linefile;


};
class ReaderXML :public Reader {
public:
	ReaderXML();
	ReaderXML(string path);
	void load(vector <Mat>& images);
private:
	vector<char> buffer;
	rapidxml::xml_document<> xmlfile;
};
class ReaderTXT :public Reader {
public:
	ReaderTXT();
	ReaderTXT(string path);
	void load(vector <Mat>& images);
private:

};

