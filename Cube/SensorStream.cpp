
#include "SensorStream.h"
#include <cmath>


SensorStream::SensorStream() {

}


void SensorStream::loadReading(vector <double> &score, string measurement) {
	score.clear();
	double ax, ay, az, x1;
	static double horizposition = 0;
	int licz = 0;
	getData(measurement, data2);
	if (data2.size() < 1) {
		score.push_back(0);
		score.push_back(0);
		return;
	}
		
	ax = data2[0][0];
	ay = data2[0][1];
	az = data2[0][2];
	if (data2.size() >= 4) {
		x1 = 150 * tan(data2[3][0] * 3.14 / 180);
		horizposition = x1;
	}
	else
		x1 = horizposition;
	double g = sqrt((ax*ax) + (az * az));
	double alfa = acos(az / g);
	//cout << "ax: " << ax << "  az: " << az << endl;
	//cout << licz << "  Przyspieszenie ziemskie: " << g << " kat " << alfa * 180 / 3.141592 << endl;
	double x0 = 150 * tan((3.141592 / 2) - alfa);
	cout << "Przesuniecie: " << x0 << endl;
	score.push_back(x0);
	score.push_back(x1);
			//file.clear();
			//file.seekg(0, ios::beg);
			//}
}

void SensorStream::getData(string data, vector <vector<double>> &data2) {
	data2.clear();
	vector <int> id;
	if (data.size() < 3)
		return;
	double time;
	stringstream ss;

	for (auto c = data.begin(); c != data.end(); c++) {
		if (*c == ',')
			*c = ' ';
	}
	ss << data;
	ss >> time;
	while (ss.rdbuf()->in_avail() != 0)
	{
		int elem;
		ss >> elem;
		id.push_back(elem);
		data2.push_back(vector <double>());
		for (int i = 0; i < 3; i++) {
			double val;
			ss >> val;
			data2.back().push_back(val);
		}
	}
	/*for (int i = 0; i < id.size(); i++) {
	cout << id[i] << " nalezy do niego: ";
	for (int j = 0; j < data2[i].size(); j++) {
	cout << data2[i][j] << " ";
	}
	}*/

	//cout << endl;
}
SensorStream::~SensorStream()
{
}