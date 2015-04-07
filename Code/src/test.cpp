#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
//jansson lib
#include <jansson.h>
//serialization
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

char* DATA_TRUE = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/Evaluation_Dataset/odom.dat";
char* DATA_ESTIM = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/Evaluation_Dataset/results_narrowed.txt";

int main(){
	//open the data file
	std::ifstream data_true_file (DATA_TRUE);
	std::ifstream data_estim_file (DATA_ESTIM);
	//
	return 1;
}

/*char* TMP_MAP = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/Evaluation_Dataset/template.html";
char* MAP_TRUE = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/Evaluation_Dataset/groundtruth1.html";
char* MAP_ESTIM = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/Evaluation_Dataset/estim1.html";
char* DATA_TRUE = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/Evaluation_Dataset/odom.dat";
char* DATA_ESTIM = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/Evaluation_Dataset/results_narrowed.txt";

bool replace(std::string& str, const std::string& from, const std::string& to) 
{
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) 
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

int main(){

	std::string data_true_line, data_estim_line;
	//open the data files to read
	std::ifstream data_true_file (DATA_TRUE);
	std::ifstream data_estim_file (DATA_ESTIM);
	if(data_true_file.is_open() && data_estim_file.is_open()){
		//get the centre of the data file
		std::getline(data_true_file, data_true_line);
		std::stringstream str_stream;
		str_stream << data_true_line;
		int num_landmarks;
		str_stream >> num_landmarks;
		double lat, lng;
		str_stream >> lat;
		str_stream >> lng;
		//create json_t object to store the centre
		json_t *lat_obj, *lng_obj;
		lat_obj = json_real(lat);
		lng_obj = json_real(lng);
		json_t *center_obj = json_object();
		json_object_set(center_obj, "lat", lat_obj);
		json_object_set(center_obj, "lng", lng_obj);
		std::string center_replace = json_dumps(center_obj, JSON_PRESERVE_ORDER);
		replaceAll(center_replace, "\"", "");
		
		//loop over the remaining lines to get the data
		int num_lines = 0;
		//create json_t arrays
		json_t *loc_array_true = json_array();
		json_t *loc_array_estim = json_array();
		json_t *loc_lat_true, *loc_lng_true, *loc_obj_true, *loc_latlng_true;
		json_t *loc_lat_estim, *loc_lng_estim, *loc_obj_estim, *loc_latlng_estim;
		while(num_lines < 12 && std::getline(data_true_file, data_true_line) 
				&& std::getline(data_estim_file, data_estim_line)){
				
			std::cout << "data: " << data_true_line << std::endl;
			std::cout << "estim: " << data_estim_line << std::endl;
			std::stringstream true_stream, estim_stream;
			//read the true datastream first
			true_stream << data_true_line;
			true_stream >> lat;
			true_stream >> lng;
			std::cout << "lat: " << lat << " lng: " << lng << std::endl;
			//push to the json_t objs
			loc_lat_true = json_real(lat);
			loc_lng_true = json_real(lng);
			std::cout << "1" << std::endl;
			loc_latlng_true = json_object();
			json_object_set(loc_latlng_true, "lat", loc_lat_true);
			json_object_set(loc_latlng_true, "lng", loc_lng_true);
			std::cout << "2" << std::endl;
			loc_obj_true = json_object();
			json_object_set(loc_obj_true, "latLng", loc_latlng_true);
			std::cout << "3" << std::endl;
			json_array_append(loc_array_true, loc_obj_true);
			std::cout << "4" << std::endl;
			json_decref(loc_obj_true);
			std::cout << "finished first datastream" << std::endl;
			//read the estim datasetream
			estim_stream << data_estim_line;
			estim_stream >> lat;
			estim_stream >> lng;
			std::cout << "lat: " << lat << " lng: " << lng << std::endl;
			//push to the json_t objs
			loc_lat_estim = json_real(lat);
			loc_lng_estim = json_real(lng);
			std::cout << "1" << std::endl;
			loc_latlng_estim = json_object();
			json_object_set(loc_latlng_estim, "lat", loc_lat_estim);
			json_object_set(loc_latlng_estim, "lng", loc_lng_estim);
			std::cout << "2" << std::endl;
			loc_obj_estim = json_object();
			json_object_set(loc_obj_estim, "latLng", loc_latlng_estim);
			std::cout << "3" << std::endl;
			json_array_append(loc_array_estim, loc_obj_estim);
			std::cout << "4" << std::endl;
			json_decref(loc_obj_estim);
			std::cout << "second datasetream finished" << std::endl;
			//increment the line num
			num_lines ++;		
			std::cout << "iteration num: " << num_lines << std::endl;
		}
		std::string locs_replace_true = json_dumps(loc_array_true, JSON_PRESERVE_ORDER);
		replaceAll(locs_replace_true, "\"", "");
		std::string locs_replace_estim = json_dumps(loc_array_estim, JSON_PRESERVE_ORDER);
		replaceAll(locs_replace_estim, "\"", "");
		
		std::cout << locs_replace_true << std::endl;
		std::cout << locs_replace_estim << std::endl;
		//open the template file
		std::ifstream templ_file (TMP_MAP);
		//open the groundtruth map file
		std::ofstream map_true_file;
		map_true_file.open(MAP_TRUE);
		//open the estimated map file
		std::ofstream map_estim_file;
		map_estim_file.open(MAP_ESTIM);
		//write to files
		std::string templ_line;
		if(templ_file.is_open() && map_true_file.is_open()
			&& map_estim_file.is_open()){
			while(std::getline(templ_file, templ_line)){
				std::string line_repl = templ_line;
				//fill in the data
				replace(templ_line, "$center", center_replace);
				replace(templ_line, "$loc_vec", locs_replace_true);
				//write the line to the map file
				map_true_file << templ_line << "\n";
				//fill in the data
				replace(line_repl, "$center", center_replace);
				replace(line_repl, "$loc_vec", locs_replace_estim);
				//write the line to the map file
				map_estim_file << line_repl << "\n";
			}	
		}
		templ_file.close();
		map_true_file.close();
		map_estim_file.close();
	}
	data_true_file.close();
	data_estim_file.close();
	return 0;
}*/
