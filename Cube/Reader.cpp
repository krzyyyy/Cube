
#include "stdafx.h"
#include "Reader.h"
#include "Exceptions.h"
#include <memory>

char tab[] = "<?xml version=\"1.0\" encoding=\"utf - 8\"?>"
"<configuration>"
"<Photo graphicFilter=\"thresholding\">images/1.png</Photo>"
"<Photo>images/2.png</Photo>"
"<Photo>images/3.png</Photo>"
"<Photo>images/4.png</Photo>"
"<Photo>images/5.png</Photo>"
"<Photo>images/6.png</Photo>"
"</configuration>";
Reader::Reader(): linefile(0) {
}
Reader::Reader(string path) : linefile(0) {
	confpath = path;
	file.open(confpath, ios::in | ios::binary);
	if (!file.good())
		throw ConfigurationFileException(confpath);
}

Reader* Reader::open(string path) {
	if (path.substr(path.length() - 4) == ".txt") {
		unique_ptr<ReaderTXT> reader(new ReaderTXT(path));
		return new ReaderTXT(path);
	}
	else if (path.substr(path.length() - 4) == ".xml") {
		unique_ptr<ReaderXML> reader(new ReaderXML(path));
		return new ReaderXML(path);

		/*rapidxml::xml_node<>*nodemaster = xmlfile.first_node();
		for (rapidxml::xml_node<>* photo = nodemaster->first_node(); photo; photo = photo->next_sibling()) {
			string path = photo->value();
			string mode = "";
			if (photo->first_attribute())
				mode = photo->first_attribute()->value();
			Mat temp = imread(path);
			if (temp.empty()) {
				throw ImageFileException(linefile, path);
			}
			else if (mode == "gauss")
				gauss(temp, temp);
			else if (mode == "thresholding")
				thresholding(temp, temp);
			else if (mode != "")
				throw ImageModeException(linefile, mode, path);
		}*/
	}

}



Reader::~Reader() {
	file.close();
}
void Reader::gauss(Mat &imgIn, Mat &imgOut) {
	imgIn.copyTo(imgOut);
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


}
void Reader::thresholding(Mat &imgIn, Mat &imgOut) {
	imgIn.copyTo(imgOut);
	//imgOut = Mat(imgIn.cols, imgIn.rows, imgIn.type(), Scalar(0, 0, 0));
	switch (imgIn.channels())
	{
	case 1:
	{
		for (int i = 0; i<imgIn.rows; i++)
			for (int j = 0; j < imgIn.cols; j++) {
				int pix = 0;
				if (imgIn.at<uchar>(i, j) <= 128)
					pix = 0;
				else
					pix = 255;
				imgOut.at<uchar>(i, j) = pix;
			}

		break;
	}
	case 3:
	{
		Mat_<Vec3b> _I = imgIn;
		for (int i = 0; i<imgIn.rows; i++)
			for (int j = 0; j < imgIn.cols; j++) {
				for (int idx = 0; idx < 3; idx++) {
					double pix = 0;
					if (_I(i, j)[idx] <= 80)
						pix = 0;
					else
						pix = 255;
					_I(i, j)[idx] = pix;
				}
			}
		imgOut = _I;
		break;
	}
	}
}
bool Reader::makeFoto(vector <Mat> &images) {
	VideoCapture cap(0);
	if (!cap.isOpened())
		return false;
	Mat frame;
	while (true) {
		cap >> frame;
		imshow("foto", frame);
		if (waitKey(10) == 'f')
			break;

	}
	images.push_back(frame);
	return true;
}
void ReaderXML::load(vector <Mat>& images) {
	//print(cout, xmlfile, 0);
	//using namespace::rapidxml;
	rapidxml::xml_node<>*nodemaster = xmlfile.first_node();
	for (rapidxml::xml_node<>* photo = nodemaster->first_node(); photo; photo = photo->next_sibling()) {
		string path = photo->value();
		string mode = "";
		if (photo->first_attribute())
			mode = photo->first_attribute()->value();
		Mat temp = imread(path);
		if (temp.empty()) {
			throw ImageFileException(linefile, path);
		}
		else if (mode == "gauss")
			gauss(temp, temp);
		else if (mode == "thresholding")
			thresholding(temp, temp);
		else if (mode != "")
			throw ImageModeException(linefile, mode, path);
		images.push_back(temp);
	}
	if (images.size() < 6)
		throw TooShortConfigException(confpath, linefile);
}
ReaderXML::ReaderXML() {}
ReaderXML::ReaderXML(string path):Reader(path) {
	buffer = vector<char>((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	try
	{
		xmlfile.parse<0>(&buffer[0]);
	}
	catch (const rapidxml::parse_error & e)
	{
		std::cerr << e.what() << " here: " << e.where < char >() << std::endl;
		return;
	}
	print(cout, xmlfile, 0);
}

void ReaderTXT::load(vector <Mat>& images) {
	while (!file.eof()) {
		linefile++;
		string line, path, mode;
		getline(file, line);
		stringstream ss(line);
		ss >> path >> mode;
		Mat temp = imread(path);
		if (temp.empty()) {
			throw ImageFileException(linefile, path);
		}
		else if (mode == "gauss")
			gauss(temp, temp);
		else if (mode == "thresholding")
			thresholding(temp, temp);
		else if (mode != "")
			throw ImageModeException(linefile, mode, path);
		images.push_back(temp);
	}
	if (images.size() < 6)
		throw TooShortConfigException(confpath, linefile);
}
ReaderTXT::ReaderTXT(string path):Reader(path) {
}
ReaderTXT::ReaderTXT() {}
