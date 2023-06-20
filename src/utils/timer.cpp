#include <iostream>

#include "timer.hpp"

namespace CT
{
// Timer
Timer::Timer(const std::string& label) : Timer([label](int64_t time) {
    std::cout << label << " took " << time << " ms" << std::endl; }) { }

Timer::Timer(std::function<void(int64_t)> callback) : _callback(std::move(callback)) { }

Timer::~Timer()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - _start).count();
    _callback(duration);
}


// Cumulative Timer
CumTimer::CumTimer(const std::string& label) : CumTimer([label](int64_t time) {
    std::cout << label << " took " << time << " ms" << std::endl; }) { }

CumTimer::CumTimer(std::function<void(int64_t)> callback) : _callback(std::move(callback)) { }

CumTimer::~CumTimer()
{
    _callback(_cumulative);
}

Timer CumTimer::IncreaseCum()
{
    return {[this](int64_t time) { _cumulative += time; }};
}

}