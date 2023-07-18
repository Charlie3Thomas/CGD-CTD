#pragma once

#include <cstddef>

namespace CT
{
class DepthCounter
{
public:
    std::size_t GetDepth() const noexcept;

    class Incrementer
    {
    public:
        Incrementer(const Incrementer&) = delete;
        Incrementer(Incrementer&&) = delete;
        Incrementer& operator=(const Incrementer&) = delete;
        Incrementer& operator=(Incrementer&&) = delete;
        ~Incrementer();

    private:
        Incrementer(DepthCounter& counter);
        friend class DepthCounter;
        DepthCounter& _counter;
    };

    Incrementer Increment();

private:
    friend class Incrementer;
    std::size_t _depth = 0;
};

}
