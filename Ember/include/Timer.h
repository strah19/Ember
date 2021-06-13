#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

namespace Ember {
	class Timer {
	public:
		Timer();
		~Timer();

		void StartTimer();
		void StopTimer();
		void Update();
		void Reset();

		bool FetchAt(int timer_spot);
		inline int GetSeconds() const { return seconds; }
	private:
		unsigned int last_time = 0, current_time = 0;
		bool on = true;
		int seconds = 0;
	};
}

#endif // !TIMER_H
