
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