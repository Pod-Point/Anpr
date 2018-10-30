/*
 * main_test.cpp
 *
 *  Created on: 15 Oct 2018
 *      Author: jose
 */
#include "gtest/gtest.h"
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace cv;
using namespace std;

TEST(opencv, TimeOfCapturing)
{
	const uint64_t kNumOfCapture = 10;
	chrono::system_clock::time_point startTime;
	chrono::system_clock::time_point endTime;
	uint32_t counter = 0;
	uint64_t sumTime = 0;
	VideoCapture cap(1); // open the default camera
	Mat frame;
//	namedWindow("Test",1);

	for(;;){
		startTime = chrono::system_clock::now();
		cap >> frame;
		endTime = chrono::system_clock::now();
		auto delta = chrono::duration_cast<chrono::microseconds>(endTime-startTime);
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

TEST(opencv, CameraProperty)
{
	VideoCapture cap(1);
	double ret;
	for(int i = 0; i <= CV_CAP_PROP_AUTOFOCUS; i++ ){
		ret = cap.get(i);
		cout << "i=" << i << "ret=" << ret << "\n";
	}
	cout << endl;
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
