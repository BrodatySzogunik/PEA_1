
#pragma once

#include <chrono>
enum TimerPrecision{
    Nanoseconds,
    Microseconds,
    Milliseconds,
    Seconds
};
    class Timer
    {
    public:
        void start();
        void stop();
        long long getTime(TimerPrecision precision);
    private:
        std::chrono::high_resolution_clock::time_point startPoint;
        std::chrono::high_resolution_clock::time_point endPoint;
    };

