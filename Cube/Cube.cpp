// Cube.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Model.h"





using namespace std;
using namespace cv;
//funkcje kolejno rotacji punktów 3d, obliczania œrodkow ka¿dej ze scian i obslugi przyciskow
int main() {
	Model m1;
	vector <Mat> images;
	unsigned int sign=0;
	for (int i = 1; i < 7; i++) { // wczytywanie obrazow
		stringstream name;
		name << i;
		images.push_back(imread("images/" + name.str() + ".png"));
	}
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
