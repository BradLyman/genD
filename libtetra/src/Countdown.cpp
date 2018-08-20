#include <chrono>
#include <tetra/Countdown.hpp>

using namespace tetra;

/**
 * Reset the countdown to 'ms' milliseconds from now.
 */
void Countdown::reset(std::chrono::milliseconds ms)
{
    finish = std::chrono::high_resolution_clock::now() + ms;
}

/**
 * Is the time up for this countdown?
 * true - time is up
 * false - some time remains untile the countdown expires
 */
bool Countdown::time_up() const
{
    return std::chrono::high_resolution_clock::now() > finish;
}
