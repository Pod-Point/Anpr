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

#include <sys/types.h>
#include <dirent.h>

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

//TEST(opencv, oneThreadGet)
//{
//	const uint64_t kNumOfCapture = 100;
//	chrono::system_clock::time_point startTime;
//	chrono::system_clock::time_point endTime;
//	uint32_t counter = 0;
//	uint64_t sumTime = 0;
//	VideoCapture cap(0); // open the default camera
//	Mat frame;
//
//	for(;;){
//		startTime = chrono::system_clock::now();
//		cap >> frame;
//		endTime = chrono::system_clock::now();
//		auto delta = chrono::duration_cast<chrono::milliseconds>(endTime-startTime);
//		sumTime += delta.count();
//		counter++;
//		if(counter > kNumOfCapture){
//			sumTime /= kNumOfCapture;
//			cout << "Time in ms over 1000 samples: " << sumTime << endl;
//			return;
//		}
//	}
//}
//
//TEST(opencv, oneThreadGetSave)
//{
//	const uint64_t kNumOfCapture = 100;
//	chrono::system_clock::time_point startTime;
//	chrono::system_clock::time_point endTime;
//	uint32_t counter = 0;
//	uint64_t sumTime = 0;
//	VideoCapture cap(0); // open the default camera
//	Mat frame;
//#if RASP
//	string imgNamePrefix = "/home/pi/Documents/Projects/Anpr/Result/img";
//#else
//	string imgNamePrefix = "/home/jose/Documents/Projects/PodPoint/Anpr/Result/img";
//#endif
//	cout << imgNamePrefix << endl;
//	int imgNumber = 0;
//	string imgNameSuffix = ".jpg";
//
//	for(;;){
//		startTime = chrono::system_clock::now();
//		cap >> frame;
//		endTime = chrono::system_clock::now();
//		auto delta = chrono::duration_cast<chrono::milliseconds>(endTime-startTime);
//		sumTime += delta.count();
//		counter++;
//		if(counter > kNumOfCapture){
//			sumTime /= kNumOfCapture;
//			cout << "Time in ms over 1000 samples: " << sumTime << endl;
//			return;
//		}
//		string imgName = imgNamePrefix + to_string(imgNumber) + imgNameSuffix;
//        imwrite(imgName, frame);
//        imgNumber++;
//	}
//}
//
//TEST(opencv, oneThreadGetAnprSave)
//{
//	const uint64_t kNumOfCapture = 100;
//	chrono::system_clock::time_point startTime;
//	chrono::system_clock::time_point endTime;
//	uint32_t counter = 0;
//	uint64_t sumTime = 0;
//	VideoCapture cap(0); // open the default camera
//	Mat frame;
//	alpr::Alpr openalpr("gb", "/etc/openalpr/openalpr.conf");
//
//	// Optionally specify the top N possible plates to return (with confidences).  Default is 10
//	openalpr.setTopN(5);
//
//	// Optionally, provide the library with a region for pattern matching.  This improves accuracy by
//	// comparing the plate text with the regional pattern.
//	openalpr.setDefaultRegion("md");
//
//	// Make sure the library loaded before continuing.
//	// For example, it could fail if the config/runtime_data is not found
//	if (openalpr.isLoaded() == false)
//	{
//		std::cerr << "Error loading OpenALPR" << std::endl;
//		return;
//	}
//
//	cap >> frame;
//
//    std::vector<alpr::AlprRegionOfInterest> regionsOfInterest;
//    regionsOfInterest.push_back(alpr::AlprRegionOfInterest(0, 0, frame.cols/2, frame.rows));
//    regionsOfInterest.push_back(alpr::AlprRegionOfInterest(frame.cols/2, 0, frame.cols/2, frame.rows));
//    alpr::AlprResults results;
//
//#if RASP
//	string imgNamePrefix = "/home/pi/Documents/Projects/Anpr/Result/img";
//#else
//	string imgNamePrefix = "/home/jose/Documents/Projects/PodPoint/Anpr/Result/img";
//#endif
//	int imgNumber = 0;
//	string imgNameSuffix = "v.jpg";
//
//	for(;;){
//		startTime = chrono::system_clock::now();
//		cap >> frame;
//		endTime = chrono::system_clock::now();
//		auto delta = chrono::duration_cast<chrono::milliseconds>(endTime-startTime);
//		sumTime += delta.count();
//		counter++;
//		if(counter > kNumOfCapture){
//			sumTime /= kNumOfCapture;
//			cout << "Time in ms over 1000 samples: " << sumTime << endl;
//			return;
//		}
//        results = openalpr.//recognize(imgName);
//         		recognize(frame.data, frame.elemSize(), frame.cols, frame.rows, regionsOfInterest);
//		string imgName = imgNamePrefix + to_string(imgNumber) + imgNameSuffix;
//        imwrite(imgName, frame);
//        imgNumber++;
//	}
//}

//TEST(opencv, twoThreadGetAnprSave)
//{
//	thread getThread(getFrame);
//	thread useThread(useFrame);
//
//	getThread.join();
//	useThread.join();
//
//}

TEST(opencv, DirTest)
{
#if defined(RASP)
	DIR *dir = opendir("/home/pi/Documents/Projects/Anpr/TestImage");
#else
	DIR *dir = opendir("/home/jose/Documents/Projects/PodPoint/Anpr/ResultPI/PiF20181101_1530");
#endif
	if(dir == NULL){
//		cout << "KO " << endl;
		GTEST_FAIL();
	}
	dirent * entry;
	uint32_t count = 0;
	while(entry = readdir(dir)){
		if(entry->d_type != DT_DIR){
//			cout << entry->d_name << "\n";
			count++;
		}
	}
//	EXPECT_EQ(string("A"), Utils::getASCIIFromHexString(string("41")));
//	cout << endl;
}

TEST(opencv, TestImage)
{
	Mat frame;
//	namedWindow("Test",1);
	alpr::Alpr openalpr("gb", "/etc/openalpr/openalpr.conf");

	// Optionally specify the top N possible plates to return (with confidences).  Default is 10
	openalpr.setTopN(5);

	// Optionally, provide the library with a region for pattern matching.  This improves accuracy by
	// comparing the plate text with the regional pattern.
	openalpr.setDefaultRegion("gb");

	// Make sure the library loaded before continuing.
	// For example, it could fail if the config/runtime_data is not found
	if (openalpr.isLoaded() == false)
	{
		std::cerr << "Error loading OpenALPR" << std::endl;
		return;
	}

#if defined(RASP)
	string pathdir = "/home/pi/Documents/Projects/Anpr/TestImage";
#else
	string pathdir = "/home/jose/Documents/Projects/PodPoint/Anpr/ResultPI/ResultPi20181012_1240";
#endif
	DIR *dir = opendir(pathdir.c_str());
	vector <string> elemlist;
	if(dir == NULL){
		GTEST_FAIL();
	}
	dirent * entry;
	while(entry = readdir(dir)){
		if(entry->d_type != DT_DIR){
			string elem(entry->d_name);
			elemlist.push_back(pathdir+"/"+elem);
		}
	}
	sort(elemlist.begin(), elemlist.end());

	chrono::system_clock::time_point startTime;
	chrono::system_clock::time_point endTime;
	uint32_t roitime = 0;
	uint32_t wholetime = 0;
	uint32_t totaElemSize = elemlist.size();

	while(!elemlist.empty()){
		string imgName = elemlist.back();
		elemlist.pop_back();

		frame = imread(imgName);
		cout << imgName << "\n";
		std::vector<alpr::AlprRegionOfInterest> regionsOfInterest;
		regionsOfInterest.push_back(alpr::AlprRegionOfInterest(0, frame.rows/4, frame.cols/4*3, frame.rows/4*3));
		regionsOfInterest.push_back(alpr::AlprRegionOfInterest(frame.cols/4, frame.rows/4, frame.cols/4*3, frame.rows/4*3));
		alpr::AlprResults results;

		startTime = chrono::system_clock::now();
		results = openalpr.//recognize(imgName);
				recognize(frame.data, frame.elemSize(), frame.cols, frame.rows, regionsOfInterest);
		endTime = chrono::system_clock::now();

		auto delta = chrono::duration_cast<chrono::milliseconds>(endTime-startTime);
		roitime += delta.count();

		cout << "ROI" << "\n";
		for (uint32_t i = 0; i < results.plates.size(); i++)
		{
			alpr::AlprPlateResult plate = results.plates[i];
			for (uint32_t k = 0; k < plate.topNPlates.size(); k++)
			{
				alpr::AlprPlate candidate = plate.topNPlates[k];
				std::cout << "    - " << candidate.characters << "\t confidence: " << candidate.overall_confidence;
				std::cout << "\t pattern_match: " << candidate.matches_template << "\n";
			}
		}
		regionsOfInterest.clear();
		regionsOfInterest.push_back(alpr::AlprRegionOfInterest(0, 0, frame.cols, frame.rows));

		startTime = chrono::system_clock::now();
		results = openalpr.//recognize(imgName);
	     		recognize(frame.data, frame.elemSize(), frame.cols, frame.rows, regionsOfInterest);
		endTime = chrono::system_clock::now();

		delta = chrono::duration_cast<chrono::milliseconds>(endTime-startTime);
		wholetime += delta.count();

		cout << "Whole" << "\n";
		for (uint32_t i = 0; i < results.plates.size(); i++)
		{
			alpr::AlprPlateResult plate = results.plates[i];
			for (uint32_t k = 0; k < plate.topNPlates.size(); k++)
			{
				alpr::AlprPlate candidate = plate.topNPlates[k];
				std::cout << "    - " << candidate.characters << "\t confidence: " << candidate.overall_confidence;
				std::cout << "\t pattern_match: " << candidate.matches_template << "\n";
			}
		}
		regionsOfInterest.clear();
	}
	cout << "roitime: " << roitime / totaElemSize << "\n";
	cout << "wholetime: " << wholetime / totaElemSize << "\n";
	cout << endl;
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
