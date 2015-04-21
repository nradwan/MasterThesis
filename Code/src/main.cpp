#include "../include/main.hpp"
#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0
using namespace boost::gil;

int main(int argc, char** argv)
{
	std::vector<CvRect > recog_text = spotText(argv[1]);
	std::vector<std::pair<char*, int > > detection = performOcr(recog_text, argv[1]);
	for(std::vector<std::pair<char*, int > >::iterator it = detection.begin(); it!=detection.end(); ++it){
		std::cout << "text: " << it->first << " ,confidence: " << it->second << std::endl;
	}
	
	return 0;
	
	/*double latit = atof(argv[1]);
	double longit = atof(argv[2]);
	std::pair<double, double > location (latit, longit);
	std::vector<Place > results = nearbySearch(location, argv[3]);
	std::cout << "returned results size: " << results.size() << std::endl;
	for(std::vector<Place >::iterator it = results.begin(); it!=results.end(); ++it){
		std::cout << it->name << ": " << it->longitude << "," << it->latitude << std::endl;
	}
	return 0;*/
	
	/*std::cout << logoFound(argv[1], argv[2]) << std::endl;
	return 0;*/
	
	/*Place p;
	p.name = "Parianos Reisen - Griechenland aus erster Hand";
	std::string x = "https://lh6.googleusercontent.com/--6U-bNF7ua8/UlLeto7pCxI/AAAAAAAAMWA/UK9hfha-Bk8/s1600-w145/photo.jpg";//getPhotoRef("CnRmAAAAwC8bEPLw3sACDGvDqRU0ZycG2hq7PoTcmo33FC0B3c5qaHN0EkRZvfsXf7IojVZVOvR56s_1iBJJyUt8tD0Gz8uiT9IrK0w19vpoDvoXzIHxLTaxLFkaZ0-1Qy5CIO7druFe6FZGFC9xNEO588Wi5hIQt7hVB6OzKd8cusZUlD0NMBoUim0UyMHN7gSLrBp__TpIJBMOxYQ");
	std::cout << x << std::endl;
	std::vector<std::string> pl;
	pl.push_back(x);
	p.place_icon = pl;
	savePlaceIcon(p);
	for(std::vector<std::string>::iterator it= p.place_icon.begin(); it!=p.place_icon.end(); ++it){
		std::cout << *it << std::endl;
	}*/
	

	/*std::cout << ocrCorrection(argv[1]) << std::endl;
	return 0;*/
	
	//run(argv[1]);
	/*std::vector<std::string> elems;
	elems.push_back("1");
	elems.push_back("2");
	elems.push_back("3");
	elems.push_back("4");
	elems.push_back("5");
	std::vector<std::string> result = getCombinations(elems);
	for(std::vector<std::string>::iterator it=result.begin(); it!=result.end(); ++it)
		std::cout << *it << std::endl;*/

	//reverseSearch(argv[1], argv[2]);
	
	/*MatrixXd m;
	m = MatrixXd::Zero(3,3);
	m << 1, 2, 3,
	4, 5, 6,
	7, 8, 9;
	
	m.conservativeResize(5,5);
	std::cout << m << std::endl;*/
	
	/*if(argc >= 2){
		std::string param = argv[1];
		if(param.compare("-c") == 0){
			USE_CACHED_DATA_ = true;
			//load the data
			//std::ifstream file (TEXT_CORR_DATA_);
			//if(file){
				// Read json.
				//std::istringstream is;
				//std::string line;
				//std::string all_lines = "";
				//while(std::getline(file, line))
					//all_lines += line + "\n";
				//is.str(all_lines);
				//read_json (is, pt_text);
			//}
			std::ifstream nearby_file (NEARBY_DATA_);
			if(nearby_file){
				// Read json.
				std::istringstream is;
				std::string line;
				std::string all_lines = "";
				while(std::getline(nearby_file, line))
					all_lines += line + "\n";
				is.str(all_lines);
				read_json (is, pt_nearby);
			}
		}
		else USE_CACHED_DATA_ = false;
	}
	
	runKalmanFilter();
	
	/*latlng center;
	center.lat = 4.6;
	center.lng = 5.0;
	std::vector<latlng> locs;
	latlng l1;
	l1.lat = 2.3;
	l1.lng = 2.2;
	latlng l2;
	l2.lat = 5.4;
	l2.lng = 4.5;
	locs.push_back(l1);
	locs.push_back(l2);
	updateMap(center, locs);*/
	
	
	//return 0;
}
/*void run(char* input_im){
	//first call spotText
	std::vector<CvRect> bounding_boxes = spotText(input_im);
	//do ocr on the bounding boxes
	std::vector<std::pair<char*, int > > detected_text = performOcr(bounding_boxes, input_im);
	//do ocr correction step
	//first concatenate all detected text with "+" separator instead of " "
	std::string concatenated_text;
	for(std::vector<std::pair<char*, int > >::iterator it = detected_text.begin();
			it!=detected_text.end(); ++it){
			concatenated_text += it->first;
			concatenated_text += "+";
	}
	//replace any remaining " " by "+"
	std::replace(concatenated_text.begin(), concatenated_text.end(), ' ', '+');
	//std::cout << "search query for: " << concatenated_text << std::endl;
	//call ocrCorrection
	std::string corrected_text = ocrCorrection(concatenated_text);
	//check if there was any correction made or not
	if(corrected_text.compare("")==0)
		corrected_text = concatenated_text;
	
	//split the corrected text by " "
	std::vector<std::string> tokenized_text;
	boost::split(tokenized_text, corrected_text, boost::is_any_of(" +"));
	//for(std::vector<std::string>::iterator it = tokenized_text.begin(); it!=tokenized_text.end(); ++it)
		//std::cout << *it << std::endl;
	
	//replace any remaining " " by "+"
	std::replace(corrected_text.begin(), corrected_text.end(), ' ', '+');
	//std::cout << "corrected text: " << corrected_text << std::endl;
	
	//get nearby places
	//first process name of image to get lat and long
	std::string image_name = input_im;
	size_t start_pos = image_name.find_first_of('_');
	size_t end_pos = image_name.find_last_of('.');
	std::string location = image_name.substr(start_pos+1, end_pos);
	size_t split = location.find(',');
	double latitude = atof(location.substr(0, split).c_str());
	double longitude = atof(location.substr(split+1).c_str());
	std::pair<double, double > gps_loc (latitude, longitude);
	//call get nearby places method
	std::vector<Place> nearby_places = nearbySearch(gps_loc, corrected_text);
	bool no_photos = true;
	//std::cout << nearby_places.size() << std::endl;
	//loop over the returned places to try and get the matched location
	for(std::vector<Place>::iterator it = nearby_places.begin(); 
			it!=nearby_places.end(); ++it){
		std::cout << "current place possible location: " << it->name << std::endl;
		//save the picture reference of the array
		savePlaceIcon(*it);
		//std::cout << "here!" << std::endl;
		//std::cout << it->place_icon.size() << std::endl;
		if(it->place_icon.size()>0)
			no_photos = false;
		//loop over the returned pictures and try to see if they match the current location
		for(std::vector<std::string>::iterator ij = it->place_icon.begin();
				ij!=it->place_icon.end(); ++ij){
			//try to match the logo with the current image
			bool true_match = logoFound(&(*ij)[0], input_im);
			std::string x = true_match ? "a ":"not a ";
			std::cout << it->name << " is " << x << "possible match to input image" << std::endl;	
			if(true_match)	
				return;
		}	
	}
	//if nearbySearch returned zero results then we need to get possible combinations of 
	//search query and repeat the search with that
	if(nearby_places.size()==0 || no_photos){
		std::vector<std::string> combination_queries = getCombinations(tokenized_text);
		//loop over the combination_queries and call each of them with the results
		for(std::vector<std::string>::iterator it = combination_queries.begin();
				it!=combination_queries.end(); ++it){
			nearby_places = nearbySearch(gps_loc, *it);
			//std::cout << "size: " << nearby_places.size() << std::endl;
			if(nearby_places.size() == 0)
				continue;
			//otherwise loop over the returned places to try and get a matched location
			for(std::vector<Place>::iterator ij = nearby_places.begin(); ij!=nearby_places.end(); ++ij){
				std::cout << "current place possible location: " << ij->name << std::endl;
				//save the picture reference of the array
				if(ij->place_icon.size()>0){
					//loop over the returned places
					for(std::vector<std::string>::iterator ik = ij->place_icon.begin(); ik!=ij->place_icon.end(); ++ik){
						bool true_match = logoFound(&(*ik)[0], input_im);
						std::string x = true_match ? "a ":"not a ";
						std::cout << ij->name << " is " << x << "possible match to input image" << std::endl;
						if(true_match)
							return;
					}
				}
				else{
					std::cout << ij->name << " has no photo references" << std::endl;
				}
			}		
		}
	}
	
	return;
}*/

Place reverseSearch(const char* input_im, std::string saved_key, std::pair<double, double> gps_loc){
	//first process name of image to get lat and long
	std::string image_name = input_im;
	//size_t start_pos = image_name.find_first_of('_');
	//size_t end_pos = image_name.find_last_of('.');
	//std::string location = image_name.substr(start_pos+1, end_pos);
	//size_t split = location.find(',');
	//double latitude = atof(location.substr(0, split).c_str());
	//double longitude = atof(location.substr(split+1).c_str());
	//std::pair<double, double > gps_loc (latitude, longitude);
	//call nearbySearch with the given search word
	std::pair<std::vector<Place>, std::string> nearby_res;
	nearby_res = nearbySearch(gps_loc, saved_key);
	std::vector<Place> possible_locs = nearby_res.first;
	std::string status_string = nearby_res.second;
	if(status_string.compare("OVER_QUERY_LIMIT") == 0){
		Place error;
		error.name = status_string;
		return error;
	}
	//call text-spotting for the image
	std::cout << "calling SpotText with: " << input_im << std::endl;
	std::vector<CvRect > bounding_boxes = spotText(input_im);
	std::cout << "finished text spotting" << std::endl;
	//call tesseract for the output
	std::vector<std::pair<char*, int> > detected_words = performOcr(bounding_boxes, input_im);
	//do ocr correction step
	//first concatenate all detected text with "+" separator instead of " "
	std::string concatenated_text;
	for(std::vector<std::pair<char*, int > >::iterator it = detected_words.begin();
			it!=detected_words.end(); ++it){
			concatenated_text += it->first;
			concatenated_text += "+";
	}
	//replace any remaining " " by "+"
	std::replace(concatenated_text.begin(), concatenated_text.end(), ' ', '+');
	//call ocrCorrection
	std::cout << "calling ocrCorrection!" << std::endl;
	std::string corrected_text = ocrCorrection(concatenated_text);
	std::cout << "returned from ocrCorrection!" << std::endl;
	//check if there was any correction made or not
	if(corrected_text.compare("")==0)
		corrected_text = concatenated_text;
	
	replaceAll(corrected_text, "+", " ");
	
	//remove non characters from string
	std::string ocr_text = processString(corrected_text);
	boost::algorithm::to_lower(corrected_text);
	std::cout << "corrected text: " << corrected_text << std::endl;
	
	//readjust the scores
	std::stringstream space_sep_text (ocr_text);
	std::vector<std::pair<std::string, int > > new_detected_words;
	do{
		std::string tmp;
		space_sep_text >> tmp;
		//std::cout << "tmp now contains: " << tmp << std::endl;
		//int idx = 0;
		//loop over the detected words
		for(std::vector<std::pair<char*, int > >::iterator it = detected_words.begin();
				it != detected_words.end(); ++it){
			//std::cout << "word: " << it->first << std::endl;
			std::string word = it->first;
			if(tmp.length() < 2)
				continue;
			boost::algorithm::to_lower(tmp);
			boost::algorithm::to_lower(word);
			size_t found_idx = word.find(tmp);
			if(found_idx != std::string::npos){
				//std::cout << "found: " << it->first << std::endl;
				//std::cout << "changing: " << detected_words[found_idx].first << " to: " << tmp << std::endl;
				//detected_words[idx].first = &tmp[0];
				//std::cout << "adding: " << tmp << " with score: " << it->second << std::endl;
				//std::cout << tmp << std::endl;
				std::pair<std::string, int> new_pair (tmp, it->second);
				//std::cout << "pushing: " << new_pair.first << ", score: " << new_pair.second << std::endl;
				new_detected_words.push_back(new_pair);
				break;
			}		
		}
	}while(space_sep_text);
	if(new_detected_words.empty()){
		for(std::vector<std::pair<char*, int> >::iterator it = detected_words.begin();
				it != detected_words.end(); ++it){
			std::string corrected_string = it->first;
			boost::algorithm::to_lower(corrected_string);
			corrected_string = processString(corrected_string);
			replaceAll(corrected_string, " ", "");
			std::pair<std::string, int> new_pair (corrected_string, it->second);
			new_detected_words.push_back(new_pair);		
		}
	}	
	
	//print debugging
	/*std::cout << "printing new_detected_words vector:" << std::endl;
	for(std::vector<std::pair<std::string, int> >::iterator x = new_detected_words.begin();
			x != new_detected_words.end(); ++x){
		std::cout << "word: " << x->first << " score: " << x->second << std::endl;		
	}
	std::cout << "==========================================" << std::endl;*/
	
	Place best_match = getBestMatch(possible_locs, corrected_text);
	std::cout << "best match: " << best_match.name << std::endl;
	//check the output of the edit distance
	//TODO:parameter tuning
	//if the minimum distance is equal to the length of the longer string
	//then we do heuristic search with query expansion to find the best keyword
	int max_str_len = std::max(best_match.formatted_name.length(), corrected_text.length());
	if(best_match.match_score > ceil((double)(1.0 * max_str_len / 2.0))){
		std::cout << "best match: " << best_match.name << " with score: " << best_match.match_score
			<< " max allowed score: " << max_str_len / 2.0 << std::endl;
			
		Place query_exp_match = queryExpansion(new_detected_words, corrected_text, gps_loc);
		if(query_exp_match.name.compare("OVER_QUERY_LIMIT") == 0){
			//query limit exceeded during queryExpansion, return!
			std::cout << "query limit exceeded during query expansion" << std::endl;
			return query_exp_match;
		}
		//if the query expansion best match has a worse score than the normal
		//search, then we return the best match result
		if(query_exp_match.match_score <= best_match.match_score){
			best_match.longitude = query_exp_match.longitude;
			best_match.latitude = query_exp_match.latitude;
			best_match.name = query_exp_match.name;
			best_match.formatted_name = query_exp_match.formatted_name;
			best_match.match_score = query_exp_match.match_score;
		}
		
		/*//get the synonyms of the word that has the highest confidence level
		//initialize wordnet
		wordnet wn("/usr/share/wordnet/");
		//arrange the vector by score
		std::sort(new_detected_words.begin(), new_detected_words.end(), sort_pred());
		std::vector<std::string> max_synsets;
		for(std::vector<std::pair<std::string, int> >::iterator ij = new_detected_words.begin();
				ij != new_detected_words.end(); ++ij){
			std::cout << "word: " << ij->first << ", score: " << ij->second << std::endl;
			boost::algorithm::to_lower(ij->first);
			std::vector<synset> synsets = wn.get_synsets(ij->first);
			if(synsets.size() == 0)
				continue;
			max_synsets = synsets[0].words;
			break;	
		}
		//redo the nearby place search with the keyword being the first synonym
		//std::ostringstream keyword;
		//copy(max_synsets.begin(), max_synsets.end(), std::ostream_iterator<std::string>(keyword, "+"));
		if(max_synsets.size() != 0){
			std::vector<std::string>::iterator syns_start = max_synsets.begin();
			std::string str_keyword = *syns_start;//keyword.str();
			//str_keyword = str_keyword.substr(0, str_keyword.length()-1);
			std::cout << "query expansion now with keyword: " << str_keyword << std::endl;
			possible_locs = nearbySearch(gps_loc, str_keyword);
			if(possible_locs.size() == 0){
				std::cout << "Empty result list from query expansion" << std::endl;
			}
			else{
				start = possible_locs.begin();
				boost::algorithm::to_lower(start->name);
				place_name = start->name;
				replaceAll(start->name, " ", "");
				min_score = editDistance(corrected_text, start->name);
				Place query_exp_match;
				query_exp_match.longitude = start->longitude;
				query_exp_match.latitude = start->latitude;
				//best_match.place_icon = start->place_icon;
				query_exp_match.name = place_name;
				query_exp_match.formatted_name = start->name;
				query_exp_match.match_score = min_score;
				std::cout << query_exp_match.formatted_name << " score: " << min_score << std::endl;
				for(std::vector<Place>::iterator it = ++start; it != possible_locs.end(); ++it){
					boost::algorithm::to_lower(it->name);
					place_name = it->name;
					replaceAll(it->name, " ", "");
					curr_score = editDistance(corrected_text, it->name);
					std::cout << place_name << " score: " << curr_score << std::endl;
					if(curr_score < min_score){
						min_score = curr_score;
						query_exp_match.longitude = it->longitude;
						query_exp_match.latitude = it->latitude;
						//best_match.place_icon = it->place_icon;
						query_exp_match.name = place_name;
						query_exp_match.formatted_name = it->name;
						query_exp_match.match_score = curr_score;
					}
				}
			}
		}*/
	}
	std::cout << "best match: " << best_match.name << " with score: " << best_match.match_score << std::endl;
	return best_match;
	
}

Place getBestMatch(std::vector<Place> possible_locs, std::string corrected_text){
	Place best_match;
	if(possible_locs.size() != 0){
		//find the best match location
		std::vector<Place>::iterator start = possible_locs.begin();
		boost::algorithm::to_lower(start->name);
		std::string place_name = start->name;
		replaceAll(start->name, " ", "");
		int min_score = editDistance(corrected_text, start->name);
		best_match.longitude = start->longitude;
		best_match.latitude = start->latitude;
		//best_match.place_icon = start->place_icon;
		best_match.name = place_name;
		best_match.formatted_name = start->name;
		best_match.match_score = min_score;
		std::cout << start->name << " score: " << min_score << std::endl;
		int curr_score;
		for(std::vector<Place>::iterator it = ++start; it != possible_locs.end(); ++it){
			boost::algorithm::to_lower(it->name);
			place_name = it->name;
			replaceAll(it->name, " ", "");
			curr_score = editDistance(corrected_text, it->name);
			std::cout << place_name << " score: " << curr_score << std::endl;
			if(curr_score < min_score){
				min_score = curr_score;
				best_match.longitude = it->longitude;
				best_match.latitude = it->latitude;
				//best_match.place_icon = it->place_icon;
				best_match.name = place_name;
				best_match.formatted_name = it->name;
				best_match.match_score = curr_score;
			}
		}
	}
	else{
		std::cout << "Empty result from nearbySearch" << std::endl;
		best_match.match_score = 1000;
	}
	return best_match;
}

Place queryExpansion(std::vector<std::pair<std::string, int> > detected_words, std::string formatted_name, std::pair<double, double> gps_loc){
	//first normalize the probabilities
	//loop over the data to get the sum of the confidence levels
	double sum = 0.0;
	for(std::vector<std::pair<std::string, int> >::iterator it = detected_words.begin();
			it != detected_words.end(); ++it){
		sum += it->second;		
	}
	//add the normalized probabilities in a new vector
	std::vector<std::pair<std::string, double> > batch_data;
	std::cout << "detected_words size: " << detected_words.size() << std::endl;
	for(std::vector<std::pair<std::string, int> >::iterator it = detected_words.begin();
			it != detected_words.end(); ++it){
		boost::algorithm::to_lower(it->first);
		std::pair<std::string, double> curr_place (it->first, (double)(1.0 * it->second / sum));
		std::cout << "place: " << it->first << " normalized score: " << (double)(1.0 * it->second / sum) << std::endl;
		batch_data.push_back(curr_place);		
	}
	//sort by descending order of probabilities
	std::sort(batch_data.begin(), batch_data.end(), sort_pred());
	//get the synsets of each word
	//initialize wordnet
	wordnet wn("/usr/share/wordnet/");
	//create a map for the data and synsets
	std::map<std::string, std::vector<std::string> > word_synsets_map;
	//create a map for start index at each word
	std::map<std::string, int> word_index_map;
	for(std::vector<std::pair<std::string, double> >::iterator it = batch_data.begin();
			it != batch_data.end(); ++it){
		std::vector<std::string> synonyms;
		synonyms.push_back(it->first);
		std::vector<synset> synsets = wn.get_synsets(it->first);
		std::cout << "wordNet output for: " << it->first << std::endl;
		if(synsets.size() == 0)
			word_synsets_map[it->first] = synonyms;
		else{
			synonyms.insert(synonyms.end(), synsets[0].words.begin(), synsets[0].words.end());
			word_synsets_map[it->first] = synonyms;
			for(std::vector<std::string>::iterator x = synsets[0].words.begin();
					x != synsets[0].words.end(); ++x){
				std::cout << *x << std::endl;		
			}
		}	
		std::cout << "==============================================================================" << std::endl;
		word_index_map[it->first] = 0;
	}
	printProbabilities(batch_data);
	//Do the query expansion at most 10 times before returning
	Place best_match;
	best_match.match_score = 1000;
	for(int i = 0; i < 10; i++){
		int start_idx = 0;
		std::vector<std::string> synonyms;
		int idx;
		std::string word_key;
		while(start_idx < batch_data.size()){
			//get the word with the highest probability
			word_key = batch_data.at(start_idx).first;
			//get the index to start searching with
			idx = word_index_map[word_key];
			//get the vector of synonyms
			synonyms = word_synsets_map[word_key];
			if(idx < synonyms.size())
				break;
			else
				start_idx++;
			if(start_idx >= batch_data.size()){
				std::cout << "Finished search (ran out of words)" << std::endl;
				return best_match;
			}
		}
		word_key = batch_data.at(start_idx).first;
		idx = word_index_map[word_key];
		synonyms = word_synsets_map[word_key];
		std::string search_keyword = synonyms.at(idx);
		std::cout << "calling nearbySearch with keyword: " << search_keyword << std::endl;
		//call the nearbySearch with the search keyword
		std::pair<std::vector<Place>, std::string> nearby_res;
		nearby_res = nearbySearch(gps_loc, search_keyword);
		std::vector<Place> possible_locs = nearby_res.first;
		std::string status_string = nearby_res.second;
		if(status_string.compare("OVER_QUERY_LIMIT") == 0){
			Place error;
			error.name = status_string;
			return error;
		}
		//find the best matching place from the possible_locs
		Place curr_best_match = getBestMatch(possible_locs, formatted_name);
		std::cout << "curr_best_match: " << curr_best_match.name << ", score: " << curr_best_match.match_score << std::endl;
		//update the best_match
		if(best_match.match_score >= curr_best_match.match_score){
			best_match.longitude = curr_best_match.longitude;
			best_match.latitude = curr_best_match.latitude;
			best_match.name = curr_best_match.name;
			best_match.formatted_name = curr_best_match.formatted_name;
			best_match.match_score = curr_best_match.match_score;
		}
		//get the match percentage
		double max_str_len = std::max(curr_best_match.formatted_name.length(), formatted_name.length());
		double inv_match_score = max_str_len - curr_best_match.match_score;
		double match_accuracy = inv_match_score / max_str_len;
		if(curr_best_match.match_score == 1000){
			//empty match
			match_accuracy = 0;
		}
		std::cout << "match_accuracy: " << match_accuracy << std::endl;
		//update the place probability
		double success_prob = batch_data.at(start_idx).second;
		success_prob = success_prob + (1.0 / (idx + 2.0)) * (match_accuracy - success_prob); 
		std::pair<std::string, double> updated_pair (batch_data.at(start_idx).first, success_prob);
		batch_data.at(start_idx) = updated_pair;
		std::cout << "updated probability of: " << word_key << "/" << batch_data.at(start_idx).first << " to: " << success_prob << std::endl;
		//batch_data.at(0).second = batch_data.at(0).second * match_accuracy;
		//normalizeBatchData(batch_data);
		//update the index map
		word_index_map[word_key] = word_index_map[word_key] + 1;
		//resort by descending order of probabilities
		std::sort(batch_data.begin(), batch_data.end(), sort_pred());
		printProbabilities(batch_data);
	}
	return best_match;
}

void printProbabilities(std::vector<std::pair<std::string, double> > batch_data){
	std::cout << "Printing batch data vector" << std::endl;
	for(std::vector<std::pair<std::string, double> >::iterator it = batch_data.begin();
			it != batch_data.end(); ++it){
		std::cout << it->first << " has now confidence: " << it->second << std::endl;		
	}
	std::cout << "=============================================================" << std::endl;
}

/*void normalizeBatchData(std::vector<std::pair<std::string, double> >& batch_data){
	double sum = 0;
	for(std::vector<std::pair<std::string, double> >::iterator it = batch_data.begin();
			it != batch_data.end(); ++it){
		sum += it->second;		
	}
	std::vector<std::pair<std::string, double> > temp;
	std::copy(batch_data.begin(), batch_data.end(), temp);
	batch_data.clear();
	for(std::vector<std::pair<std::string, double> >::iterator it = temp.begin();
			it != temp.end(); ++it){
		std::pair<std::string, double> new_elem (it->first, it->second / sum);
		batch_data.push_back(new_elem);		
	}
}*/

std::vector<CvRect > spotText(const char* input_im){
	std::vector<CvRect > text_rect;
	//read the image
	ccv_dense_matrix_t* image = 0;
	ccv_read(input_im, &image, CCV_IO_GRAY | CCV_IO_ANY_FILE);
	ccv_array_t* words= ccv_swt_detect_words(image, ccv_swt_default_params);
	//load image to cv_mat in grayscale
	IplImage* cv_image = cvLoadImage(input_im, CV_LOAD_IMAGE_COLOR);
	if (words){
		int i;
		for (i = 0; i < words->rnum; i++)
		{
			ccv_rect_t* rect = (ccv_rect_t*)ccv_array_get(words, i);
			printf("%d %d %d %d\n", rect->x, rect->y, rect->width, rect->height);
			//Visualization! draw rectangle
			cvRectangle(cv_image,cvPoint(rect->x,rect->y), cvPoint(rect->x+rect->width, rect->y+rect->height),cvScalar(0, 255, 0, 0),2,8,0);
			//add rect to text_rect
			CvRect rectangle= cvRect(rect->x, rect->y, rect->width, rect->height);
			text_rect.push_back(rectangle);
		}
		printf("total : %d\n", words->rnum);
		std::cout << "number of rectangles: " << text_rect.size() << std::endl;
		ccv_array_free(words);
	}
	ccv_matrix_free(image);
	//Visualization
	cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
   	cvShowImage( "Display window", cv_image );                   // Show our image inside it.

    cv::waitKey(0);                                          // Wait for a keystroke in the window
    
    return text_rect;
}

std::vector<std::pair<char*, int> > performOcr(std::vector<CvRect > bounding_boxes, const char* input_im){
	
	char* tess_data_path = TESSDATA_PATH_;
	char* language = DETECTION_LANGUAGE_;
	std::vector<std::pair<char*, int> > detected_text;
	//read the picture
	Pix *image = pixRead(input_im);
	//initialize tesseract api
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	api->Init(tess_data_path, language, tesseract::OEM_TESSERACT_ONLY);
	//api->ReadConfigFile(CONFIG_FILE_);
	api->SetImage(image);
	//tesseract example approach
	//Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
	//printf("Found %d textline image components.\n", boxes->n);
	//for (int i = 0; i < boxes->n; i++) {
	//	BOX* box = boxaGetBox(boxes, i, L_CLONE);
	//	api->SetRectangle(box->x, box->y, box->w, box->h);
	//	char* ocrResult = api->GetUTF8Text();
	//	int conf = api->MeanTextConf();
	//	fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s",
	//			        i, box->x, box->y, box->w, box->h, conf, ocrResult);
	//}
	//std::cout << "============================================" << std::endl;
	//std::cout << "CCV result" << std::endl;
	//get components from ccv
	//loop over bounding boxes from ccv
	for(std::vector<CvRect >::iterator it = bounding_boxes.begin(); it!=bounding_boxes.end(); ++it){
		api->SetRectangle(it->x, it->y, it->width, it->height);
		//get text result
		char* ocrResult = api->GetUTF8Text();
		//remove new lines
		*std::remove(ocrResult, ocrResult+strlen(ocrResult), '\n') = '\0';
		//get confidence
		int conf = api->MeanTextConf();
		std::pair<char* , int > detection (ocrResult, conf);
		detected_text.push_back(detection);
		fprintf(stdout, "Box: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s\n", it->x, it->y, it->width, it->height, conf, ocrResult);
	}
	return detected_text;
}

static int writer(char *data, size_t size, size_t nmemb, std::string *buffer)
{
    int result = 0;
    if (buffer != NULL)
    {
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }

    return result;
}

std::pair<std::vector<Place>, std::string> nearbySearch(std::pair<double, double> gps_location, std::string keyword){
	std::string search_keyword;
	//if the keyword is the image name, then 
	if(keyword.find("im_") == 0){
		search_keyword = "*";
	}
	else search_keyword = keyword;
	//create vector for the returned results
	std::vector<Place> nearbyLocs;
	//format the url
	std::stringstream ss;
	ss << "https://maps.googleapis.com/maps/api/place/nearbysearch/json?location=";
	ss << gps_location.first << "," << gps_location.second;
	ss << "&radius=18&keyword=" << search_keyword << "&key=AIzaSyBwfgwqnuQ9Dkh4gYOFArAmyDBU-dRzhpM";
	std::string url = ss.str();
	//std::cout << "Will retrive from url: '" << url << "'" << std::endl;
	CURLWrapper::Easy easy;
	//std::cout << USE_CACHED_DATA_ << std::endl;
	if(!USE_CACHED_DATA_){
		//retrieve results from cUrl
		easy.SetOption(CURLOPT_URL, url.c_str());
		easy.SetOption(CURLOPT_HEADER, 0);
		easy.SetOption(CURLOPT_FOLLOWLOCATION, 1);
		easy.SetOption(CURLOPT_WRITEFUNCTION, writer);
		easy.SetOption(CURLOPT_WRITEDATA, &buffer);
		easy.Perform();
		//std::cout << "finished search" << std::endl;
		//std::cout << buffer << std::endl;
    }
    else{
    	buffer = loadNearbyData(keyword);
    }
	//load the results with jansson
	json_t *root;
    json_error_t error;
    
    root = json_loads(buffer.c_str(), 0, &error);
	buffer = "";
	
	/*if(!USE_CACHED_DATA_){
		//write the results to file
		//std::cout << json_dumps(root, JSON_INDENT(0)) << std::endl;
		saveNearbyData(keyword, json_dumps(root, JSON_INDENT(0)));
		//std::cout << "finished save" << std::endl;
	}*/
	
	//check if reply from cUrl is ok
	if(easy.IsOK()){
		if(!root)
		{
			//fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
			std::cout << "error! " << error.line << " " << error.text << std::endl;
			std::pair<std::vector<Place>, std::string> output (nearbyLocs, "");
			return output;
		}
		json_t *status, *results;
		//get the returned status
		status = json_object_get(root, "status");
		if(!json_is_string(status))
		{
			std::cout << "error: return data : status is not a string" << std::endl;
			json_decref(root);
			std::pair<std::vector<Place>, std::string> output (nearbyLocs, "no status");
			return output;
		}
		std::string status_string = json_string_value(status);
		if(status_string.compare("OK") == 0){
			//query returned results		
			results = json_object_get(root, "results");
			if(!json_is_array(results)){
				std::cout << "error: return data : results is not an array" << std::endl;
				json_decref(root);
				std::pair<std::vector<Place>, std::string> output (nearbyLocs, status_string);
				return output;
			}
			//loop over the results array
			json_t *data, *geometry, *location, *longit, *latit, *photos, *photo, *photo_ref, *name;
			std::vector<std::string> photo_url;
			//std::cout << "results size: " << json_array_size(results) << std::endl;
			for(size_t i = 0; i<json_array_size(results); i++){
				data = json_array_get(results, i);
				//get the icon
				photos = json_object_get(data, "photos");
				if(!json_is_array(photos)){
					//std::cout << "error: no photos array found" << std::endl;
				}
				else{
					//loop over the photos array
					for(size_t j = 0; j<json_array_size(photos); j++){
						//std::cout << "beep" << std::endl;
						photo = json_array_get(photos, j);
						photo_ref = json_object_get(photo, "photo_reference");
						//call method to return photo query
						//photo_url.push_back(getPhotoRef(json_string_value(photo_ref)));
						//std::cout << "boop" << std::endl;
					}
				}
				//std::cout << "here" << std::endl;
				try{
				//std::cout << "finished photos loop" << std::endl;
				//get lang and latit
				geometry = json_object_get(data, "geometry");
				location = json_object_get(geometry, "location");
				longit = json_object_get(location, "lng");
				latit = json_object_get(location, "lat");
				std::pair<double, double> pt1 (json_real_value(latit), json_real_value(longit));
				Odometry odom = getOdom(gps_location, pt1);
				double dist = odom.dist;
				if(dist > 0.15)
					continue;
				//std::cout << "dist: " << dist << std::endl;
				//get the name
				name = json_object_get(data, "name");
				//create new place
				Place curr;
				curr.place_icon = photo_url;
				curr.longitude = json_real_value(longit);
				curr.latitude = json_real_value(latit);
				curr.name = json_string_value(name);
				//savePlaceIcon(curr);
				//std::cout << "finished savePlaceIcon" << std::endl;
				nearbyLocs.push_back(curr);
				photo_url.erase(photo_url.begin(), photo_url.end());
				}
				catch(std::exception &e){
					std::cout << e.what() << std::endl;
				}
			}
			
		}
		else{
			std::pair<std::vector<Place>, std::string> output (nearbyLocs, status_string);
			if(status_string.compare("OVER_QUERY_LIMIT") == 0){
				//exceded the query limit
				std::cout << "exceeded query limit for today" << std::endl;
				return output;
			}	
			else{
				//query failed
				std::cout << "returned status string: " << status_string << std::endl;
				return output;
			}
		}
	}
	else{
		std::pair<std::vector<Place>, std::string> output (nearbyLocs, "curl error");
		std::cout << "failed to perform with cUrl" << std::endl;
		return output;
	}
	//std::cout << "returning" << std::endl;
	//std::cout << "returning: " << nearbyLocs.size() << std::endl;
	std::pair<std::vector<Place>, std::string> output (nearbyLocs, "OK");
	return output;
}

bool logoFound(char* logo_im, const char* input_im){
	cv::Mat img_1 = cv::imread( logo_im, CV_LOAD_IMAGE_GRAYSCALE );
	cv::Mat img_2 = cv::imread( input_im, CV_LOAD_IMAGE_GRAYSCALE );
	
	if( !img_1.data || !img_2.data ){
		std::cout<< " --(!) Error reading images " << std::endl; return false;
	}

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	cv::SurfFeatureDetector detector( minHessian );

	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;

	detector.detect( img_1, keypoints_1 );
	detector.detect( img_2, keypoints_2 );

	//-- Step 2: Calculate descriptors (feature vectors)
	cv::SurfDescriptorExtractor extractor;

	cv::Mat descriptors_1, descriptors_2;

	extractor.compute( img_1, keypoints_1, descriptors_1 );
	extractor.compute( img_2, keypoints_2, descriptors_2 );

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	cv::FlannBasedMatcher matcher;
	std::vector< cv::DMatch > matches;
	matcher.match( descriptors_1, descriptors_2, matches );

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for( int i = 0; i < descriptors_1.rows; i++ ){ 
		double dist = matches[i].distance;
		if( dist < min_dist ) min_dist = dist;
		if( dist > max_dist ) max_dist = dist;
	}

	//printf("-- Max dist : %f \n", max_dist );
	//printf("-- Min dist : %f \n", min_dist );

	//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
	//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
	//-- small)
	//-- PS.- radiusMatch can also be used here.
	std::vector< cv::DMatch > good_matches;

	for( int i = 0; i < descriptors_1.rows; i++ ){ 
		if( matches[i].distance <= std::max(2*min_dist, 0.02) ){ 
			good_matches.push_back( matches[i]); 
		}
	}

	//-- Draw only "good" matches
	cv::Mat img_matches;
	drawMatches(img_1, keypoints_1, img_2, keypoints_2,
				good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
				std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	//-- Show detected matches
	imshow( "Good Matches", img_matches );

	//std::cout << "all matches: " << (int)matches.size() << " good matches: " <<  
		//		(int)good_matches.size() << " ratio: " << (double)good_matches.size()/(double)matches.size() << std::endl;

	//for( int i = 0; i < (int)good_matches.size(); i++ )
	//{ printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); }

	//cv::waitKey(0);

	std::cout << "match probability: " << (double)good_matches.size()/(double)matches.size() << std::endl;
	return (double)good_matches.size()/(double)matches.size()>0.4;
}

void saveMapImage(std::string url){
	//std::cout << "url: " << url << std::endl;
	CURL *image;
	CURLcode imgresult;
	FILE *fp;
	
	image = curl_easy_init();
	if(image){
		fp = fopen(MAP_PATH_, "wb");
		
		if( fp == NULL ) std::cout << "File cannot be opened"; 

		curl_easy_setopt(image, CURLOPT_URL, url.c_str()); 
		curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL); 
		curl_easy_setopt(image, CURLOPT_WRITEDATA, fp); 
		
		imgresult = curl_easy_perform(image); 
		//std::cout << "grabbing image" << std::endl;
		if( imgresult ){ 
			std::cout << "Cannot grab the image!\n"; 
			return;
		} 
	}
	else{
		std::cout << "curl error" << std::endl;
		return;
	}

	// Clean up the resources 
	curl_easy_cleanup(image); 
	// Close the file 
	fclose(fp); 
	return;
}

void savePlaceIcon(Place& place){
	CURL *image; 
	CURLcode imgresult; 
	FILE *fp; 

	int counter = 0;
	for(std::vector<std::string>::iterator it= place.place_icon.begin(); it!=place.place_icon.end(); ++it){
		//preprocess the image name
		std::string image_url = *it;
		std::string image_name;
		//std::cout << "image_url: " << image_url << std::endl;
		unsigned found = image_url.find_last_of("/");
		if(found >= image_url.length())
			continue;
		image_name = image_url.substr(found+1);
		found = image_name.find_last_of(".");
		if(found >= image_name.length())
			continue;
		std::stringstream ss;
		ss << place.name << "_" << counter << image_name.substr(found);
		image_name = ss.str();
		std::replace(image_name.begin(), image_name.end(), ' ', '_');
		std::replace(image_name.begin(), image_name.end(), '/', '_');
		
		std::string new_image_path = saved_logos_prefix+image_name;
		//std::cout << saved_logos_prefix << " " << image_name << std::endl;
		//std::cout << new_image_path << std::endl;

		image = curl_easy_init(); 
		if( image ){ 
			// Open file 
			fp = fopen(new_image_path.c_str(), "wb"); 
			if( fp == NULL ) std::cout << "File cannot be opened"; 

			curl_easy_setopt(image, CURLOPT_URL, image_url.c_str()); 
			curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL); 
			curl_easy_setopt(image, CURLOPT_WRITEDATA, fp); 

			//std::cout << "setup done" << std::endl;
			// Grab image 
			imgresult = curl_easy_perform(image); 
			//std::cout << "grabbing image" << std::endl;
			if( imgresult ){ 
				std::cout << "Cannot grab the image!\n"; 
				return;
			} 
		} 
		else{
			std::cout << "curl error" << std::endl;
			return;
		}

		// Clean up the resources 
		curl_easy_cleanup(image); 
		// Close the file 
		fclose(fp); 

		
		//check if the image is in jpg (if yes we need to convert it to png)
		if(image_name.find("jpg")!=std::string::npos || image_name.find("jpeg")!=std::string::npos){
			unsigned found = image_name.find_last_of(".");
			rgb8_image_t im;
			jpeg_read_image(new_image_path, im);
			//std::cout << saved_logos_prefix+image_name.substr(0,found+1)+"png" << std::endl;
			//view(im);
			png_write_view(saved_logos_prefix+image_name.substr(0,found+1)+"png", view(im));
			new_image_path = saved_logos_prefix+image_name.substr(0,found+1)+"png";
		}
	
		//save the new image icon location in the places struct
		place.place_icon[counter] = new_image_path;
		
		counter++;
	}
	//std::cout << "finished savePlaceIcon and returning" << std::endl;
	return;
}

std::string getPhotoRef(std::string photo_ref){
	std::stringstream ss;
	ss << "https://maps.googleapis.com/maps/api/place/photo?maxwidth=400&photoreference=" << photo_ref 
		<< "&key=AIzaSyBwfgwqnuQ9Dkh4gYOFArAmyDBU-dRzhpM";
	std::string url = ss.str();
						
	char* return_url;
						
	CURL* curl;
	CURLcode curl_res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_res = curl_easy_perform(curl);
		
		if(curl_res==CURLE_OK){
			curl_res = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL , &return_url);
			if(curl_res!=CURLE_OK)
				std::cout << "curl error by getting redirect url" << std::endl;
		}
		else{
			std::cout << "curl error" << std::endl;
		}	
	
	}
	return return_url;
}

std::string ocrCorrection(std::string query){
	std::stringstream ss;
	ss << "https://www.googleapis.com/customsearch/v1?q=" << query 
		<< "&cx=014594212095381448740%3A2kyeah__gum&key=AIzaSyBwfgwqnuQ9Dkh4gYOFArAmyDBU-dRzhpM";
	std::string url = ss.str();
	//std::cout << url << std::endl;
	//retrieve results from cUrl
	
	CURLWrapper::Easy easy;
	easy.SetOption(CURLOPT_URL, url.c_str());
	easy.SetOption(CURLOPT_HEADER, 0);
	easy.SetOption(CURLOPT_FOLLOWLOCATION, 1);
	easy.SetOption(CURLOPT_WRITEFUNCTION, writer);
	easy.SetOption(CURLOPT_WRITEDATA, &buffer);
	easy.Perform();
   
	//load the results with jansson
	json_t *root;
    json_error_t error;
    
    root = json_loads(buffer.c_str(), 0, &error);
	buffer = "";
	
	
	//check if reply from cUrl is ok
	if(easy.IsOK()){
		if(!root)
		{
			//fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
			std::cout << "error! " << error.line << " " << error.text << std::endl;
			return "";
		}
		json_t *spelling, *corrected_query;
		//get the returned status
		spelling = json_object_get(root, "spelling");
		if(!spelling)
			return "";
		corrected_query = json_object_get(spelling, "correctedQuery");
		if(corrected_query)
			return json_string_value(corrected_query);
		else return "";
		
	}
	else{
		std::cout << "failed to perform with cUrl" << std::endl;
		return "";
	}
}

std::vector<std::string> getCombinations(std::vector<std::string> tokens){
	std::vector<std::string> combin;
	for(int i=tokens.size()-1; i>0; i--){
		combinationRec(tokens, i, 0, 0, "", combin);
	}	
	return combin;
}

int editDistance(std::string s, std::string t){
	// degenerate cases
	if(s.compare(t) == 0)
		return 0;
	if(s.length() == 0)
		return t.length();
	if(t.length() == 0)
		return s.length();

	// create two work vectors of integer distances
	int v0 [t.length() + 1];
	int v1 [t.length() + 1];

	// initialize v0 (the previous row of distances)
	// this row is A[0][i]: edit distance for an empty s
	// the distance is just the number of characters to delete from t
	for (int i = 0; i < t.length() + 1; i++)
		v0[i] = i;

	for (int i = 0; i < s.length(); i++){
		// calculate v1 (current row distances) from the previous row v0

		// first element of v1 is A[i+1][0]
		//   edit distance is delete (i+1) chars from s to match empty t
		v1[0] = i + 1;

		// use formula to fill in the rest of the row
		for (int j = 0; j < t.length(); j++){
			int cost = (s[i] == t[j]) ? 0 : 1;
			v1[j + 1] = std::min(std::min(v1[j] + 1, v0[j + 1] + 1), v0[j] + cost);
		}

		// copy v1 (current row) to v0 (previous row) for next iteration
		for (int j = 0; j < t.length() + 1; j++)
			v0[j] = v1[j];
	}

	return v1[t.length()];
}

void combinationRec(std::vector<std::string> &words, int max_len, int curr_size, int curr_start, std::string curr_word, std::vector<std::string> &result){
	if(curr_size==max_len){
		//std::cout << curr_word << std::endl;
		result.push_back(curr_word);
		return;
	}
	
	if(words.size()-curr_start < max_len-curr_size)
		return;
	
	std::string x;
	for(int i=curr_start; i<words.size(); i++){
		x = curr_word+"+"+words[i];
		combinationRec(words, max_len, curr_size+1, i+1, x, result);
	}
}

void saveNearbyData(std::string keyword, std::string json_data){
	// Write json.
	pt_nearby.put (keyword, json_data);
}
std::string loadNearbyData(std::string keyword){
	std::string result = pt_nearby.get<std::string> (keyword);
	return result;
}
/*void saveCorrText(std::string text, std::string corr_text){
	// Write json.
	pt_text.put (text, corr_text);
}
std::string loadCorrText(std::string text){
	std::string result = pt_text.get<std::string> (text);
	return result;
}*/

// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / pi);
}


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

Odometry getOdom(std::pair<double, double> pt1, std::pair<double, double> pt2){
	Odometry result;
	result.dist = distanceEarth(pt1.first, pt1.second, pt2.first, pt2.second);
	return result;
}

std::pair<double, double> getGPS(Pose robot_pose, std::pair<double, double> pt1){
	//std::cout << "point: " << pt1.first << "," << pt1.second << std::endl;
	//std::cout << "robot pose: " << robot_pose.mu(0) << "," << robot_pose.mu(1) << ", " << robot_pose.mu(2) << std::endl;
	double dist = sqrt(pow(robot_pose.mu(0), 2)+pow(robot_pose.mu(1),2));
	//std::cout << "range: " << dist << std::endl;
	double earth_radius = 6371.0000; //earth radius in kmeters
	double delta = (dist / earth_radius);
	//std::cout << "delta: " << delta << std::endl;
	double phi1 = pt1.first * M_PI / 180;
	double lambda1 = pt1.second * M_PI / 180;
	double phi2 =  asin(sin(phi1) * cos(delta) + cos(phi1) * sin(delta) * cos(robot_pose.mu(2)));
	double lambda2 = lambda1 + atan2(sin(robot_pose.mu(2)) * sin(delta) * cos(phi1), cos(delta) - sin(phi1) * sin(phi2));
	phi2 = phi2 * 180 / M_PI;
	lambda2 = lambda2 * 180 / M_PI;
	//std::cout << "returning: " << phi2 << "," << lambda2 << std::endl;
	std::pair<double, double> gps (phi2, lambda2);
	return gps;
}

double normalize_angle(double angle){
	while(angle>M_PI)
		angle = angle - 2 * M_PI;
	while(angle<-1*M_PI)
		angle = angle + 2*M_PI;
	return angle;
}

MatrixXd normalize_all_bearings(MatrixXd z){
	for(int i = 1; i < z.rows(); i += 2){
		z(i) = normalize_angle(z(i));
	}
	return z;
}

Pose motionModel(Pose curr_pose, Odometry motion){
	//std::cout << "MOTION MODEL" << std::endl;
	//std::cout << "prev mu: " << curr_pose.mu << std::endl;
	//std::cout << "prev sigma: " << curr_pose.sigma << std::endl;
	
	double old_theta = curr_pose.mu(2);
	
	curr_pose.mu(0) = curr_pose.mu(0) + motion.dist * cos(curr_pose.mu(2) + motion.theta1);
	curr_pose.mu(1) = curr_pose.mu(1) + motion.dist * sin(curr_pose.mu(2) + motion.theta1);
	curr_pose.mu(2) = normalize_angle(curr_pose.mu(2) + motion.theta1 + motion.theta2);
	
	MatrixXd G = MatrixXd::Identity(curr_pose.sigma.rows(), curr_pose.sigma.cols());
	G(0,2) = -motion.dist * sin(old_theta + motion.theta1);
	G(1,2) = motion.dist * cos(old_theta + motion.theta1);
	//std::cout << "G: " << G << std::endl;
	
	double noise = 0.1;
	MatrixXd motion_noise = MatrixXd::Zero(curr_pose.sigma.rows(), curr_pose.sigma.cols());
	motion_noise(0,0) = noise;
	motion_noise(1,1) = noise;
	motion_noise(2,2) = 1.0 * noise/10;
	//std::cout << "motion_noise: " << motion_noise << std::endl;
	//std::cout << G * curr_pose.sigma * G.transpose() << std::endl;
	curr_pose.sigma = G * curr_pose.sigma * G.transpose() + motion_noise;
	
	//std::cout << "motion model!" << std::endl;
	//std::cout << "new mu: " << curr_pose.mu << std::endl;
	//std::cout << "new sigma: " << std::endl;
	//std::cout << curr_pose.sigma << std::endl;
	
	return curr_pose;
}


Pose correctionStep(Pose estim_pose, std::vector<Location> observed_locations, std::vector<bool> &observed_landmarks){
	//std::cout << "CORRECTION STEP" << std::endl;
	//std::cout << "prev mu: " << estim_pose.mu << std::endl;
	//std::cout << "prev sigma: " << estim_pose.sigma << std::endl;
	
	MatrixXd Z = MatrixXd::Zero(observed_locations.size() * 2, 1);
	MatrixXd expected_Z = MatrixXd::Zero(observed_locations.size() * 2, 1);
	MatrixXd delta = MatrixXd::Zero(2, 1);
	MatrixXd H (observed_locations.size() * 2, estim_pose.sigma.rows());
	
	//loop over the observed_locations
	double curr_range, curr_bearing;
	int index = 0;
	for(std::vector<Location>::iterator it = observed_locations.begin(); 
		it != observed_locations.end(); ++it){
			//compute the range and bearing
			curr_range = it->range;
			curr_bearing = it->bearing;
			//std::cout << "current landmark range: " << curr_range << ", bearing: " << curr_bearing << std::endl;
			if(observed_landmarks[it->id] == false)
			{
				//initialize the pose of the landmark in mu based on the measurement and the robot pose
				estim_pose.mu(2 * it->id + 3) = estim_pose.mu(0) + curr_range * cos(curr_bearing + estim_pose.mu(2));
				estim_pose.mu(2 * it->id + 4) = estim_pose.mu(1) + curr_range * sin(curr_bearing + estim_pose.mu(2));
			
				//std::cout << "estimated pose:" << estim_pose.mu(2 * it->id + 3) << ", " << estim_pose.mu(2 * it->id + 4) << std::endl;
				//std::cout << "mu: " << estim_pose.mu(0) << ", " << estim_pose.mu(1) << std::endl;
				//std::cout << "current landmark location: " << estim_pose.mu(2 * it->id + 3) << ", " << estim_pose.mu(2 * it->id +4) << std::endl;
				observed_landmarks[it->id] = true;
			}
			//Add the landmark measurement to the Z vector
			Z(2 * index) = curr_range;
			Z(2 * index + 1) = curr_bearing;
			//std::cout << "Z range: " << Z(2 * index) << ",bearing: " << Z(2 * index + 1) << std::endl;
	
			//Use the current estimate of the landmark pose
			//to compute the corresponding expected measurement in expectedZ:
			delta(0) = estim_pose.mu(2 * it->id + 3) - estim_pose.mu(0);
			delta(1) = estim_pose.mu(2 * it->id + 4) - estim_pose.mu(1);
			//std::cout << "delta: " << delta << std::endl;
			MatrixXd temp = delta.transpose() * delta;
			double q= temp(0);
			expected_Z(2 * index) = sqrt(q);
			expected_Z(2 * index + 1) = normalize_angle(atan2(delta(1), delta(0)) - estim_pose.mu(2));
			//std::cout << "expected measurement range: " << expected_Z(2 * index) << ", bearing: " << expected_Z(2 * index +1) << std::endl;
			
			//Compute the Jacobian Hi of the measurement function h for this observation
			MatrixXd Hi = MatrixXd::Zero(2, estim_pose.sigma.rows());
			Hi(0, 0) = -delta(0) / (sqrt(delta(0) * delta(0) + delta(1) * delta(1)));
			Hi(0, 1) = -delta(1) / (sqrt(delta(0) * delta(0) + delta(1) * delta(1)));
			Hi(0, 2 * it->id + 3) = delta(0) / (sqrt(delta(0) * delta(0) + delta(1) * delta(1)));
			Hi(0, 2 * it->id + 4) = delta(1) / (sqrt(delta(0) * delta(0) + delta(1) * delta(1)));
			Hi(1, 0) = delta(1) / (delta(0) * delta(0) + delta(1) * delta(1));
			Hi(1, 1) = -delta(0) / (delta(0) * delta(0) + delta(1) * delta(1));
			Hi(1, 2) = -1;
			Hi(1, 2 * it->id + 3) = -delta(1) / (delta(0) * delta(0) + delta(1) * delta(1));
			Hi(1, 2 * it->id + 4) = delta(0) / (delta(0) * delta(0) + delta(1) * delta(1));
		
			//std::cout << "Hi:" << Hi << std::endl;
			
			// Augment H with the new Hi
			H << Hi;	
			//increment the index
			index++;
	}
	
	//std::cout << "H matrix: " << H << std::endl;

	//Construct the sensor noise matrix Q
	MatrixXd Q = 0.01 * MatrixXd::Identity(2 * observed_locations.size(), 2 * observed_locations.size());
	//std::cout << "Q matrix: " << Q << std::endl;

	//Compute the Kalman gain
	MatrixXd K = MatrixXd::Zero(estim_pose.sigma.rows(), 2 * observed_locations.size());
	MatrixXd temp = H * estim_pose.sigma * H.transpose() + Q;
	//std::cout << "inversion step: " << temp.inverse() << std::endl;
	//std::cout << "sigma * h^t: " << estim_pose.sigma * H.transpose() << std::endl; 
	K = estim_pose.sigma * H.transpose() * temp.inverse();
	//std::cout << "Kalman gain: " << K << std::endl;
	
	//Compute the difference between the expected and recorded measurements.
	//Remember to normalize the bearings after subtracting!
	MatrixXd difference = normalize_all_bearings(Z - expected_Z);
	//std::cout << "difference matrix: " << difference << std::endl;

	//Finish the correction step by computing the new mu and sigma.
	//Normalize theta in the robot pose.
	estim_pose.mu = estim_pose.mu + K * difference;
	estim_pose.sigma = (MatrixXd::Identity(estim_pose.sigma.rows(), estim_pose.sigma.cols()) - K * H) * estim_pose.sigma;
	
	//std::cout << "correction step!" << std::endl;
	//std::cout << "new mu: " << estim_pose.mu << std::endl;
	//std::cout << "new sigma: " << std::endl;
	//std::cout << estim_pose.sigma << std::endl;
	return estim_pose;
}

void runKalmanFilter(){
	//initialize visualization
	/*int argc = 5;
	char* argv[5]= {"hello"};
	QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(MainWindow::tr("Image Viewer"));
    QCommandLineParser commandLineParser;
    commandLineParser.addHelpOption();
    commandLineParser.addPositionalArgument(MainWindow::tr("[file]"), MainWindow::tr("Image file to open."));
    commandLineParser.process(QCoreApplication::arguments());
    MainWindow imageViewer;
    imageViewer.show();*/
	//open data file
	//std::string file_loc_ = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/TestRun/odometry.dat";
	
	//open the results file
	std::ofstream results_file;
	results_file.open(RESULTS_FILE_, std::ios_base::app);
	
	//open the new odometry file to copy the data to
	std::ofstream new_data_file;
	new_data_file.open(NEW_ODOM_FILE_);
		
	latlng center;
	std::vector<latlng> locs;
	std::ifstream data_file;
	data_file.open(ODOM_DATA_, std::ios::in);
	
	if(data_file.is_open()){
		std::string curr_line;
		std::getline(data_file, curr_line);
		std::stringstream xs (curr_line);		
		//get the number of landmarks
		int num_landmarks;
		xs >> num_landmarks;
		//get initial gps
		std::pair<double, double> corrected_gps;
		xs >> corrected_gps.first;
		xs >> corrected_gps.second;
		center.lat = corrected_gps.first;
		center.lng = corrected_gps.second;
		//initialize the robot pose
		Pose robot_pose;
		robot_pose.mu = MatrixXd::Zero(2 * num_landmarks + 3, 1);
		robot_pose.mu(0) = corrected_gps.first;
		robot_pose.mu(1) = corrected_gps.second;
		robot_pose.sigma = MatrixXd::Zero(2 * num_landmarks + 3, 2 * num_landmarks +3);
		robot_pose.sigma.block(3, 3, 2 * num_landmarks, 2 * num_landmarks).setIdentity();
		robot_pose.sigma = robot_pose.sigma * 1000;
		std::vector<bool> observed_landmarks;
		observed_landmarks.reserve(num_landmarks);
		std::fill(observed_landmarks.begin(), observed_landmarks.end(), false);
		
		std::string label;
		int index = 0;
		std::cout << "starting robot pose: " << robot_pose.mu(0) << "," << robot_pose.mu(1) << std::endl;
	
		latlng tmp;
		bool start_saving = false;
		//loop over the data
		while(std::getline(data_file, curr_line)){
			//if empty line is reached
			if(curr_line.compare("") == 0)
				continue;
			std::stringstream ss(curr_line);
			std::pair<double, double> curr_gps;
			ss >> curr_gps.first;
			ss >> curr_gps.second;
			
			//add the current location markers
			//std::stringstream robot_loc_parser;
			//robot_loc_parser << curr_gps.first;
			//robot_loc_parser << ",";
			//robot_loc_parser << curr_gps.second;
			//std::stringstream robot_label_parser;
			//robot_label_parser << "R";
			//robot_label_parser << index;
			//imageViewer.updateTrueLocs(robot_loc_parser.str(), robot_label_parser.str());
			
			std::cout << "detected_gps: " << curr_gps.first << "," << curr_gps.second << std::endl;
			
			if(start_saving){
				new_data_file << curr_line << "\n";
				continue;
			}
			//---------------------------Motion model---------------------------
			//update the robot pose
			robot_pose.mu(0) = curr_gps.first;
			robot_pose.mu(1) = curr_gps.second;
			//update the sigma
			double noise = 0.1;
			MatrixXd motion_noise = MatrixXd::Zero(robot_pose.sigma.rows(), robot_pose.sigma.cols());
			motion_noise(0,0) = noise;
			motion_noise(1,1) = noise;
			motion_noise(2,2) = 1.0 * noise/10;
			robot_pose.sigma = robot_pose.sigma + motion_noise;
			
			std::cout << "after motion model: " << robot_pose.mu(0) << "," << robot_pose.mu(1) << std::endl;
			
			//get the image name and search keyword
			std::string image_name;//, search_keyword;
			ss >> image_name;
			//ss >> search_keyword;
			unsigned idx = image_name.find_last_of("/");
			std::string saved_key = image_name.substr(idx + 1);
			
			//std::cout << "saved_key: " << saved_key << std::endl;
			//std::cout << "calling reverseSearch" << std::endl;
			Place detected_location = reverseSearch(image_name.c_str(), saved_key, curr_gps);
			if(detected_location.name.compare("OVER_QUERY_LIMIT") == 0){
				//query limit exceeded
				std::cout << "query limit exceeded\nSaving and exiting" << std::endl;
				//write the curr_gps and remaining number of landmarks to the new odom file
				new_data_file << num_landmarks << " " << curr_gps.first << " " << curr_gps.second << "\n";
				//set flag to true to start copying the remaining data
				start_saving = true;
				//close the results file
				results_file.close();
				continue;
			}
			else{
				num_landmarks--;
			}
			
			//-----------------------------------Correction Step-------------------------------------
			//Construct the sensor noise matrix Q
			MatrixXd Q = 0.01 * MatrixXd::Identity(2, 2);
	
			//Construct the C matrix
			MatrixXd C = MatrixXd::Identity(2, robot_pose.sigma.rows());
			
			//Compute the Kalman gain
			MatrixXd K = MatrixXd::Zero(robot_pose.sigma.rows(), 2);
			MatrixXd temp = C * robot_pose.sigma * C.transpose() + Q;
			//std::cout << "inversion step: " << temp.inverse() << std::endl;
			//std::cout << "sigma * h^t: " << estim_pose.sigma * H.transpose() << std::endl; 
			K = robot_pose.sigma * C.transpose() * temp.inverse();
			//std::cout << "Kalman gain: " << K << std::endl;
			
			MatrixXd detected_loc_gps (2, 1);
			detected_loc_gps(0) = detected_location.latitude;
			detected_loc_gps(1) = detected_location.longitude;
			
			robot_pose.mu = robot_pose.mu + K * (detected_loc_gps - C * robot_pose.mu);
			MatrixXd ident = MatrixXd::Identity(robot_pose.sigma.rows(), robot_pose.sigma.cols());
			robot_pose.sigma = (ident - K * C) * robot_pose.sigma;
			
			//add the current landmark location markers
			//std::stringstream landmark_loc_parser;
			//landmark_loc_parser << detected_location.latitude;
			//landmark_loc_parser << ",";
			//landmark_loc_parser << detected_location.longitude;
			//std::stringstream landmark_label_parser;
			//landmark_label_parser << "L";
			//landmark_label_parser << index;
			//imageViewer.updateTrueLandmarks(landmark_loc_parser.str(), landmark_label_parser.str());
			
			std::cout << "landmark: " << detected_location.latitude << "," << detected_location.longitude << std::endl;
			
			std::cout << "corrected_gps: " << robot_pose.mu(0) << "," << robot_pose.mu(1) << std::endl;
		
			tmp.lat = robot_pose.mu(0);
			tmp.lng = robot_pose.mu(1);
			locs.push_back(tmp);
			
			//write the corrected position and the bestmatch loc to file
			results_file << robot_pose.mu(0) << " " << robot_pose.mu(1) << " " << detected_location.name << "\n";
			
			//plot the updates
			//std::stringstream parser;
			//parser << robot_pose.mu(0);
			//parser << ",";
			//parser << robot_pose.mu(1);
			//std::stringstream result_label_parser;
			//result_label_parser << "P";
			//result_label_parser << index;
			//imageViewer.updateUrl(parser.str(), result_label_parser.str());
			//saveMapImage(imageViewer.url_string);
			//imageViewer.updateGui(MAP_PATH_);
			//app.processEvents();
			
			index++;
			
		}
		new_data_file.close();
		data_file.close();
		
		/*if(!USE_CACHED_DATA_){
			std::ostringstream buf; 
			write_json (buf, pt_nearby, false);
			std::ofstream nearby_file;
			nearby_file.open(NEARBY_DATA_);
			nearby_file << buf.str();
			nearby_file.close();
			
			//buf.str(""); 
			//write_json (buf, pt_text, false);
			//std::ofstream text_file;
			//text_file.open(TEXT_CORR_DATA_);
			//text_file << buf.str();
			//text_file.close();
		}*/
		updateMap(center, locs);
	}
	return;
	//return app.exec();
}

void updateMap(latlng center, std::vector<latlng> locs){
	//open the template file
	std::ifstream templ_file (TEMPLATE_FILE_);
	//open the map file
	std::ofstream map_file (MAP_VIS_);
	
	//create the json_t objects
	//for the center
	json_t *lat, *lng;
	lat = json_real(center.lat);
	lng = json_real(center.lng);
	json_t *center_obj = json_object();
	json_object_set(center_obj, "lat", lat);
	json_object_set(center_obj, "lng", lng);
	std::string center_replace = json_dumps(center_obj, JSON_PRESERVE_ORDER);
	replaceAll(center_replace, "\"", "");
	//std::cout << center_replace << std::endl;
	//for the locs
	json_t *loc_array = json_array();
	json_t *loc_lat, *loc_lng, *loc_obj, *loc_latlng;
	for(std::vector<latlng>::iterator it = locs.begin(); it != locs.end(); ++it){
		loc_lat = json_real(it->lat);
		loc_lng = json_real(it->lng);
		loc_latlng = json_object();
		json_object_set(loc_latlng, "lat", loc_lat);
		json_object_set(loc_latlng, "lng", loc_lng);
		loc_obj = json_object();
		json_object_set(loc_obj, "latLng", loc_latlng);
		json_array_append(loc_array, loc_obj);
		json_decref(loc_obj);
	}
	std::string locs_replace = json_dumps(loc_array, JSON_PRESERVE_ORDER);
	replaceAll(locs_replace, "\"", "");
	//std::cout << locs_replace << std::endl;
	
	std::string line;
	if(templ_file.is_open() && map_file.is_open()){
		while(std::getline(templ_file, line)){
			//try replacing the line
			replace(line, "$center", center_replace);
			replace(line, "$loc_vec", locs_replace);
			//write the line to the map file
			map_file << line << "\n";
		}
	}
	templ_file.close();
	map_file.close();
}

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

int isNotAlphabetic(char c){
        return !std::isalpha(c) && (c != '\'');
}

std::string processString(std::string& str){
	//replace non alphabetic letters with space
	std::replace_if(str.begin(), str.end(), isNotAlphabetic, ' ');
	//remove double character letters
	std::vector<std::string> sub_strings;
	boost::split(sub_strings, str, boost::is_any_of(" "));
	str = "";
	std::string result;
	for(std::vector<std::string>::iterator it = sub_strings.begin(); 
			it != sub_strings.end(); ++it){
		if(it->length() > 1){
			str += *it;		
			result += *it + " ";
		}
	}
	return result;
}

//FIXME
/*int runEKalmanFilter(){
	//initialize visualization
	int argc = 5;
	char* argv[5]= {"hello"};
	QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(MainWindow::tr("Image Viewer"));
    QCommandLineParser commandLineParser;
    commandLineParser.addHelpOption();
    commandLineParser.addPositionalArgument(MainWindow::tr("[file]"), MainWindow::tr("Image file to open."));
    commandLineParser.process(QCoreApplication::arguments());
    MainWindow imageViewer;
    imageViewer.show();
	//open data file
	//std::string file_loc_ = "/home/noha/Documents/UniversityofFreiburg/MasterThesis/TestRun/odometry.dat";
	std::ifstream data_file;
	data_file.open(ODOM_DATA_, std::ios::in);
	
	if(data_file.is_open()){
		std::string curr_line;
		std::getline(data_file, curr_line);
		std::stringstream xs (curr_line);		
		//get the number of landmarks
		int num_landmarks;
		xs >> num_landmarks;
		//get initial gps
		std::pair<double, double> corrected_gps;
		xs >> corrected_gps.first;
		xs >> corrected_gps.second;
		//initialize the robot pose
		Pose robot_pose;
		robot_pose.mu = MatrixXd::Zero(2 * num_landmarks + 3, 1);
		robot_pose.mu(0) = corrected_gps.first;
		robot_pose.mu(1) = corrected_gps.second;
		robot_pose.sigma = MatrixXd::Zero(2 * num_landmarks + 3, 2 * num_landmarks +3);
		robot_pose.sigma.block(3, 3, 2 * num_landmarks, 2 * num_landmarks).setIdentity();
		robot_pose.sigma = robot_pose.sigma * 1000;
		std::vector<bool> observed_landmarks;
		observed_landmarks.reserve(num_landmarks);
		std::fill(observed_landmarks.begin(), observed_landmarks.end(), false);
		
		std::string label;
		int index = 0;
		std::cout << "starting robot pose: " << robot_pose.mu(0) << "," << robot_pose.mu(1) << std::endl;
	
		//loop over the data
		while(std::getline(data_file, curr_line)){
			std::stringstream ss(curr_line);
			std::pair<double, double> curr_gps;
			ss >> curr_gps.first;
			ss >> curr_gps.second;
			
			//add the current location markers
			std::stringstream robot_loc_parser;
			robot_loc_parser << curr_gps.first;
			robot_loc_parser << ",";
			robot_loc_parser << curr_gps.second;
			std::stringstream robot_label_parser;
			robot_label_parser << "R";
			robot_label_parser << index;
			imageViewer.updateTrueLocs(robot_loc_parser.str(), robot_label_parser.str());
			
			std::cout << "detected_gps: " << curr_gps.first << "," << curr_gps.second << std::endl;
			
			std::pair<double, double> robot_gps (robot_pose.mu(0), robot_pose.mu(1));
			Odometry odom = getOdom(robot_gps, curr_gps);
			//call the motion model
			robot_pose = motionModel(robot_pose, odom);
			
			std::cout << "after motion model: " << robot_pose.mu(0) << "," << robot_pose.mu(1) << std::endl;
			
			//get the image name and search keyword
			std::string image_name, search_keyword;
			ss >> image_name;
			ss >> search_keyword;
			Place detected_location = reverseSearch(image_name.c_str(), search_keyword, curr_gps);
			std::pair<double, double> detected_loc_gps (detected_location.latitude, detected_location.longitude);
			//add the current landmark location markers
			std::stringstream landmark_loc_parser;
			landmark_loc_parser << detected_loc_gps.first;
			landmark_loc_parser << ",";
			landmark_loc_parser << detected_loc_gps.second;
			std::stringstream landmark_label_parser;
			landmark_label_parser << "L";
			landmark_label_parser << index;
			imageViewer.updateTrueLandmarks(landmark_loc_parser.str(), landmark_label_parser.str());
			std::cout << "landmark: " << detected_loc_gps.first << "," << detected_loc_gps.second << std::endl;
			
			//call the correction step
			robot_gps.first = robot_pose.mu(0);
			robot_gps.second = robot_pose.mu(1);
			Odometry landmark_odom = getOdom(robot_gps, detected_loc_gps);
			Location landmark_loc;
			landmark_loc.id = index;
			landmark_loc.range = landmark_odom.dist;
			landmark_loc.bearing = landmark_odom.theta1;
			std::vector<Location> observations;
			observations.push_back(landmark_loc);
			robot_pose = correctionStep(robot_pose, observations, observed_landmarks);
			std::cout << "corrected_gps: " << robot_pose.mu(0) << "," << robot_pose.mu(1) << std::endl;
			
			//plot the updates
			std::stringstream parser;
			parser << robot_pose.mu(0);
			parser << ",";
			parser << robot_pose.mu(1);
			std::stringstream result_label_parser;
			result_label_parser << "P";
			result_label_parser << index;
			imageViewer.updateUrl(parser.str(), result_label_parser.str());
			saveMapImage(imageViewer.url_string);
			imageViewer.updateGui(MAP_PATH_);
			app.processEvents();
			
			index++;
			
		}
		/*if(!USE_CACHED_DATA_){
			std::ostringstream buf; 
			write_json (buf, pt_nearby, false);
			std::ofstream nearby_file;
			nearby_file.open(NEARBY_DATA_);
			nearby_file << buf.str();
			nearby_file.close();
			
			/*buf.str(""); 
			write_json (buf, pt_text, false);
			std::ofstream text_file;
			text_file.open(TEXT_CORR_DATA_);
			text_file << buf.str();
			text_file.close()/
		}
	}
	return app.exec();
}*/

