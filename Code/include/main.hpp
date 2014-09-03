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

//ubuntu bug
#include <pthread.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "../include/ccv.hpp"

struct Place{
	double longitude;
	double latitude;
	std::vector<std::string> place_icon;
	std::string name;
};

std::string saved_logos_prefix = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/Logos/";
char* TESSDATA_PATH_ = "/usr/local/share/tessdata/";
char* DETECTION_LANGUAGE_ = "deu+deu-frak+eng";
char* CONFIG_FILE_ = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/tesseract-3.03/tessdata/tessconfigs/specialconfig";

static std::string buffer;

std::vector<CvRect > spotText(char* input_im);
std::vector<std::pair<char*, int> > performOcr(std::vector<CvRect > bounding_boxes, char* input_im);
static int writer(char *data, size_t size, size_t nmemb, std::string *buffer);
std::vector<Place> nearbySearch(std::pair<double, double> location, std::string keyword);
bool logoFound(char* logo_im, char* input_im);
void savePlaceIcon(Place& place);
std::string getPhotoRef(std::string photo_ref);
std::string ocrCorrection(std::string query);
void run(char* input_im);
std::vector<std::string> getCombinations(std::vector<std::string> tokens);
void combinationRec(std::vector<std::string> &words, int max_len, int curr_size, int curr_start, std::string curr_word, std::vector<std::string> &result);
