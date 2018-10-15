/*
 * PlateHistogram.cpp
 *
 *  Created on: 8 Oct 2018
 *      Author: jose
 */

#include "PlateHistogram.h"
#include <vector>
#include <stdint.h>

using namespace std;

PlateHistogram::Histogram::Histogram() {
	_startTime = chrono::system_clock::now();
	_endTime = chrono::system_clock::now();
	_isCarParking = false;
}

PlateHistogram::Histogram::~Histogram() {
	// TODO Auto-generated destructor stub
}

PlateHistogram::PlateHistogramManager::PlateHistogramManager() {
}

PlateHistogram::PlateHistogramManager::~PlateHistogramManager() {
	// TODO Auto-generated destructor stub
}

PlateHistogram::Position operator++(PlateHistogram::Position & pos) {
	if(pos == PlateHistogram::Position::kMaxPosition){
		return pos;
	}
	pos = static_cast<PlateHistogram::Position>(static_cast<int>(pos) + 1);
	return pos;
}

void PlateHistogram::PlateHistogramManager::removeInconsistency(){

	for(auto hist : _histogramList){
		//reduce probability
		vector<string> v2rm;
		for (std::pair<std::string, int> plateElem : hist._histogram){
			plateElem.second--;
			if(plateElem.second < 0){
				v2rm.push_back(plateElem.first);
			}
		}

		// remove unused
		for(auto plate2rm: v2rm){
			hist._histogram.erase(plate2rm);
		}
	}

}

bool PlateHistogram::PlateHistogramManager::isPlateAvailable(const PlateHistogram::Position pos){
	uint8_t postion = static_cast<uint8_t>(pos);

	if(_histogramList[postion]._histogram.empty()){
		return false;
	}

	_histogramList[postion]._endTime = chrono::system_clock::now();
	auto delta = chrono::duration_cast<chrono::seconds>(_histogramList[postion]._endTime-_histogramList[postion]._startTime);

	if(_histogramList[postion]._isCarParking && delta.count() > 10){
		return true;
	}
	return false;
}

std::string PlateHistogram::PlateHistogramManager::getBestCalculatedPlate(const PlateHistogram::Position pos){
	int maxOccurence = 0;
	string ret;
	uint8_t postion = static_cast<uint8_t>(pos);

	for(auto  plate : _histogramList[postion]._histogram){
		if(plate.second > maxOccurence){
			maxOccurence = plate.second;
			ret = plate.first;
		}
	}
	_histogramList[postion]._isCarParking = false;
	_histogramList[postion]._histogram.clear();
	return ret;
}

void PlateHistogram::PlateHistogramManager::addPossiblePlate(const string & plate, bool match, const PlateHistogram::Position pos){
	uint8_t postion = static_cast<uint8_t>(pos);
	_histogramList[postion]._histogram[plate] += (match*20) + 2;
	_histogramList[postion]._isCarParking = true;
	_histogramList[postion]._startTime = chrono::system_clock::now();
}

