/*
 * PlateHistogram.h
 *
 *  Created on: 8 Oct 2018
 *      Author: jose
 */

#ifndef PLATEHISTOGRAM_H_
#define PLATEHISTOGRAM_H_

#include <unordered_map>
#include <string>
#include <chrono>
namespace PlateHistogram
{
	enum Position {kLeft, kRight,kMaxPosition};

	class Histogram {
	public:
		Histogram();
		virtual ~Histogram();
//	private:
		std::unordered_map<std::string, int> _histogram;
		std::chrono::system_clock::time_point _startTime;
		std::chrono::system_clock::time_point _endTime;
		bool _isCarParking;
	};

	class PlateHistogramManager {
	public:
		PlateHistogramManager();
		virtual ~PlateHistogramManager();
		void removeInconsistency();
		bool isPlateAvailable(const Position pos);
		std::string getBestCalculatedPlate(const Position pos);
		void addPossiblePlate(const std::string & plate, bool match, const Position pos);

	private:
		Histogram _histogramList[2];
	};
}
#endif /* PLATEHISTOGRAM_H_ */
