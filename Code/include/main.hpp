//opencv libs
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>

//tesseract libs
#include <publictypes.h>
#include <baseapi.h>
#include <allheaders.h>

//cURL libs
//#include <curl/curl.h>
#include "../include/CURLWrapper.h"

//jansson lib
#include <jansson.h>

//for converting jpg to png
#include "boost/gil/gil_all.hpp"
#include "boost/gil/extension/io/jpeg_io.hpp"
#include "boost/gil/extension/io/png_io.hpp"

//visualization
#include "../visualization/mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

//ubuntu bug
#include <pthread.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <exception>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>

#include <Eigen/Dense>
#include <Eigen/LU>
using Eigen::MatrixXd;

#include "../include/ccv.hpp"

struct Place{
	double longitude;
	double latitude;
	std::vector<std::string> place_icon;
	std::string name;
	int match_score;
};

struct Odometry{
	double dist;
	double theta1;
	double theta2;
};

struct Pose{
	MatrixXd mu;
	MatrixXd sigma;
};

struct Location{
	double x;
	double y;
};

std::string saved_logos_prefix = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/Logos/";
char* TESSDATA_PATH_ = "/usr/local/share/tessdata/";
char* DETECTION_LANGUAGE_ = "deu+deu-frak+eng";
char* CONFIG_FILE_ = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/tesseract-3.03/tessdata/tessconfigs/specialconfig";

static std::string buffer;

std::vector<CvRect > spotText(const char* input_im);
std::vector<std::pair<char*, int> > performOcr(std::vector<CvRect > bounding_boxes, const char* input_im);
static int writer(char *data, size_t size, size_t nmemb, std::string *buffer);
std::vector<Place> nearbySearch(std::pair<double, double> location, std::string keyword);
bool logoFound(char* logo_im, const char* input_im);
void savePlaceIcon(Place& place);
std::string getPhotoRef(std::string photo_ref);
std::string ocrCorrection(std::string query);
void run(char* input_im);
std::vector<std::string> getCombinations(std::vector<std::string> tokens);
void combinationRec(std::vector<std::string> &words, int max_len, int curr_size, int curr_start, std::string curr_word, std::vector<std::string> &result);
Place reverseSearch(const char* input_im, std::string search_word);
bool comparePlaces(const Place &a, const Place &b){
	return (a.match_score>b.match_score);
}
Odometry getOdom(std::pair<double, double> pt1, std::pair<double, double> pt2);
std::pair<double, double> getGPS(Pose robot_pose, std::pair<double, double> pt);
Pose motionModel(Pose curr_pose, Odometry motion);
double normalize_angle(double angle);
Pose correctionStep(Pose estim_pose, std::vector<Location> observs);
int runKalmanFilter();
