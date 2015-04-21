#ifndef VLALGR_H
#define VLALGR_H

#include <iostream>
#include <map>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <queue>

//jansson lib
#include <jansson.h>

//ccv lib
#include "../include/ccv.hpp"

//tesseract libs
#include <publictypes.h>
#include <baseapi.h>
#include <allheaders.h>

//opencv libs
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>

//Eigen libs
#include <Eigen/Dense>
#include <Eigen/LU>

//cURL libs
//#include <curl/curl.h>
#include "../include/CURLWrapper.h"

//LM computation
#include "../include/LM_algorithm.hpp"
//World initialization
#include "../include/init_world.hpp"

static std::string buffer;

static int writer(char *data, size_t size, size_t nmemb, std::string *buffer);

class VLAlgr{
	public:
	
		struct Match{
			std::string shop_name;
			int score;
		};

		struct CompareMatch{
			bool operator()(Match const &m1, Match const &m2){
				return m1.score > m2.score;
			}
		};

		struct Pose{
			double lat;
			double lng;	
		};

		struct Step{
			Pose pose;
			std::vector<double> obsr_angles;
			std::vector<std::string> images;
		};
	
		VLAlgr();
		~VLAlgr();
		void evaluateResults();
		void localize();
		void dummyLocalize();
		
		std::vector<Step> data_steps;
	
	//protected:
		
	
	private:
		void init();
		void initializeData();
		std::vector<CvRect > spotText(const char* input_im);
		std::vector<std::pair< std::pair<char*, int>, std::pair<int, int> > > performOcr(std::vector<CvRect > bounding_boxes, const char* input_im);
		std::string processText(std::vector<std::pair< std::pair<char*, int>, std::pair<int, int> > > detection);
		std::string ocrCorrection(std::string query);
		bool replace(std::string& str, const std::string& from, const std::string& to);
		void replaceAll(std::string& str, const std::string& from, const std::string& to);
		std::string removeNonChars(std::string& str);
		std::vector<std::string> getNBestMatches(std::string text);
		int editDistance(std::string s, std::string t);
		Pose improvePose(Step step, std::map<int, std::vector<std::string> > landmark_angle_idx);
		int countLandmarks(std::map<int, std::vector<std::string> > landmark_angle_idx);
		void printMatches(std::vector<std::string> matches, std::string image);
		void printPQ(std::vector<Match> x);
		
		std::string DATAFOLDER;
		std::string COORDFILE;
		std::string WORLD;
		char* TESSDATA_PATH_;
		char* DETECTION_LANGUAGE_;
		World* world_map;
		
		int max_num_matches;
		std::vector<Pose> final_poses;
		std::vector<Pose> ground_truth;
};

#endif
