#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

//jansson lib
#include <jansson.h>

int main(int argc, char** argv){
	std::string input_file_name = "/home/radwann/Documents/MultipleLandmarkDataset/trial1/mapFromXML.json";
	std::string output_file_name = "/home/radwann/Documents/MultipleLandmarkDataset/trial1/formattedMap.json";
	//std::ifstream ifs(input_file_name.c_str());
	std::ofstream ofs;
	ofs.open(output_file_name.c_str());
	//std::string map_source ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	//load the json file
	json_t *root; 
	json_error_t error;
	root = json_load_file(input_file_name.c_str(), 0, &error);
	if(!root){
		std::cout << "ERROR at line " << error.line << ": " << error.text << std::endl;
		return 1;
	}
	
	//create map array object
	json_t *map_array;
	map_array = json_array();
	
	json_t *osm, *node;
	//extract the node object from osm
	osm = json_object_get(root, "osm");
	node = json_object_get(osm, "node");
	//loop over the node array
	
	for(size_t i = 0; i < json_array_size(node); i++){
		json_t *data, *lon, *lat, *tag;
		
		data = json_array_get(node, i);
		//get the geo information
		lon = json_object_get(data, "@lon");
		lat = json_object_get(data, "@lat");
		//check for tag object
		tag = json_object_get(data, "tag");
		if(!json_is_array(tag))
			continue;
		else{
			for(size_t j = 0; j < json_array_size(tag); j++){
				json_t *inner_data, *k, *v;
				inner_data = json_array_get(tag, j);
				//get the k value
				k = json_object_get(inner_data, "@k");
				std::string k_value = json_string_value(k);
				if(k_value.compare("name") == 0){
					v = json_object_get(inner_data, "@v");
					std::string shop_name = json_string_value(v);
					double lon_value = atof(json_string_value(lon));
					double lat_value = atof(json_string_value(lat));
					json_t *tmp = json_object();
					json_object_set_new(tmp, "name", json_string(shop_name.c_str()));
					json_t *loc = json_object();
					json_object_set_new(loc, "latitude", json_real(lat_value));
					json_object_set_new(loc, "longitude", json_real(lon_value));
					json_object_set_new(tmp, "location", loc);
					json_array_append_new(map_array, tmp);
					json_decref(tmp);
					json_decref(loc);
				}
				json_decref(inner_data);
				json_decref(k);
				json_decref(v);
			}
		}
		json_decref(data);
		json_decref(lon);
		json_decref(lat);
		json_decref(tag);
	}
	
	json_t *new_root = json_object();
	json_object_set_new(new_root, "map", map_array);
	
	std::string s = json_dumps(new_root, JSON_INDENT(3));
	ofs << s;
	
	json_decref(new_root);
	json_decref(map_array);
	json_decref(root);
	json_decref(osm);
	json_decref(node);
	
	return 0;
}
