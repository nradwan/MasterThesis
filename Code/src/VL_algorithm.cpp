#include "../include/VL_algorithm.hpp"

VLAlgr::VLAlgr(){
	data_steps.clear();
	init();
}

VLAlgr::~VLAlgr(){
	data_steps.clear();
}


void VLAlgr::init(){
	//files and folder initializations
	DATAFOLDER = "/home/radwann/Documents/MultipleLandmarkDataset/trial1/";
	COORDFILE = "data2.json";//"dataSteps.json";//"simulation.json";
	WORLD = "formattedMap.json";//"formattedMap.json";//"dummy_world.json";
	TESSDATA_PATH_ = "/usr/local/share/tessdata/";
	DETECTION_LANGUAGE_ = "eng";
	max_num_matches = 1;
	//initialize world map
	world_map = new World(DATAFOLDER, WORLD);
	//load the data file
	initializeData();	
	//std::cout << "initialize finished" << std::endl;
}

void VLAlgr::initializeData(){
	json_t* root = json_object();
	json_error_t error;
	
	std::string file_name = DATAFOLDER + COORDFILE;
	//std::ifstream ifs(file_name.c_str());
	//std::cout << "finished opening file" << std::endl;
	//std::string map_source ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	//load the json file
	//std::cout << file_name << std::endl;
	root = json_load_file(file_name.c_str(), 0, &error);
	//std::cout << "file loaded" << std::endl;
	//root = json_loads(map_source.c_str(), 0, &error);
	if(!root){
		std::cout << "ERROR at line " << error.line << ": " << error.text << std::endl;
		return;
	}
	json_t* steps = json_object();
	//extract the steps object
	steps = json_object_get(root, "steps");
	if(!json_is_array(steps)){
		std::cout << "ERROR: steps is not an array" << std::endl;
		json_decref(root);
		return;
	}
	//loop over the steps array
	
	for(size_t i = 0; i < json_array_size(steps); i++){
		
		json_t *data =  json_object();
		json_t *location = json_object();
		json_t *lat =  json_object();
		json_t *lng =  json_object();
		json_t *obsrvs =  json_object();
		json_t *gt = json_object();
		json_t *gt_lat = json_object();
		json_t *gt_lng = json_object();
			
		//std::cout << "step: " << i << std::endl;
		Step curr_step;
		data = json_array_get(steps, i);
		//get the location
		location = json_object_get(data, "location");
		lat = json_object_get(location, "lat");
		lng = json_object_get(location, "lng");
		
		double lat_value = json_real_value(lat);
		double lng_value = json_real_value(lng);
		curr_step.pose.lat = lat_value;
		curr_step.pose.lng = lng_value;
		//get the observations array
		obsrvs = json_object_get(data, "obsrvs");
		//loop over the observations array
		for(size_t j = 0; j < json_array_size(obsrvs); j++){
			json_t *inner_data = json_object();
			json_t *angle = json_object();
			json_t *image = json_object();
			
			inner_data = json_array_get(obsrvs, j);
			//get the angle
			angle = json_object_get(inner_data, "angle");
			double angle_value = json_real_value(angle);
			curr_step.obsr_angles.push_back(angle_value);
			//get the image
			image = json_object_get(inner_data, "image");
			std::string image_value = json_string_value(image);
			curr_step.images.push_back(image_value);
			
			json_decref(angle);
			json_decref(image);
			json_decref(inner_data);
			
		}
		
		
		//add curr step to the data_steps vector
		data_steps.push_back(curr_step);
		
		//get the groundtruth for evaluation
		gt = json_object_get(data, "GT");
		gt_lat = json_object_get(gt, "lat");
		gt_lng = json_object_get(gt, "lng");
		Pose gt_pose;
		gt_pose.lat = json_real_value(gt_lat);
		gt_pose.lng = json_real_value(gt_lng);
		ground_truth.push_back(gt_pose);
		
		
		json_decref(obsrvs);
		json_decref(lat);
		json_decref(lng);
		json_decref(location);
		json_decref(data);
		
		json_decref(gt_lat);
		json_decref(gt_lng);
		json_decref(gt);
	}
	//std::cout << "Not a problem of some point later" << std::endl;
	json_decref(root);
}

void VLAlgr::localize(){
	//loop over the steps array
	for(std::vector<Step>::iterator it = data_steps.begin(); it != data_steps.end(); ++it){
		//loop over the images array
		std::cout << "Step: " << std::endl;
		std::cout << "initial pose estimate: " << it->pose.lat << "," << it->pose.lng << std::endl;
		std::map<int, std::vector<std::string> > landmark_angle_idx;
		int i = 0;
		std::vector<std::string> image_array(it->images);
		for(std::vector<std::string>::iterator im_it =image_array.begin(); im_it != image_array.end(); ++im_it){
			//call text spotting for the image
			std::string image_data = DATAFOLDER + image_array.at(i);
			std::vector<CvRect> bounding_boxes = spotText(image_data.c_str());
			std::vector<std::pair< std::pair<char*, int>, std::pair<int, int> > > detection = performOcr(bounding_boxes, image_data.c_str());
			//clean up the detected text
			std::string clean_detection = processText(detection);
			std::cout << "cleaned up text: " << clean_detection << std::endl;
			//get the best n matches for the current detection
			std::vector<std::string> possible_matches = getNBestMatches(clean_detection);
			printMatches(possible_matches, image_array.at(i));
			//add the matches to the landmark_angle map
			landmark_angle_idx.insert(std::pair<int, std::vector<std::string> > (i, possible_matches));
			i++;
		}
		//call LM to estimate the pose
		Pose estimated_pose = improvePose(*it, landmark_angle_idx);
		final_poses.push_back(estimated_pose);
		//std::cout << "initial pose estimate: " << it->pose.lat << "," << it->pose.lng << std::endl;
		std::cout << "final pose estimate: " << estimated_pose.lat << "," << estimated_pose.lng << std::endl;
		//TODO:dump pose and data to file for visualization
		//TODO:write visualization class
	}
}

void VLAlgr::dummyLocalize(){
	//loop over the steps array
	for(std::vector<Step>::iterator it = data_steps.begin(); it != data_steps.end(); ++it){
		std::cout << "Step: " << std::endl;
		std::cout << "initial pose estimate: " << it->pose.lat << "," << it->pose.lng << std::endl;
		//loop over the images array
		std::map<int, std::vector<std::string> > landmark_angle_idx;
		std::vector<std::string> image_array(it->images);
		//for(int i = 0; i < it->images.size(); i++){
		int i = 0;
		for(std::vector<std::string>::iterator im_it = image_array.begin(); im_it != image_array.end(); ++im_it){
			//get the best n matches for the current detection
			std::string image_name = *im_it;
			boost::algorithm::to_lower(image_name);
			std::vector<std::string> possible_matches = getNBestMatches(image_name);
			printMatches(possible_matches, *im_it);
			//add the matches to the landmark_angle map
			landmark_angle_idx.insert(std::pair<int, std::vector<std::string> > (i, possible_matches));
			i++;
		}
		//call LM to estimate the pose
		Pose estimated_pose = improvePose(*it, landmark_angle_idx);
		final_poses.push_back(estimated_pose);
		//std::cout << "initial pose estimate: " << it->pose.lat << "," << it->pose.lng << std::endl;
		std::cout << "final pose estimate: " << estimated_pose.lat << "," << estimated_pose.lng << std::endl;
		std::cout << "============================================================================================\n" << std::endl; 
		//TODO:dump pose and data to file for visualization
		//TODO:write visualization class
	}
}

void VLAlgr::printMatches(std::vector<std::string> matches, std::string image){
	std::cout << "matches for image: " << image << " are: " << std::endl;
	for(std::vector<std::string>::iterator it = matches.begin(); it != matches.end(); ++it){
		std::cout << *it << ",";
	}
	std::cout << std::endl;
}

std::vector<CvRect > VLAlgr::spotText(const char* input_im){
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

std::vector<std::pair< std::pair<char*, int>, std::pair<int, int> > > VLAlgr::performOcr(std::vector<CvRect > bounding_boxes, const char* input_im){
	
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

std::string VLAlgr::processText(std::vector<std::pair< std::pair<char*, int>, std::pair<int, int> > > detection){
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

std::string VLAlgr::ocrCorrection(std::string query){
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

bool VLAlgr::replace(std::string& str, const std::string& from, const std::string& to){
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void VLAlgr::replaceAll(std::string& str, const std::string& from, const std::string& to){
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

std::string VLAlgr::removeNonChars(std::string& str){
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

std::vector<std::string> VLAlgr::getNBestMatches(std::string text){
	//create a priority queue of matches
	//for each shop in the world, compute the edit distance
	//add to the pq
	//return the top n matches
	std::priority_queue<Match, std::vector<Match>, CompareMatch> ordered_matches;
	for(std::vector<std::string>::iterator it = world_map->shop_names.begin(); it != world_map->shop_names.end(); ++it){
		std::string form_name = *it;
		boost::algorithm::to_lower(form_name);
		replaceAll(form_name, " ", "");
		int score = editDistance(text, form_name);
		Match m;
		m.shop_name = *it;
		m.score = score;
		ordered_matches.push(m);
	}
	
	//debugging
	//std::cout << "printing pq contents for: " << text << std::endl;
	//std::vector<Match> tmp_vector(ordered_matches.size());
	//std::copy(&(ordered_matches.top()), &(ordered_matches.top()) + ordered_matches.size(), &tmp_vector[0]);
	//printPQ(tmp_vector);
	
	std::vector<std::string> result;
	for(int i = 0; i < max_num_matches; i++){
		if(!ordered_matches.empty()){
			Match tmp = ordered_matches.top();
			result.push_back(tmp.shop_name);
			ordered_matches.pop();
		}
	}
	return result;
}

void VLAlgr::printPQ(std::vector<Match> x){
	for(std::vector<Match>::iterator it = x.begin(); it != x.end(); ++it){
		std::cout << "name: " << it->shop_name << ", score: " << it->score << std::endl;
	}
}

int VLAlgr::editDistance(std::string s, std::string t){
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

VLAlgr::Pose VLAlgr::improvePose(Step step, std::map<int, std::vector<std::string> > landmark_angle_idx){
	//initialize pose matrix
	Eigen::MatrixXf pose;
	pose.resize(2, 1);
	pose(0, 0) = step.pose.lng;
	pose(1, 0) = step.pose.lat;
	//std::cout << "pose matrix: \n" << pose << std::endl;
	//get the total number of observed landmarks
	int m = countLandmarks(landmark_angle_idx);
	//initialize matrix for angles and for landmarks
	Eigen::MatrixXf observations, params;
	observations.resize(m, 1);
	params.resize(m, 2);
	int counter = 0;
	for(std::map<int, std::vector<std::string> >::iterator it = landmark_angle_idx.begin(); it != landmark_angle_idx.end(); ++it){
		for(std::vector<std::string>::iterator ij = it->second.begin(); ij != it->second.end(); ++ij){
			observations(counter, 0) = step.obsr_angles.at(it->first);
			std::pair<double, double> loc = world_map->shops_coords[*ij];
			params(counter, 0) = loc.second;
			params(counter, 1) = loc.first;
			counter++;
		}
	}
	//std::cout << "observations matrix: \n" << observations << std::endl;
	//std::cout << "params matrix: \n" << params << std::endl;
	//std::cout << "check counter==m: " << ((counter == m) ? "yes": "no") << std::endl;
	//Eigen::MatrixXf x, y, z;
	LMAlgr *lm_instance = new LMAlgr(observations, params, pose);
	Pose result;
	result.lat = pose(1, 0);
	result.lng = pose(0, 0);
	return result;
}

int VLAlgr::countLandmarks(std::map<int, std::vector<std::string> > landmark_angle_idx){
	int res = 0;
	for(std::map<int, std::vector<std::string> >::iterator it = landmark_angle_idx.begin(); it != landmark_angle_idx.end(); ++it){
		res += it->second.size();
	}
	return res;
}

void VLAlgr::evaluateResults(){
	std::vector<Pose>::iterator gt_it = ground_truth.begin();
	std::vector<Pose>::iterator fp_it = final_poses.begin();
	double dist_sum = 0;
	for(int i = 0; i < ground_truth.size(); i++){
		double dist = sqrt(((gt_it->lat - fp_it->lat) * (gt_it->lat - fp_it->lat)) + ((gt_it->lng - fp_it->lng) * (gt_it->lng - fp_it->lng)));
		std::cout << "dist at step " << i << ": " << dist << std::endl;
		dist_sum += dist;
	}
	std::cout << "distance sum: " << dist_sum << std::endl;
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

int main(int argc, char** argv){
	VLAlgr *test = new VLAlgr();
	//call localization function
	test->localize();
	std::cout << "localize finished" << std::endl;
	//test->dummyLocalize();
	//evaluate the result
	test->evaluateResults();
}
