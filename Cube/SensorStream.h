#pragma once



#include "stdafx.h"
#include <sstream>
#include <fstream>
using namespace std;
using namespace cv;


class SensorStream
{
public:
	SensorStream();
	void visualisationPosition(vector<double> pos, Mat &imgout);
	void loadReading(vector <double> &score, string measurement);
	~SensorStream();
private:
	void getData(string data, vector <vector<double>> &data2);
	ifstream file;
	vector <vector<double>> data2;
};


