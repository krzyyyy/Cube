
#include "SensorStream.h"
#include <cmath>


SensorStream::SensorStream(string path) {
	this->path = path;

}


void SensorStream::loadReading(vector <double> &score) {
	score.clear();
	double ax, ay, az;
	int licz = 0;
		file.open(path);
		if (file.is_open()) {

			//while (!file.eof()) {
			licz++;
			string line;
			string x, y, z;
			getline(file, x);
			getData(x, data2);
			if (x.size() < 3)
				throw string("za krotki wiersz");
			ax = data2[0][0];
			ay = data2[0][1];
			az = data2[0][2];
			double g = sqrt((ax*ax) + (az * az));
			double alfa = acos(az / g);
			cout << "ax: " << ax << "  az: " << az << endl;
			cout << licz << "  Przyspieszenie ziemskie: " << g << " kat " << alfa * 180 / 3.141592 << endl;
			double x0 = 150 * tan((3.141592 / 2) - alfa);
			cout << "Przesuniecie: " << x0 << endl;
			score.push_back(x0);
			//file.clear();
			//file.seekg(0, ios::beg);
			//}
			file.close();
		}
		else
			throw string("nie mozna otworzyc pliku");
}

void SensorStream::getData(string data, vector <vector<double>> &data2) {
	data2.clear();
	vector <int> id;
	if (data.size() < 3)
		return;
	string line(data.begin() + 2, data.end() - 1);
	double time;
	stringstream ss;

	for (auto c = line.begin(); c != line.end(); c++) {
		if (*c == ',')
			*c = ' ';
	}
	ss << line;
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
	for (int j = 0; j < scores[i].size(); j++) {
	cout << scores[i][j] << " ";
	}
	}*/

	//cout << endl;
}
SensorStream::~SensorStream()
{
}