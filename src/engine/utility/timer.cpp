#include "engine/utility/timer.h"


void Timer::Reset() {
	_time = Clock_t::now();
}
float Timer::Elapsed() {
	auto time_now = Clock_t::now();
	float interval = (float)std::chrono::duration_cast<std::chrono::microseconds>(time_now - _time).count();
	_time = time_now;
	return interval / 1.0E6f;
}