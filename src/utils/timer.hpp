#pragma once
#include <chrono>
#include <atomic>
#include <functional>
#include <string>

namespace CT
{
class Timer
{
public:
    Timer(const std::string& label);
    Timer(std::function<void(int64_t)>);
    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator = (const Timer&) = delete;
    Timer& operator = (Timer&&) = delete;
    ~Timer();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start = std::chrono::high_resolution_clock::now();
    const std::function<void(int64_t)> _callback;
};

class CumTimer
{
public:
    CumTimer(const std::string& label);
    CumTimer(std::function<void(int64_t)>);
    CumTimer(const CumTimer&) = delete;
    CumTimer(CumTimer&&) = delete;
    CumTimer& operator = (const CumTimer&) = delete;
    CumTimer& operator = (CumTimer&&) = delete;
    ~CumTimer();

    Timer IncreaseCum();

private:
    std::atomic<int64_t> _cumulative = 0;
    const std::function<void(int64_t)> _callback;
};
}