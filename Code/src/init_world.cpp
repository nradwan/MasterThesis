#include "../include/init_world.hpp"

World::World(std::string fldr, std::string file){
	init(fldr, file);
}

World::World(){
	
}


World::~World(){

}

void World::init(std::string fldr, std::string file){
	
	DATAFOLDER = fldr;//"/home/radwann/Documents/MultipleLandmarkDataset/";
	WORLD = file;//"world.json";
	
	loadMap();
}

void World::loadMap(){
	//reads the world.json file and inserts the data into a map
	json_t* root = json_object();
	json_error_t error;
	
	std::string file_name = DATAFOLDER + WORLD;
	std::ifstream ifs(file_name.c_str());
	std::string map_source ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	//load the json file
	//root = json_load_file(file_name.c_str(), 0, &error);
	root = json_loads(map_source.c_str(), 0, &error);
	if(!root){
		std::cout << "ERROR at line " << error.line << ": " << error.text << std::endl;
		return;
	}
	json_t* map = json_object();
	//extract the map object
	map = json_object_get(root, "map");
	if(!json_is_array(map)){
		std::cout << "ERROR: map is not an array" << std::endl;
		json_decref(root);
		return;
	}
	//loop over the map array
	
	for(size_t i = 0; i < json_array_size(map); i++){
		json_t *data = json_object();
		json_t *name = json_object();
		json_t *location = json_object();
		json_t *lat = json_object();
		json_t *lng = json_object();
		
		std::string shop_name;
		std::pair<double, double> shop_loc;
		data = json_array_get(map, i);
		//get the name
		name = json_object_get(data, "name");
		if(json_is_string(name)){
			shop_name = json_string_value(name);
		}
		//get the location
		location = json_object_get(data, "location");
		lat = json_object_get(location, "latitude");
		lng = json_object_get(location, "longitude");
		shop_loc.first = json_real_value(lat);
		shop_loc.second = json_real_value(lng);
		
		//push to the map
		shops_coords.insert(std::pair<std::string, std::pair<double, double> >(shop_name, shop_loc));
		//push to the vector
		shop_names.push_back(shop_name);
		
		json_decref(data);
		json_decref(name);
		json_decref(location);
		json_decref(lat);
		json_decref(lng);
	}
	json_decref(map);
	json_decref(root);
}
