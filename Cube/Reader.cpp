
#include "stdafx.h"
#include "Reader.h"
#include "Exceptions.h"
#include "TraceLogger.h"
#include <memory>

Reader::Reader(): linefile(0) {
	int g = 0;
}
Reader::Reader(string path) : linefile(0) {
	
	confpath = path;
	file.open(confpath, ios::in | ios::binary);
	if (!file.good())
		throw ConfigurationFileException(confpath);
	TraceLogger::complite();
}
Reader::Reader(Reader& rd) {
	this->confpath = rd.confpath;
	this->linefile = rd.linefile;
	rd.file.close();
	this->file.open(rd.confpath);
}
void Reader::open(string path, unique_ptr<Reader> &ptr) {
	TraceLogger::addLog("Openning configuration file... ");
	if (path.substr(path.length() - 4) == ".txt") {
		ptr = move(make_unique<ReaderTXT>(ReaderTXT(path)));
		//return unique_ptr<Reader>(make_unique<ReaderTXT>(ReaderTXT(path)));
	}
	else if (path.substr(path.length() - 4) == ".xml") {
		ptr = make_unique<ReaderXML>(ReaderXML(path));
	}
	else {
		throw ConfigurationFileException(path);
	}
}
void Reader::writeLog(string log, string type) {
	if (!log.empty()) {
		cout << "\n-------------------------------\n";
		cout << "Where do you want to write "+ type+" log?\n1. console\n2. file "+type+"log.txt\n";
		int choice = 0;
		cin >> choice;
		if (choice == 1) {
			cout << log;
		}
		else if (choice == 2) {
			fstream filet(type+"log.txt", ios::out);
			if (filet.good())
			{
				filet << log;
				filet.close();
			}
		}
		else {
			cout << "There is no such choise\n";
		}
	}
}
Reader& Reader::operator=(Reader& rd) {
	this->confpath = rd.confpath;
	this->linefile = rd.linefile;
	rd.file.close();
	this->file.open(rd.confpath);
	return *this;
}

Reader::~Reader() {
	if (TraceLogger::getSize() > 1) {
		writeLog(TraceLogger::toString(), "trace");
		writeLog(ErrorLoger::getLog(), "error");
	}
	if(file)
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
	cout << "nacisnij f aby zrobic zdjecie\n";
	while (true) {
		cap >> frame;
		imshow("foto", frame);
		if (waitKey(10) == 'f')
			break;

	}
	destroyAllWindows();
	images.push_back(frame);
	return true;
}
void ReaderXML::load(vector <Mat>& images) {
		//while(*currentphoto.get()) {
		while (currentphoto) {
		//string path = ((*currentphoto.get())->value());
		string path = currentphoto->value();
		string mode = "";
		TraceLogger::addLog("Openning image named: " +path+" ... ");
		//if ((*currentphoto.get())->first_attribute())
			//mode = (*currentphoto.get())->first_attribute()->value();
		if (currentphoto->first_attribute())
			mode = currentphoto->first_attribute()->value();
		//currentphoto = make_unique<rapidxml::xml_node<>*>((*currentphoto.get())->next_sibling());
		currentphoto = currentphoto->next_sibling();
		linefile++;
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
		TraceLogger::complite();
	}
	if (images.size() < 6)
		throw TooShortConfigException(confpath, linefile);
}
ReaderXML::ReaderXML():Reader() {}
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
	//this->currentphoto= make_unique<rapidxml::xml_node<>*>( xmlfile.first_node()->first_node());
	currentphoto = xmlfile.first_node()->first_node();
	print(cout, xmlfile, 0);
	linefile++;
}
ReaderXML::ReaderXML(ReaderXML &rd):Reader(rd) {
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
	//this->currentphoto =make_unique<rapidxml::xml_node<>*>( this->xmlfile.first_node()->first_node());
	currentphoto = xmlfile.first_node()->first_node();
	print(cout, xmlfile, 0);
	linefile++;
}
ReaderXML& ReaderXML:: operator= (ReaderXML& rd) {
	this->buffer = rd.buffer;
	try
	{
		this->xmlfile.parse<0>(&buffer[0]);
	}
	catch (const rapidxml::parse_error & e)
	{
		std::cerr << e.what() << " here: " << e.where < char >() << std::endl;
		return *this;
	}
	//this->currentphoto = this->xmlfile.first_node()->first_node();
	print(cout, xmlfile, 0);
	linefile++;
	return *this;
}

void ReaderTXT::load(vector <Mat>& images) {
	while (!file.eof()) {
		linefile++;
		string line, path, mode;
		getline(file, line);
		stringstream ss(line);
		ss >> path >> mode;
		TraceLogger::addLog("Openning image named: " + path + " ... ");
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
		TraceLogger::complite();
	}
	if (images.size() < 6)
		throw TooShortConfigException(confpath, images.size());
}
ReaderTXT::ReaderTXT(string path):Reader(path) {
}
ReaderTXT::ReaderTXT():Reader() {}
ReaderTXT::ReaderTXT(ReaderTXT &rd):Reader(rd) {
}
