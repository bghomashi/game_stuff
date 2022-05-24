
#include <chrono>

class Timer {
	typedef std::chrono::high_resolution_clock Clock_t;
	Clock_t::time_point _time;
public:
	void Reset();
	float Elapsed();
};
