#ifndef INITWORLD_H
#define INITWORLD_H

#include <iostream>
#include <map>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <vector>

//jansson lib
#include <jansson.h>

class World{
	public:
		World(std::string fldr, std::string file);
		~World();
		World();
		
		std::map<std::string, std::pair<double, double> > shops_coords;
		std::vector<std::string> shop_names;
	private:
		void init(std::string fldr, std::string file);
		void loadMap();
		
		std::string DATAFOLDER; 
		std::string WORLD;
};

#endif
