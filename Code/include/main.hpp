//opencv libs
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//tesseract libs
#include <publictypes.h>
#include <baseapi.h>
#include <allheaders.h>

//cURL libs
//#include <curl/curl.h>
#include "../include/CURLWrapper.h"

//jansson lib
#include <jansson.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>

#include "../include/ccv.hpp"

struct Place{
	double longitude;
	double latitude;
	std::string place_icon;
};

static std::string buffer;

std::vector<CvRect > spotText(char* input_im);
std::vector<std::pair<char*, int> > performOcr(std::vector<CvRect > bounding_boxes, char* input_im, char* tess_data_path, char* language);
static int writer(char *data, size_t size, size_t nmemb, std::string *buffer);
std::vector<Place> nearbySearch(std::pair<double, double> location, std::string keyword);

