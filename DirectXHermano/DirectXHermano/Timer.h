#pragma once
#include <chrono>

using namespace std::chrono;

class Timer
{
public:
	Timer() { current_time = steady_clock::now(); }
	float Mark() 
	{
		//Return time elapsed since last Mark() call
		steady_clock::time_point old_time = current_time;
		current_time = steady_clock::now();

		const duration<float> frame_time = current_time - old_time;
		return frame_time.count();
	}
	float Peek() const
	{
		//Return time elapsed since last Mark() call without reseting/
		return duration<float>(steady_clock::now() - current_time).count();
	}

private:
	steady_clock::time_point current_time;
};