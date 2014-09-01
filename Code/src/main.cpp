#include "../include/main.hpp"
using namespace boost::gil;

int main(int argc, char** argv)
{
	/*std::vector<CvRect > recog_text = spotText(argv[1]);
	std::vector<std::pair<char*, int > > detection = performOcr(recog_text, argv[1], argv[2], argv[3]);
	for(std::vector<std::pair<char*, int > >::iterator it = detection.begin(); it!=detection.end(); ++it){
		std::cout << "text: " << it->first << " ,confidence: " << it->second << std::endl;
	}
	
	return 0;*/
	
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
	p.name = "test";
	std::string x = getPhotoRef("CnRmAAAAFR8pjw0S0k2KdOhdHRJAf_zC5aAZMP7Ecf-Awzc3basCr3eoCTivtir6TDG_L02lxvlufw_K3u-MlCzJE_06Tgfl7gi3yeDmt_8d5Udj5PN6j8IccAuKgg28wfuuv57CfgvvT7dI1ezZl6CWyYGM_RIQcj4OHhxFgcBJrIYapQva6hoUhCyx01dj_on1A0Q9Fd80vpR04aw");
	std::cout << x << std::endl;
	std::vector<std::string> pl;
	pl.push_back(x);
	p.place_icon = pl;
	savePlaceIcon(p);
	
	for(std::vector<std::string>::iterator it= p.place_icon.begin(); it!=p.place_icon.end(); ++it){
		std::cout << *it << std::endl;
	}*/
	
	std::cout << ocrCorrection(argv[1]) << std::endl;
	return 0;
}

std::vector<CvRect > spotText(char* input_im){
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

std::vector<std::pair<char*, int> > performOcr(std::vector<CvRect > bounding_boxes, 
									char* input_im, char* tess_data_path, char* language){
	std::vector<std::pair<char*, int> > detected_text;
	//read the picture
	Pix *image = pixRead(input_im);
	//initialize tesseract api
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	api->Init(tess_data_path, language, tesseract::OEM_TESSERACT_ONLY);
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
		//get confidence
		int conf = api->MeanTextConf();
		std::pair<char* , int > detection (ocrResult, conf);
		detected_text.push_back(detection);
		fprintf(stdout, "Box: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s", it->x, it->y, it->width, it->height, conf, ocrResult);
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

std::vector<Place> nearbySearch(std::pair<double, double> location, std::string keyword){
	//create vector for the returned results
	std::vector<Place> nearbyLocs;
	//format the url
	std::stringstream ss;
	ss << "https://maps.googleapis.com/maps/api/place/nearbysearch/json?location=";
	ss << location.first << "," << location.second;
	ss << "&rankby=distance&keyword=" << keyword << "&key=AIzaSyBwfgwqnuQ9Dkh4gYOFArAmyDBU-dRzhpM";
	std::string url = ss.str();
	std::cout << "Will retrive from url: '" << url << "'" << std::endl;
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
			return nearbyLocs;
		}
		json_t *status, *results;
		//get the returned status
		status = json_object_get(root, "status");
		if(!json_is_string(status))
		{
			std::cout << "error: return data : status is not a string" << std::endl;
			json_decref(root);
			return nearbyLocs;
		}
		std::string status_string = json_string_value(status);
		if(status_string.compare("OK") == 0){
			//query returned results		
			results = json_object_get(root, "results");
			if(!json_is_array(results)){
				std::cout << "error: return data : results is not an array" << std::endl;
				json_decref(root);
				return nearbyLocs;
			}
			//loop over the results array
			json_t *data, *geometry, *location, *longit, *latit, *photos, *photo, *photo_ref, *name;
			std::vector<std::string> photo_url;
			for(size_t i = 0; i<json_array_size(results); i++){
				data = json_array_get(results, i);
				//get the icon
				photos = json_object_get(data, "photos");
				//loop over the photos array
				for(size_t j = 0; j<json_array_size(photos); j++){
					photo = json_array_get(photos, j);
					photo_ref = json_object_get(photo, "photo_reference");
					//call method to return photo query
					photo_url.push_back(getPhotoRef(json_string_value(photo_ref)));
				}
				//get lang and latit
				geometry = json_object_get(data, "geometry");
				location = json_object_get(geometry, "location");
				longit = json_object_get(location, "lng");
				latit = json_object_get(location, "lat");
				//get the name
				name = json_object_get(data, "name");
				//create new place
				Place curr;
				curr.place_icon = photo_url;
				curr.longitude = json_real_value(longit);
				curr.latitude = json_real_value(latit);
				curr.name = json_string_value(name);
				savePlaceIcon(curr);
				nearbyLocs.push_back(curr);
			}
			
		}
		else{
			//query failed
			std::cout << "returned status string: " << status_string << std::endl;
			return nearbyLocs;
		}
	}
	else{
		std::cout << "failed to perform with cUrl" << std::endl;
		return nearbyLocs;
	}
	return nearbyLocs;
}

bool logoFound(char* logo_im, char* input_im){
	cv::Mat img_1 = cv::imread( logo_im, CV_LOAD_IMAGE_GRAYSCALE );
	cv::Mat img_2 = cv::imread( input_im, CV_LOAD_IMAGE_GRAYSCALE );
	
	if( !img_1.data || !img_2.data ){
		std::cout<< " --(!) Error reading images " << std::endl; return -1;
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

	std::cout << "all matches: " << (int)matches.size() << " good matches: " <<  
				(int)good_matches.size() << " ratio: " << (double)good_matches.size()/(double)matches.size() << std::endl;

	for( int i = 0; i < (int)good_matches.size(); i++ )
	{ printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); }

	cv::waitKey(0);

	
	return (double)good_matches.size()/(double)matches.size()>0.4;
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
		unsigned found = image_url.find_last_of("/");
		image_name = image_url.substr(found+1);
		found = image_name.find_last_of(".");
		std::stringstream ss;
		ss << place.name << "_" << counter << image_name.substr(found);
		image_name = ss.str();
	
		std::string new_image_path = saved_logos_prefix+image_name;

		image = curl_easy_init(); 
		if( image ){ 
			// Open file 
			fp = fopen(new_image_path.c_str(), "wb"); 
			if( fp == NULL ) std::cout << "File cannot be opened"; 

			curl_easy_setopt(image, CURLOPT_URL, image_url.c_str()); 
			curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL); 
			curl_easy_setopt(image, CURLOPT_WRITEDATA, fp); 


			// Grab image 
			imgresult = curl_easy_perform(image); 
			if( imgresult ){ 
				std::cout << "Cannot grab the image!\n"; 
			} 
		} 

		// Clean up the resources 
		curl_easy_cleanup(image); 
		// Close the file 
		fclose(fp); 
		
		std::cout << "image grabbed and saved" << std::endl;
		
		//check if the image is in jpg (if yes we need to convert it to png)
		if(image_name.find("jpg")!=std::string::npos || image_name.find("jpeg")!=std::string::npos){
			unsigned found = image_name.find_last_of(".");
			rgb8_image_t im;
			jpeg_read_image(new_image_path, im);
			png_write_view(saved_logos_prefix+image_name.substr(0,found+1)+"png", view(im));
			new_image_path = saved_logos_prefix+image_name.substr(0,found+1)+"png";
		}
	
		//save the new image icon location in the places struct
		place.place_icon[counter] = new_image_path;
		
		counter++;
	}
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
		<< "+zurich&cx=014594212095381448740%3A2kyeah__gum&key=AIzaSyBwfgwqnuQ9Dkh4gYOFArAmyDBU-dRzhpM";
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
