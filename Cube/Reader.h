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
	void load(vector <Mat>& images);
	void loadXML(vector <Mat>& images);
	bool makeFoto(vector <Mat>& imeges);
	void gauss(Mat &imgIn, Mat &imgOut);
	void thresholding(Mat &imgIn, Mat &imgOut);
	void open();
	~Reader();

private:
	string confpath;
	fstream file;
	unsigned int linefile;
	enum class Extension { TXT, XML };
	Extension ext;
	rapidxml::xml_document<> xmlfile;


};

