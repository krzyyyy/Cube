
#include "stdafx.h"
#include "Model.h"

Model::Model(){}
Model::Model(vector <Mat> imgs):images(imgs), band({ -100, 100 }),
								center(0 ,0), trans(0, 0, -600), rot_vec(0, 0, 0)
{
	focal_length = 150;
	camera_matrix = (Mat_<double>(3, 3) << focal_length, 0, center.x,
		0, focal_length, center.y, 0, 0, 1);
	dist_coeffs = Mat::zeros(4, 1, cv::DataType<double>::type);// info o znieksztalceniu obrazu przez soczewke itd. - brak
	rot_mat = Mat::eye(3, 3, CV_32FC1); // macierz rotacji

	for (auto& x : band)//generowaie wierzcholkow kostki w 3d
		for (auto y : band)
			for (auto z : band)
				points_3d.push_back(Vec3f(x, y, z));
	for (int id = 0; id<3; id++)// generowanie wektorow ideksow wierzcholkow dla kazdej ze scian
		for (auto b : band) {
			vector <size_t> temp;
			for (size_t i = 0; i<points_3d.size(); i++) {
				if (points_3d[i][id] == b)
					temp.push_back(i);
			}
			id_img_points.push_back(temp);
		}
	corner_points = vector<Vec2f>({ Vec2f(0,0),Vec2f(images[0].cols,0),// wierzcholki obrazow
		Vec2f(0,images[0].rows),Vec2f(images[0].cols,images[0].rows) });
	
}
void Model::key_handling(int sign) {
	if (sign == 'p'&&trans[2]<-400)
		trans[2] += 2;
	else if (sign == 'l'&&trans[2]>-800)
		trans[2] -= 2;
	else if (sign == 'w')
		rot_vec[0] = 0.01;
	else if (sign == 'e')
		rot_vec[1] = 0.01;
	else if (sign == 'r')
		rot_vec[2] = 0.01;
	else if (sign == 'a')
		rot_vec[0] = -0.01;
	else if (sign == 's')
		rot_vec[1] = -0.01;
	else if (sign == 'd')
		rot_vec[2] = -0.01;
}
void Model::mul() {
	rot_mat = Mat();
	Rodrigues(rot_vec, rot_mat);
	vector<Vec3f> out(0);
	for (auto pt : points_3d)
		out.push_back(Vec3f(((float*)Mat(rot_mat*Mat(pt)).data)));
	points_3d = out;
	rot_vec = Vec3f(0, 0, 0);
}
void Model::mean() {
	vector<Vec3f> out(0);
	for (auto x : id_img_points) {
		Vec3f sum(0, 0, 0);
		for (auto id : x) {
			sum += points_3d[id];
		}
		sum /= 4;
		out.push_back(sum);
	}
	points_mean_3d = out;
}
void Model::visiable_walls() {
	projectPoints(points_3d, Vec3f(0, 0, 0), trans, camera_matrix, dist_coeffs, points_2d);
	projectPoints(points_mean_3d, Vec3f(0, 0, 0), trans, camera_matrix, dist_coeffs, points_mean_2d);
	indexes_good = vector<size_t>(0);// indeksy scian ktore bêd¹ wyœwietlane
	vector <size_t> indexes_of_id({ 0, 1, 2, 3, 4, 5 });// indeksy scian
	sort(indexes_of_id.begin(), indexes_of_id.end(),//sortowanie indeksow scian na podstawie skladowej Z srodkow tych scian
		[this](size_t a, size_t b) {return points_mean_3d[a][2]>points_mean_3d[b][2]; });
	id_img_points_good.clear();
	//Wybieranie scian do wyswietlenia tak ze srodki tych scian na obrazie 2d nie zawieraja sie
	//wewnatrz blizszej sciany( tj. takiej dla ktorej skladowa Z srodka ma wieksza wartosc)
	for (auto id : indexes_of_id) {
		bool if_internal = false;
		for (auto pts : id_img_points_good) {
			vector <Vec2f> area(4), area2(0);
			generate(area.begin(), area.end(), [pts, n = 0, this]()mutable{return points_2d[pts[n++]]; });
			convexHull(area, area2);
			if (pointPolygonTest(area2, points_mean_2d[id], false) >= 0)
				if_internal = true;
		}
		if (if_internal == false) {
			id_img_points_good.push_back(id_img_points[id]);
			indexes_good.push_back(id);
		}
	}
}
void Model::dysplay(Mat &imgout) {
	Mat img(400, 400, CV_8UC3, Scalar(0, 0, 0));
	for (auto id : indexes_good) {// wyswietlanie widocznych obrazow
		vector <Vec2f> wall_points(4);
		generate(wall_points.begin(), wall_points.end(),//generowanie punktow 2d
			[id, n = 0, this]()mutable{return points_2d[id_img_points[id][n++]] + Vec2f(200, 200); });
		Mat mat_transf = getPerspectiveTransform(corner_points, wall_points);//obliczanie macierzy transformacji
		Mat temp;
		warpPerspective(images[id], temp, mat_transf, Size(img.rows, img.cols));//transformacja obrazu
		add(temp, img, img);
	}
	img.copyTo(imgout);
}
void Model::myvconnect(Mat &img1, Mat &img2, Mat &imgout) {
	namedWindow("okno", WINDOW_NORMAL);
	Mat img1copy, img2copy;
	img1.copyTo(img1copy);
	img2.copyTo(img2copy);

	unsigned int sg = 0;
	img1copy(Rect(0, 0, img1copy.cols - 40, img1copy.rows - 1)).copyTo(img1copy);
	img2copy(Rect(1, 0, img2copy.cols - 1, img2copy.rows - 1)).copyTo(img2copy);
	hconcat(img1copy, img2copy, imgout);
}
Model::~Model() {
	destroyAllWindows();
}