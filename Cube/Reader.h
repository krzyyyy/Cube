#pragma once
#include "stdafx.h"
#include <fstream>
using namespace std;
using namespace cv;

class Reader
{
public:
	Reader();
	Reader(string path);
	void load(vector <Mat>& images);
	bool makeFoto(vector <Mat>& imeges);
	~Reader();

private:
	fstream file;
	unsigned int linefile;
	void gauss(Mat &imgIn, Mat &imgOut );
	void thresholding(Mat &imgIn, Mat &imgOut);
};

