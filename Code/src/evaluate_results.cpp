#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath> 
#include <boost/algorithm/string.hpp>
#include <string>
#include <stdlib.h>
#include <vector>
#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0

char* DATA_FILE_ = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/Evaluation_Dataset/DifficultDataset.txt";
char* RESULTS_FILE_ = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/Evaluation_Dataset/DifficultDatasetResults.txt";

// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / pi);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

double getDistance(std::pair<double, double> pt1, std::pair<double, double> pt2){
	return distanceEarth(pt1.first, pt1.second, pt2.first, pt2.second);
}

double getAccuracy(){
	//open both files
	std::ifstream data_file, results_file;
	data_file.open(DATA_FILE_, std::ios::in);
	results_file.open(RESULTS_FILE_, std::ios::in);
	
	double num_landmarks;
	double dist_sum = 0;
	double dist_average = 0;
	if(data_file.is_open() && results_file.is_open()){
		std::string data_line, results_line;
		//skip the first line of the data file
		std::getline(data_file, data_line);
		std::stringstream ss (data_line);
		ss >> num_landmarks;
		//std::cout << "landmarks: " << num_landmarks << std::endl;
		//loop over the lines of both files
		while(std::getline(data_file, data_line) && std::getline(results_file, results_line)){
			std::stringstream data_stream (data_line);
			std::stringstream results_stream (results_line);
			std::pair<double, double> data_loc;
			std::pair<double, double> results_loc;
			data_stream >> data_loc.first;
			data_stream >> data_loc.second;
			results_stream >> results_loc.first;
			results_stream >> results_loc.second;
			double dist = getDistance(data_loc, results_loc);
			if(dist > 1.0){
				std::cout << "d line: " << data_line << "r line: " << results_line << std::endl;
				std::cout << "dist: " << dist << std::endl;
			}
			//std::cout << "distance: " << dist << std::endl;
			dist_sum += dist;
		}
	}
	std::cout << "dist sum: " << dist_sum << std::endl;
	dist_average = dist_sum / num_landmarks;
	return dist_average;
}

double getCorrectMatches(){
	std::ifstream results_file, data_file;
	results_file.open(RESULTS_FILE_, std::ios::in);
	data_file.open(DATA_FILE_, std::ios::in);
	
	double num_landmarks;
	double sum = 0;
	if(results_file.is_open() && data_file.is_open()){
		std::string results_line, data_line;
		std::getline(data_file, data_line);
		std::stringstream data_stream (data_line);
		data_stream >> num_landmarks;
		while(std::getline(results_file, results_line)){
			//std::cout << "results_line: " << results_line << std::endl;
			std::vector<std::string> results_str;
			boost::split(results_str, results_line, boost::is_any_of(" "));
			double correct = atof(results_str.back().c_str());
			//std::cout << correct << std::endl;
			sum += correct;
		}
	}
	//std::cout << sum << std::endl;
	return sum/num_landmarks;
}

int main(int argc, char** argv){
	
	double score = getAccuracy();
	std::cout << "matching score: " << score << " Km" << std::endl;
	
	double correct_matches = getCorrectMatches();
	std::cout << "percentage of correct matches: " << correct_matches * 100 << std::endl;
	
	return 0;
}
