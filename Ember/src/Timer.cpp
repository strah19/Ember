#include "Timer.h"

constexpr int SECOND_MIL = 1000;

namespace Ember {
	Timer::Timer() {
		StartTimer();
	}

	Timer::~Timer() {
		StopTimer();
	}

	void Timer::StartTimer() {
		on = true;
	}

	void Timer::Update() {
		if (on) {
			current_time = SDL_GetTicks();
			if (current_time > last_time + SECOND_MIL) {
				last_time = current_time;
				seconds++;
			}
		}
	}

	void Timer::StopTimer() {
		on = false;
	}

	void Timer::Reset() {
		seconds = 0;
	}

	bool Timer::FetchAt(int timer_spot) {
		return (seconds == timer_spot);
	}
}