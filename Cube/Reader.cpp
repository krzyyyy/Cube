
#include "stdafx.h"
#include "Reader.h"

Reader::Reader() {
}
Reader::Reader(string path) {
	file.open(path, ios::in);
	if (!file.good()) 
		throw "configurationFileException";
}
void Reader::load(vector <Mat>& images) {
	images.clear();
	while (!file.eof()) {
		string line, path, mode;
		getline(file, line);
		stringstream ss(line);
		ss >> path >> mode;
		Mat temp = imread(path);
		gaus(temp, temp);
		if (temp.empty())
			throw "imageFileException";
		images.push_back(temp);

	}
}
Reader::~Reader() {
	file.close();
}
void Reader::gaus(Mat &imgIn, Mat &imgOut) {
	imgIn.copyTo(imgOut);
	namedWindow("o", WINDOW_NORMAL);
	Mat temp(imgIn.rows + 2, imgIn.cols + 2, imgIn.type());
	imgIn.copyTo(temp(Rect(1, 1, imgIn.cols, imgIn.rows)));
	Mat mask = (Mat_<double>(3, 3) << 1, 2, 1, 2, 4, 2, 1, 2, 1);
	
	switch (imgIn.channels())
	{
	case 1:
	{
		for(int i=0;i<imgIn.rows;i++)
			for (int j = 0; j < imgIn.cols; j++) {
				double sum = 0;
				for(int k=-1;k<2;k++)
					for (int l = -1; l < 2; l++) {
						sum += temp.at<uchar>(i + 1 + k, j + 1 + l)*mask.at<double>(k+1,l+1);
					}
				sum /= 16;
				imgOut.at<uchar>(i, j) = sum;
				}

		break;
	}
	case 3:
	{
		Mat_<Vec3b> _I = imgIn;
		Mat_<Vec3b> _Itemp = temp;
		for (int i = 0; i<imgIn.rows; i++)
			for (int j = 0; j < imgIn.cols; j++) {
				double sum1 = 0, sum2 = 0, sum3 = 0;
				for (int k = -1; k < 2; k++)
					for (int l = -1; l < 2; l++) {
						sum1 += _Itemp(i + 1 + k, j + 1 + l)[0] * mask.at<double>(k + 1, l + 1);
						sum2 += _Itemp(i + 1 + k, j + 1 + l)[1] * mask.at<double>(k + 1, l + 1);
						sum3 += _Itemp(i + 1 + k, j + 1 + l)[2] * mask.at<double>(k + 1, l + 1);

					}
				sum1 /= 16;
				sum2 /= 16;
				sum3 /= 16;
				_I(i, j)[0] = sum1;
				_I(i, j)[1] = sum2;
				_I(i, j)[2] = sum3;
			}
		imgOut = _I;
		break;
	}
	}

	imshow("o", imgOut);
	waitKey(0);

}
