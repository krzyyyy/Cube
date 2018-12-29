
#include "stdafx.h"
using namespace std;
using namespace cv;

class Model {
private:
	vector <Mat> images;// (0);// obrazy na œcianach kostki
	vector <int> band; // skladowe wspolzendnych punktow 3d

	vector <Vec3f> points_3d, points_mean_3d;//kolejno wierzcholki kostki i punkty przeciecia przekatnych dla kazdej ze scian
	vector <Vec2f> points_2d, points_mean_2d;// jw. w 2d
	vector <vector<size_t>> id_img_points, id_img_points_good;// wektor indeksow punktow dla kazdej ze scian
																	//parametry dla funkcji projectPoints
	double focal_length = 150;// ogniskowa
	Point2d center;// miejsce poczatku ukladu wspulrzednych obrazu
	Mat camera_matrix;
	Mat dist_coeffs;// info o znieksztalceniu obrazu przez soczewke itd. - brak
	Vec3f trans; // odleglosc od obiektu
	int sign = 0; // nr. znaku
	Mat rot_mat;
	Vec3f rot_vec;
	vector <Vec2f> corner_points;
public:
	Model();
	Model(vector <Mat> imgs);
	void key_handling(int sign);

};

