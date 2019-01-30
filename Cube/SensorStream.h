#pragma once



#include "stdafx.h"
#include <sstream>
#include <fstream>
using namespace std;
using namespace cv;


class SensorStream
{
public:
	SensorStream(string path);
	void loadReading(vector <double> &score);
	~SensorStream();

private:
	void getData(string data, vector <vector<double>> &data2);
	ifstream file;
	string path;
	vector <vector<double>> data2;
};


