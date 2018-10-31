/*
 * main_test.cpp
 *
 *  Created on: 15 Oct 2018
 *      Author: jose
 */
#include "gtest/gtest.h"
#include <opencv2/opencv.hpp>
#include <chrono>
#include <alpr.h>
#include <thread>

using namespace cv;
using namespace std;

void getFrame(){
	for(int i=0; i<100; i++)
		cout << "get" << "\n";
}

void useFrame(){
	for(int i=0; i<100; i++)
		cout << "use" << "\n";
}

TEST(opencv, oneThreadGet)
{
	const uint64_t kNumOfCapture = 100;
	chrono::system_clock::time_point startTime;
	chrono::system_clock::time_point endTime;
	uint32_t counter = 0;
	uint64_t sumTime = 0;
	VideoCapture cap(0); // open the default camera
	Mat frame;
//	namedWindow("Test",1);

	for(;;){
		startTime = chrono::system_clock::now();
		cap >> frame;
		endTime = chrono::system_clock::now();
		auto delta = chrono::duration_cast<chrono::milliseconds>(endTime-startTime);
//		cout << "Frame count: " << cap.get(CV_CAP_PROP_POS_FRAMES) << "\n";
		sumTime += delta.count();
//		cout << "delta: " << delta.count() << "\n";
		counter++;
		if(counter > kNumOfCapture){
			sumTime /= kNumOfCapture;
			cout << "Time in ms over 1000 samples: " << sumTime << endl;
			return;
		}

//        imshow("Test", frasme);
//        waitKey(30);
	}
}

TEST(opencv, oneThreadGetSave)
{
	const uint64_t kNumOfCapture = 100;
	chrono::system_clock::time_point startTime;
	chrono::system_clock::time_point endTime;
	uint32_t counter = 0;
	uint64_t sumTime = 0;
	VideoCapture cap(0); // open the default camera
	Mat frame;
//	namedWindow("Test",1);
#if RASP
	string imgNamePrefix = "/home/pi/Documents/Projects/Anpr/Result/img";
#else
	string imgNamePrefix = "/home/jose/Documents/Projects/PodPoint/Anpr/Result/img";
#endif
	cout << imgNamePrefix << endl;
	int imgNumber = 0;
	string imgNameSuffix = ".jpg";

	for(;;){
		startTime = chrono::system_clock::now();
		cap >> frame;
		endTime = chrono::system_clock::now();
		auto delta = chrono::duration_cast<chrono::milliseconds>(endTime-startTime);
//		cout << "Frame count: " << cap.get(CV_CAP_PROP_POS_FRAMES) << "\n";
		sumTime += delta.count();
//		cout << "delta: " << delta.count() << "\n";
		counter++;
		if(counter > kNumOfCapture){
			sumTime /= kNumOfCapture;
			cout << "Time in ms over 1000 samples: " << sumTime << endl;
			return;
		}
		string imgName = imgNamePrefix + to_string(imgNumber) + imgNameSuffix;
        imwrite(imgName, frame);
        imgNumber++;
//        imshow("Test", frasme);
//        waitKey(30);
	}
}

TEST(opencv, oneThreadGetAnprSave)
{
	const uint64_t kNumOfCapture = 100;
	chrono::system_clock::time_point startTime;
	chrono::system_clock::time_point endTime;
	uint32_t counter = 0;
	uint64_t sumTime = 0;
	VideoCapture cap(0); // open the default camera
	Mat frame;
//	namedWindow("Test",1);
	alpr::Alpr openalpr("gb", "/etc/openalpr/opena-lpr.conf");

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
		return;
	}

	cap >> frame;

    std::vector<alpr::AlprRegionOfInterest> regionsOfInterest;
    regionsOfInterest.push_back(alpr::AlprRegionOfInterest(0, 0, frame.cols/2, frame.rows));
    regionsOfInterest.push_back(alpr::AlprRegionOfInterest(frame.cols/2, 0, frame.cols/2, frame.rows));
    alpr::AlprResults results;

#if RASP
	string imgNamePrefix = "/home/pi/Documents/Projects/Anpr/Result/img";
#else
	string imgNamePrefix = "/home/jose/Documents/Projects/PodPoint/Anpr/Result/img";
#endif
	int imgNumber = 0;
	string imgNameSuffix = "v.jpg";

	for(;;){
		startTime = chrono::system_clock::now();
		cap >> frame;
		endTime = chrono::system_clock::now();
		auto delta = chrono::duration_cast<chrono::milliseconds>(endTime-startTime);
//		cout << "Frame count: " << cap.get(CV_CAP_PROP_POS_FRAMES) << "\n";
		sumTime += delta.count();
//		cout << "delta: " << delta.count() << "\n";
		counter++;
		if(counter > kNumOfCapture){
			sumTime /= kNumOfCapture;
			cout << "Time in ms over 1000 samples: " << sumTime << endl;
			return;
		}
        results = openalpr.//recognize(imgName);
         		recognize(frame.data, frame.elemSize(), frame.cols, frame.rows, regionsOfInterest);
		string imgName = imgNamePrefix + to_string(imgNumber) + imgNameSuffix;
        imwrite(imgName, frame);
        imgNumber++;
//        imshow("Test", frasme);
//        waitKey(30);
	}
}

TEST(opencv, twoThreadGetAnprSave)
{
	thread getThread(getFrame);
	thread useThread(useFrame);

	getThread.join();
	useThread.join();

}

//TEST(opencv, CameraProperty)
//{
//	VideoCapture cap(0);
//	double ret;
//	for(int i = 0; i <= CV_CAP_PROP_SETTINGS; i++ ){
//		ret = cap.get(i);
//		cout << "i=" << i << "ret=" << ret << "\n";
//	}
//	cout << endl;
//}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
