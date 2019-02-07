// Cube.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Model.h"
#include "Reader.h"
#include "Exceptions.h"
#include "TraceLogger.h"
#include "Catch\Catch.h"
#include "Server.h"
#include "SensorStream.h"
#include <memory>
#include <mutex>







//using namespace std;
//using namespace cv::;
//funkcje kolejno rotacji punktów 3d, obliczania œrodkow ka¿dej ze scian i obslugi przyciskow
void dysplay(vector <Mat> imgs) {
	namedWindow("win", WINDOW_NORMAL);
	namedWindow("res", WINDOW_NORMAL);
	for (auto &img : imgs) {
		imshow("win", img);
		waitKey(0);
	}
}
int main(int argc, char **argv) {
	auto poprzednia = std::chrono::steady_clock::now();
	namedWindow("okno", WINDOW_NORMAL);
	namedWindow("okno2", WINDOW_NORMAL);
	SensorStream sensors;
	std::mutex mutex;
	Server serv;
	String aa;
	std::thread t1(&Server::reciveData, serv, std::ref(aa));
	string confpath;
	Model m1, m2;
	vector <Mat> images;
	unique_ptr <Reader> reader;
	if (argc > 1)
		confpath = argv[1];
	else
		confpath = "defaultconfig.txt";
	try {
		Reader::open(confpath, reader);
		while (images.size() < 6) {
			try {
				reader->load(images);
			}
			catch (ImageFileException a) {
				cout << a.getMessage();
				cout << "co chesz zrobic?\n1. Zrobic zdjêcie\n2. Zakonczyc \n3. Wczytaæ poprzednie zdjêcie" << endl;
				int choise = 0;
				cin >> choise;
				if (choise == 1) {
					if (Reader::makeFoto(images) == false) {
						cout << "Camera is disable\n";
						return -1;
					}
				}
				else if (choise == 2)
					return -1;
				else if (choise == 3) {
					if (images.empty() == false)
						images.push_back(images[images.size() - 1]);
					else
						cout << "nie da sie wczytac poprzedniego pliku\n";
				}
			}
			catch (ImageModeException b) {
				cout << b.getMessage();
				cout << "Co chcesz zrobic? \n1.Pomin filtr\n2. uzyj gausa\n3. uzyj threshold\n";
				int choise = 0;
				cin >> choise;
				Mat img = imread(b.getPath());
				if (choise == 1)
					images.push_back(img);
				else if (choise == 2) {
					Reader::gauss(img, img);
					images.push_back(img);
				}
				else if (choise == 3) {
					Reader::thresholding(img, img);
					images.push_back(img);
				}
			}
		}
	}
	catch (TooShortConfigException tooshort) {
		cout << tooshort.getMessage();
		return -1;
	}
	catch (ConfigurationFileException conf) {
		cout << "Brak pliku konfiguracyjnego lub nieprawidlowe rozszerzenie\n";
		return -1;
	}

	unsigned int sign=0, sign2=0;
	vector <double> wyniki;
	m1 = Model(images, Vec3f(0.4, 0.4, 0));
	m2 = Model(images, Vec3f(0.55, 0.53, 0));
	while (sign != 27) {
		//auto start = std::chrono::steady_clock::now();
		//auto time = start - poprzednia;
		//cout << time.count() << endl;
		//cout << chrono::duration_cast<chrono::milliseconds>(time).count() << endl;
		//cout << "Main: " << aa << endl;
		Mat img2, res;
		mutex.lock();
		sensors.loadReading(wyniki, aa);
		mutex.unlock();
		//sensors.visualisationPosition(wyniki, res);
		
		m1.setPosition(Point2d( wyniki[1]/2, wyniki[0]/2));
		m2.setPosition(Point2d(wyniki[1]/2, wyniki[0]/2));
		//m1.key_handling(sign);
		m1.setRot(Vec3f(0.01, 0.00, 0));
		m1.mul();
		m1.mean();
		m1.visiable_walls();
		//m2.key_handling(sign2);
		m2.setRot(Vec3f(0.01, 0.00, 0));
		m2.mul();
		m2.mean();
		m2.visiable_walls();
		Mat img;
		
		m1.dysplay(img);
		m2.dysplay(img2);
		imshow("okno", img);
		imshow("okno2", img2);
		sign = waitKey(1);
		sign2 = waitKey(1);
		Model::myvconnect(img, img2, res);
		
		imshow("res", res);
		waitKey(10);
		//poprzednia = start;
	}
	t1.join();
	m1.~Model();
	return 0;
}
