#pragma once
#include <chrono>
#include <functional>
#include <string>

namespace CT
{
class Timer
{
public:
    Timer(const std::string& label);
    Timer(std::function<void(int64_t)>);
    ~Timer();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start = std::chrono::high_resolution_clock::now();
    const std::function<void(int64_t)> _callback;
};
}