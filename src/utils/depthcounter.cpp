#include "utils/depthcounter.hpp"

namespace CT
{

std::size_t DepthCounter::GetDepth() const noexcept
{
    return _depth;
}

DepthCounter::Incrementer::Incrementer(DepthCounter& counter) : _counter(counter)
{
    _counter._depth++;
}

DepthCounter::Incrementer::~Incrementer()
{
    _counter._depth--;
}

DepthCounter::Incrementer DepthCounter::Increment()
{
    return { *this };
}

}