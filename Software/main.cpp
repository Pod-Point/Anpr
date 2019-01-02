#include <iostream>
#include <fstream>      // std::ofstream
#include <ctime>
#include <cstdint>
#include <alpr.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>
#include "PlateHistogram.h"
//#define RASP
#define DEBUG_IMAGE
#define TOP_ALGO

using namespace cv;
using namespace std;

int main(int, char**)
{
	//Timer
	chrono::system_clock::time_point startTime;
	chrono::system_clock::time_point endTime;

	chrono::system_clock::time_point startTime_algo;
	chrono::system_clock::time_point endTime_algo;
	PlateHistogram::PlateHistogramManager phistManager;

	// Image Name
#if !defined(RASP)
	string imgNamePrefix = "/home/jose/Documents/Projects/PodPoint/Anpr/Result/img";
#else
	string imgNamePrefix = "/home/pi/Documents/Projects/Anpr/Result/img";
#endif
	int imgNumber = 0;
	string imgNameSuffix = ".jpg";

	// LogFile
#if !defined(RASP)
	ofstream ofs("/home/jose/Documents/Projects/PodPoint/Anpr/Result/Log.txt");
#else
	ofstream ofs("/home/pi/Documents/Projects/Anpr/Result/Log.txt");
#endif
	if(!ofs.is_open())
	cout << "file not opened" << endl;


    VideoCapture cap(0); // open the default camera
#if defined(RASP)
//    VideoCapture cap("rtsp://admin:123456@192.168.194.198:80/live/ch1");
//    VideoCapture cap("rtsp://admin:123456@10.42.0.115:80/live/ch0");
//    VideoCapture cap("rtsp://admin:123456@10.42.0.115:80/live/ch1");
#else
    //    VideoCapture cap("rtsp://admin:123456@192.168.194.198:554/live/ch0", cv::CAP_FFMPEG);
//    VideoCapture cap("rtsp://admin:123456@10.42.0.17:80/live/ch1", cv::CAP_FFMPEG);
#endif
    if(!cap.isOpened()){  // check if we succeeded
    	cout << "Error open" << endl;
        return -1;
    }
    double bright = cap.get(CV_CAP_PROP_BRIGHTNESS);
    cout << "brightness: " << bright << "\n";
//    cap.set(CV_CAP_PROP_BRIGHTNESS,0.5);
//    cap.set(CV_CAP_PROP_BRIGHTNESS+1,0.5);
//    cap.set(CV_CAP_PROP_BRIGHTNESS+2,0.5);
//
//    bright = cap.get(CV_CAP_PROP_BRIGHTNESS);
//    cout << "brightness: " << bright << "\n";
//    cap.set(CV_CAP_PROP_FRAME_WIDTH,1280);
//    cap.set(CV_CAP_PROP_FRAME_HEIGHT,960);
//    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
//    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
//    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
//    cap.set(CV_CAP_PROP_FRAME_HEIGHT,360);
	// Initialize the library using United States style license plates.
	// You can use other countries/regions as well (for example: "eu", "au", or "kr")
	alpr::Alpr openalpr("gb", "/etc/openalpr/openalpr.conf");

	// Optionally specify the top N possible plates to return (with confidences).  Default is 10
	openalpr.setTopN(5);

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

    startTime = chrono::system_clock::now();
    endTime = chrono::system_clock::now();
#if defined(DEBUG_IMAGE)
    namedWindow("Plate",1);
#endif
    int numTest = 0;
    uint64_t sumtime = 0;

    cap >> frame;

    cout << "W: " << frame.cols << "\n";
    cout << "H: " << frame.rows << endl;

    std::vector<alpr::AlprRegionOfInterest> regionsOfInterest;
    regionsOfInterest.push_back(alpr::AlprRegionOfInterest(0, 0, frame.cols/2, frame.rows));
    regionsOfInterest.push_back(alpr::AlprRegionOfInterest(frame.cols/2, 0, frame.cols/2, frame.rows));
//    regionsOfInterest.push_back(alpr::AlprRegionOfInterest(0, 0, frame.cols, frame.rows));
    alpr::AlprResults results;
    for(;;)
    {
    	startTime_algo = chrono::system_clock::now();
    	cap >> frame;
#if defined(DEBUG_IMAGE)
        imshow("Plate", frame);
        waitKey(30);
#endif

        string imgName = imgNamePrefix + to_string(imgNumber) + imgNameSuffix;


        //reduce probability
        phistManager.removeInconsistency();
        // check if a plate is available on left and on the right
        for(PlateHistogram::Position pos = PlateHistogram::Position::kLeft;
        		pos < PlateHistogram::Position::kMaxPosition;
        		pos = static_cast<PlateHistogram::Position>(static_cast<int>(pos) + 1)){
			if(phistManager.isPlateAvailable(pos)){
				string finalPlate = phistManager.getBestCalculatedPlate(pos);
		        time_t result = std::time(nullptr);
		        char * timeStr = asctime(localtime(&result));
		        size_t len = strlen(timeStr);
		        if (len > 0 && timeStr[len-1] == '\n') timeStr[--len] = '\0';
		        ofs << timeStr << "\t" << "Alpr Result "<< static_cast<int>(pos) << " " << finalPlate << endl;
				cout<< "###########################\n";
				cout<< "Alpr Result "<< static_cast<int>(pos) << " " << finalPlate << "\n";
				cout<< "###########################" << endl;
			}
        }
        results = openalpr.//recognize(imgName);
         		recognize(frame.data, frame.elemSize(), frame.cols, frame.rows, regionsOfInterest);
#if defined(DEBUG_IMAGE)
        imwrite(imgName, frame);
        imgNumber++;
//        continue;
#endif
        if( results.plates.size() < 1){
        	endTime_algo = chrono::system_clock::now();
        	auto delta = chrono::duration_cast<chrono::milliseconds>(endTime_algo-startTime_algo);
        	sumtime += delta.count();
        	if(numTest > 1000){
        		sumtime /= 1000;
        		cout<< "Algo time no plate mean time " << sumtime << endl;
        		numTest = 0;
        		sumtime = 0;
        	}

        	numTest++;
        	continue;
        }

        // Write log file
        time_t result = std::time(nullptr);
        char * timeStr = asctime(localtime(&result));
        size_t len = strlen(timeStr);
        if (len > 0 && timeStr[len-1] == '\n') timeStr[--len] = '\0';
        ofs << timeStr << "\t" << imgName << "\t";
#if !defined(DEBUG_IMAGE)
        imwrite(imgName, frame);
        imgNumber++;
#endif
        // Iterate through the results.  There may be multiple plates in an image,
        // and each plate return sthe top N candidates.
		for (uint32_t i = 0; i < results.plates.size(); i++)
		{
			alpr::AlprPlateResult plate = results.plates[i];


			PlateHistogram::Position location = PlateHistogram::Position::kLeft;

			if(plate.plate_points[0].x > frame.cols/8*3){
				location = PlateHistogram::Position::kRight;
			}
#if defined(TOP_ALGO)
			phistManager.addPossiblePlate(plate.bestPlate.characters, plate.bestPlate.overall_confidence, plate.bestPlate.matches_template, location);
			cout << static_cast<int>(location) << " " << plate.bestPlate.characters << endl;
			ofs << " location: " << static_cast<int>(location) << " " << plate.bestPlate.characters << "\t" << plate.bestPlate.overall_confidence << "\t" << "pattern_match: " << plate.bestPlate.matches_template << "\t";
#else
//			ofs << "rest :";
			for (uint32_t k = 0; k < plate.topNPlates.size(); k++)
			{
				alpr::AlprPlate candidate = plate.topNPlates[k];
				std::cout << "    - " << candidate.characters << "\t confidence: " << candidate.overall_confidence;
			    std::cout << "\t pattern_match: " << candidate.matches_template << std::endl;

			    ofs << candidate.characters << "\t" << candidate.overall_confidence << "\t" << "pattern_match: " << candidate.matches_template << "\t";

			    phistManager.addPossiblePlate(candidate.characters, candidate.overall_confidence, candidate.matches_template, location);


//				phistManager.addPossiblePlate(plate.bestPlate.characters, plate.bestPlate.matches_template, location);
///				cout << static_cast<int>(location) << " " << plate.bestPlate.characters << endl;
//				ofs << " location: " << static_cast<int>(location) << " " << plate.bestPlate.characters << "\t" << plate.bestPlate.overall_confidence << "\t" << "pattern_match: " << plate.bestPlate.matches_template << "\t";
			}
#endif
		}
		cout << "--------------" << endl;
		startTime = chrono::system_clock::now();
		ofs << endl;

		endTime_algo = chrono::system_clock::now();
		auto dt = chrono::duration_cast<chrono::milliseconds>(endTime_algo - startTime_algo);
		cout<< "Algo time with plate " << dt.count() << "\n";
    }
    ofs.close();
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}


