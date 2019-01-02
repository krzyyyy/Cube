// Cube.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Model.h"
#include "Reader.h"





using namespace std;
using namespace cv;
//funkcje kolejno rotacji punktów 3d, obliczania œrodkow ka¿dej ze scian i obslugi przyciskow
void dysplay(vector <Mat> imgs) {
	namedWindow("win", WINDOW_NORMAL);
	for (auto &img : imgs) {
		imshow("win", img);
		waitKey(0);
	}
}

int main() {
	Model m1;
	vector <Mat> images;
	try {
		Reader rd("configuration.txt");
		rd.load(images);
	}
	catch (string a) {
		cout << a << endl;
	}
	dysplay(images);
	unsigned int sign=0;
	
	m1 = Model(images);
	while (sign != 27) {
		m1.key_handling(sign);
		m1.mul();
		m1.mean();
		m1.visiable_walls();
		sign = m1.dysplay();
	}
	return 0;
}
