#pragma once

#include <list>
#include <chrono>

class FpsCounter {
public:
	const int nbrFrames = 20;

	int tick() {
		auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		auto deltaTime = now - lastTime;
		lastIntervals.push_back(deltaTime);
		if (lastIntervals.size() > nbrFrames)
			lastIntervals.pop_front();

		int avarageTime = 0;
		for (auto i : lastIntervals)
			avarageTime += i;
		avarageTime /= lastIntervals.size();

		lastTime = now;
		return avarageTime;
	}


private:
	std::list<int> lastIntervals;
	__int64 lastTime{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() };
};