//opencv libs
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//tesseract libs
#include <publictypes.h>
#include <baseapi.h>
#include <allheaders.h>

//cURL libs
#include <curl/curl.h>

#include <iostream>
#include <vector>

#include "../include/ccv.hpp"

std::vector<CvRect > spotText(char* input_im);
std::vector<std::pair<char*, int> > performOcr(std::vector<CvRect > bounding_boxes, char* input_im, char* tess_data_path, char* language);
