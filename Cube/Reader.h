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
	~Reader();

private:
	fstream file;
	void gaus(Mat &imgIn, Mat &imgOut );


};

