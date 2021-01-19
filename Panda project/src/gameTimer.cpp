#include "gameTimer.h"

namespace game {
	timer::timer(bool printDuration) {
		this->printDuration = printDuration;
		start = std::chrono::high_resolution_clock::now();
	}

	timer::~timer() {
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		double convertedDuration = duration.count();
		if (this->printDuration) {
			game::timingInfoOut("Timer took " + std::to_string(convertedDuration) + " seconds!");
		}
	}
}