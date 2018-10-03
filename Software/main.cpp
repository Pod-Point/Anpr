#include <iostream>
#include <fstream>      // std::ofstream
#include <ctime>
#include <cstdint>
#include <alpr.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace cv;
using namespace std;

int main(int, char**)
{
	//Timer
	chrono::system_clock::time_point startTime;
	chrono::system_clock::time_point endTime;

	unordered_map<string, int> plateHist;
	// Image Name
	string imgNamePrefix = "/home/jose/Documents/Projects/PodPoint/Anpr/Result/img";
	int imgNumber = 0;
	string imgNameSuffix = ".jpg";

	// LogFile
	ofstream ofs("/home/jose/Documents/Projects/PodPoint/Anpr/Result/Log.txt");
	if(!ofs.is_open())
	cout << "fanculo" << endl;


    VideoCapture cap(0); // open the default camera
//    VideoCapture cap("rtsp://admin:123456@192.168.194.198:554/live/ch0", cv::CAP_FFMPEG);

    if(!cap.isOpened()){  // check if we succeeded
    	cout << "Error open" << endl;
        return -1;
    }

	 // Initialize the library using United States style license plates.
	 // You can use other countries/regions as well (for example: "eu", "au", or "kr")
	 alpr::Alpr openalpr("gb", "/etc/openalpr/opena-lpr.conf");

	 // Optionally specify the top N possible plates to return (with confidences).  Default is 10
	 openalpr.setTopN(10);

	 // Optionally, provide the library with a region for pattern matching.  This improves accuracy by
	 // comparing the plate text with the regional pattern.
	 openalpr.setDefaultRegion("md");

	 // Make sure the library loaded before continuing.
	 // For example, it could fail if the config/runtime_data is not found
	 if (openalpr.isLoaded() == false)
	 {
		 std::cerr << "Error loading OpenALPR" << std::endl;
		 return 1;
	 }

    Mat edges;
    Mat frame;
    Mat hsv;
//    cout << ++plateHist["cane"] << endl;
//    bool ret = plateHist.insert({"ciao", 1}).second;
//    cout << ret <<endl;
//    ret = plateHist.insert({"ciao", 2}).second;
//    cout << ret <<endl;
//
//    for (std::pair<std::string, int> element : plateHist)
//		std::cout << element.first << " :: " << element.second << std::endl;
//
//	return 0;
    startTime = chrono::system_clock::now();
    endTime = chrono::system_clock::now();
    bool readFlag = false;
    namedWindow("Plate",1);
    for(;;)
    {

        cap >> frame; // get a new frame from camera CV_8UC3
//        cout << frame.size() << endl;
//        cout << frame.channels() << endl;
//        cout << frame.type() << endl;
//        cout << frame.at(0,0) << endl;

		cvtColor(frame, hsv, COLOR_BGR2HSV);
		Scalar pixelmean = mean(hsv);
        Scalar pixelsum = sum(hsv);
//        cout << "mean" << pixelmean << endl;
//        cout << "sum" << pixelsum << endl;
        if(pixelmean.val[2] < 80)
        	cout << "Night" << endl;

        imshow("Plate", frame);
        waitKey(30);
//        if(waitKey(30) >= 0) break;
//          if(waitKey(30) >= 0) continue;
        // Recognize an image file.  You could alternatively provide the image bytes in-memory.
//        alpr::AlprResults results = openalpr.recognize("C:/Users/Jose/Documents/Projects/Pod-Point/PlateRec/Data/data0.jpg");
//        imwrite("C:/Users/Jose/Documents/Projects/Pod-Point/PlateRec/build/img.jpg", frame);
        string imgName = imgNamePrefix + to_string(imgNumber) + imgNameSuffix;
//        imwrite(imgName, frame);

        std::vector<alpr::AlprRegionOfInterest> regionsOfInterest;
        regionsOfInterest.push_back(alpr::AlprRegionOfInterest(0, 0, frame.cols, frame.rows));
        alpr::AlprResults results;

//        // Reset timer, no car has been recently detected
//        if(plateHist.empty()){
//        	startTime = chrono::system_clock::now();
//        }

        //reduce probability
        vector<string> v2rm;
		for (std::pair<std::string, int> plateElem : plateHist){
			plateElem.second--;
			if(plateElem.second < 0){
				v2rm.push_back(plateElem.first);
			}
		}

		// remove unused
		for(auto e2rm: v2rm){
			plateHist.erase(e2rm);
		}

		// sendo if it is time to
		endTime = chrono::system_clock::now();
		chrono::duration<float> time_elapsed = endTime - startTime;

		if(readFlag && time_elapsed.count() > 10){
			readFlag = false;
			// fin the max
			string finalPlate;
			int maxOccurence = 0;

			for(auto  plateElem : plateHist){
				if(plateElem.second > maxOccurence){
					maxOccurence = plateElem.second;
					finalPlate = plateElem.first;
				}
			}

			cout<< "###########################\n";
			cout<< "Alpr Result " << finalPlate << "\n";

			cout<< "###########################" << endl;
			plateHist.clear();
		}


        try{

        results = openalpr.//recognize(imgName);
        		recognize(frame.data, frame.elemSize(), frame.cols, frame.rows, regionsOfInterest);
        }catch(...){
        	std::cout << "OCR problems" << std::endl;
        	continue;
        }
        if( results.plates.size() < 1){
        	continue;
        }
        imwrite(imgName, frame);

//        imshow("Plate", frame);
        // Write log file
        time_t result = std::time(nullptr);
        char * timeStr = asctime(localtime(&result));
        size_t len = strlen(timeStr);
        if (len > 0 && timeStr[len-1] == '\n') timeStr[--len] = '\0';
        ofs << timeStr << "\t" << imgName << "\t";
        imwrite(imgName, frame);
        imgNumber++;
        // Iterate through the results.  There may be multiple plates in an image,
        // and each plate return sthe top N candidates.
		for (uint32_t i = 0; i < results.plates.size(); i++)
		{
			alpr::AlprPlateResult plate = results.plates[i];
//			cout << "plate" << i << ": " << plate.topNPlates.size() << " results" << endl;


			for (uint32_t k = 0; k < plate.topNPlates.size(); k++)
			{
				alpr::AlprPlate candidate = plate.topNPlates[k];
//				std::cout << "    - " << candidate.characters << "\t confidence: " << candidate.overall_confidence;
//			    std::cout << "\t pattern_match: " << candidate.matches_template << std::endl;

			    ofs << candidate.characters << "\t" << candidate.overall_confidence << "\t" << "pattern_match: " << candidate.matches_template << "\t";
			    plateHist[candidate.characters] += (candidate.matches_template*10) + 2;

			}


			// TODO: fix this, it will check only one plate
			break;
		}
		startTime = chrono::system_clock::now();
		readFlag = true;
		ofs << endl;
    }
    ofs.close();
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

