#ifndef TETRA_COUNTDOWN_HPP
#define TETRA_CONUTDOWN_HPP

#include <chrono>

namespace tetra
{
/**
 * Objects of this class represent a countdown to a fixed point in time.
 */
class Countdown
{
  public:
    using time_point = std::chrono::high_resolution_clock::time_point;

  public:
    /**
     * Reset the countdown to 'ms' milliseconds from now.
     */
    inline void reset(std::chrono::milliseconds ms)
    {
        finish = std::chrono::high_resolution_clock::now() + ms;
    }

    /**
     * Is the time up for this countdown?
     * true - time is up
     * false - some time remains untile the countdown expires
     */
    inline bool time_up() const
    {
        return std::chrono::high_resolution_clock::now() > finish;
    }

  private:
    time_point finish = std::chrono::high_resolution_clock::now();
};
} // namespace tetra

#endif
