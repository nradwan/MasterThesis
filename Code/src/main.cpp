#include "../include/main.hpp"

int main(int argc, char** argv)
{
	/*std::vector<CvRect > recog_text = spotText(argv[1]);
	std::vector<std::pair<char*, int > > detection = performOcr(recog_text, argv[1], argv[2], argv[3]);
	for(std::vector<std::pair<char*, int > >::iterator it = detection.begin(); it!=detection.end(); ++it){
		std::cout << "text: " << it->first << " ,confidence: " << it->second << std::endl;
	}
	
	return 0;*/
	double latit = atof(argv[1]);
	double longit = atof(argv[2]);
	std::pair<double, double > location (latit, longit);
	nearbySearch(location, argv[3]);
	
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
	ss << "&rankby=distance&keyword=" << keyword << "&key=APIKEY";
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
			json_t *data, *geometry, *location, *longit, *latit, *icon;
			for(size_t i = 0; i<json_array_size(results); i++){
				data = json_array_get(results, i);
				//get the icon
				icon = json_object_get(data, "icon");
				//get lang and latit
				geometry = json_object_get(data, "geometry");
				location = json_object_get(geometry, "location");
				longit = json_object_get(location, "lng");
				latit = json_object_get(location, "lat");
				//create new place
				Place curr;
				curr.place_icon = json_string_value(icon);
				curr.longitude = json_real_value(longit);
				curr.latitude = json_real_value(latit);
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
