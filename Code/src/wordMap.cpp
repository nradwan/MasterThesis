#include "../include/wordMap.hpp"

int main(int argc, char** argv){
	//initialize the map data
	initializeMap();
	localize();

}

void initializeMap(){
	//reads the world.json file and inserts the data into a map
	json_t* root;
	json_error_t error;
	
	std::string file_name = DATAFOLDER + WORLD;
	std::ifstream ifs(file_name.c_str());
	std::string map_source ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	//load the json file
	root = json_loads(map_source.c_str(), 0, &error);
	if(!root){
		std::cout << "ERROR at line " << error.line << ": " << error.text << std::endl;
		return;
	}
	json_t* map;
	//extract the map object
	map = json_object_get(root, "map");
	if(!json_is_array(map)){
		std::cout << "ERROR: map is not an array" << std::endl;
		json_decref(root);
		return;
	}
	//loop over the map array
	json_t *data, *name, *location, *lat, *lng;
	for(size_t i = 0; i < json_array_size(map); i++){
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
	json_decref(root);
}

void localize(){
	//reads the coordinates file
	//for each step, calls text spotting, ocr, and ocr correction
	//finds the closest match in the map
	//calls triangulation function
	//calls to visualize the estimated position
	
	//open the coordinates file
	std::string file_name = DATAFOLDER + COORDFILE;
	std::ifstream input_file (file_name.c_str());
	std::string curr_line;
	while(std::getline(input_file, curr_line)){
		std::vector<std::string> tokens;
		boost::split(tokens, curr_line, boost::is_any_of(" "));
		//get the name of the node
		std::string node_name = tokens.at(0);
		//get the location of the node
		std::string node_loc = tokens.at(1);	
		//get the images for the node
		std::string curr_data;
		std::vector<std::string> matching_shops;
		for(int i = 2; i < tokens.size(); i++){
			curr_data = tokens.at(i);
			//call text spotting for the curr data file
			std::string image_data = DATAFOLDER + curr_data;
			std::vector<CvRect> bounding_boxes = spotText(image_data.c_str());
			std::vector<std::pair< std::pair<char*, int>, std::pair<int, int> > > detection = performOcr(bounding_boxes, image_data.c_str());
			//clean up the detected text
			std::string clean_detection = processText(detection);
			//get the closest match from the shop names
			std::string closest_match = getBestMatch(clean_detection);
			//std::cout << "closest match for: " << clean_detection << " is: " << closest_match << std::endl;
			matching_shops.push_back(closest_match);
		}
		//call triangulate to get the closest pose
		std::string estimated_loc = triangulate(matching_shops);
	}
	
}

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
			//printf("%d %d %d %d\n", rect->x, rect->y, rect->width, rect->height);
			//Visualization! draw rectangle
			cvRectangle(cv_image,cvPoint(rect->x,rect->y), cvPoint(rect->x+rect->width, rect->y+rect->height),cvScalar(0, 255, 0, 0),2,8,0);
			//add rect to text_rect
			CvRect rectangle= cvRect(rect->x, rect->y, rect->width, rect->height);
			text_rect.push_back(rectangle);
		}
		//printf("total : %d\n", words->rnum);
		//std::cout << "number of rectangles: " << text_rect.size() << std::endl;
		ccv_array_free(words);
	}
	ccv_matrix_free(image);
	//Visualization
	//cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
   	//cvShowImage( "Display window", cv_image );                   // Show our image inside it.

    //cv::waitKey(0);                                          // Wait for a keystroke in the window
    
    return text_rect;
}

std::vector<std::pair< std::pair<char*, int>, std::pair<int, int> > > performOcr(std::vector<CvRect > bounding_boxes, const char* input_im){
	
	char* tess_data_path = TESSDATA_PATH_;
	char* language = DETECTION_LANGUAGE_;
	std::vector<std::pair< std::pair<char*, int>, std::pair<int, int> > > detected_text;
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
		std::pair<int, int > dimensions (it->x, it->y);
		std::pair< std::pair<char*, int >, std::pair<int, int> > data (detection, dimensions);
		detected_text.push_back(data);
		//fprintf(stdout, "Box: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s\n", it->x, it->y, it->width, it->height, conf, ocrResult);
	}
	return detected_text;
}

std::string processText(std::vector<std::pair< std::pair<char*, int>, std::pair<int, int> > > detection){
	//initially order the text by the x coordinates
	std::map<int, char*> ordered_text;
	for(std::vector<std::pair< std::pair<char*, int>, std::pair<int, int> > >::iterator it = detection.begin();
			it != detection.end(); ++it){
		std::pair<int, char*> curr_text (it->second.first, it->first.first);
		//check if the map already has a string for the following coordinate
		std::map<int, char*>::iterator i_find = ordered_text.find(it->second.first);
		if(i_find != ordered_text.end()){
			//element found, keep the value of the longer string
			std::string map_elem = ordered_text[it->second.first];
			std::string curr_elem = it->first.first;
			if(curr_elem.length() > map_elem.length()){
				//replace the map element with the new one
				ordered_text[it->second.first] = it->first.first;
			}
		}
		else{
			ordered_text.insert(curr_text);
		}
	}
	std::string concat_string = "";
	//concatenate the detected text in order with "+"
	for(std::map<int, char*>::iterator it = ordered_text.begin(); it != ordered_text.end(); ++it){
		concat_string += it->second;
		concat_string += " ";
	}
	//std::cout << "first stage: " << concat_string << std::endl;
	//replace any " " with "+"
	std::replace(concat_string.begin(), concat_string.end(), ' ', '+');
	//std::cout << "second stage: " << concat_string << std::endl;
	//call the online correction
	std::string online_res = ocrCorrection(concat_string);
	//std::cout << "returned from ocrCorrection" << std::endl;
	//check if there are any corrections made
	if(online_res.compare("") == 0)
		online_res = concat_string;
	//replace all the "+" with " "
	replaceAll(online_res, "+", " ");
	//std::cout << "third stage: " << online_res << std::endl;
	//remove non characters from the string
	concat_string = removeNonChars(online_res);
	boost::algorithm::to_lower(concat_string);
	//std::cout << "forth stage: " << concat_string << std::endl;
	return concat_string;
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
    //std::cout << json_dumps(root, JSON_PRESERVE_ORDER) << std::endl;
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
		if(!spelling){
			//std::cout << "no spelling parameter found" << std::endl;
			return "";
		}
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

bool replace(std::string& str, const std::string& from, const std::string& to){
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to){
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

std::string removeNonChars(std::string& str){
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

static int writer(char *data, size_t size, size_t nmemb, std::string *buffer){
    int result = 0;
    if (buffer != NULL)
    {
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }

    return result;
}

std::string getBestMatch(std::string text){
	//std::cout << "finding best match for: " << text << std::endl;
	std::string best_match_name = shop_names.at(0);
	std::string best_form_name = shop_names.at(0);
	//std::cout << "current best match: " << best_match_name << std::endl;
	boost::algorithm::to_lower(best_form_name);
	replaceAll(best_form_name, " ", "");
	int best_match_score = editDistance(text, best_form_name);
	int curr_score;
	std::string curr_shop, curr_form;
	for(int i = 1; i < shop_names.size(); i++){
		curr_shop = shop_names.at(i);
		curr_form = shop_names.at(i);
		boost::algorithm::to_lower(curr_form);
		replaceAll(curr_form, " ", "");
		curr_score = editDistance(text, curr_form);
		if(curr_score < best_match_score){
			best_match_score = curr_score;
			best_match_name = curr_shop;
			//std::cout << "current best match: " << best_match_name << std::endl;
		}
	}
	return best_match_name;
	
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

std::string triangulate(std::vector<std::string> matching_shops){

}
