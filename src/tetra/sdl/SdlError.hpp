#ifndef TETRA_SDL_ERROR_HPP
#define TETRA_SDL_ERROR_HPP

#include <SDL.h>
#include <stdexcept>

namespace tetra
{
class SdlError : public std::runtime_error
{
public:
  SdlError(const std::string& msg);

  /**
   * Throw an SdlError if SDL_GetError returns a non empty string.
   */
  static void throwIfFound();
};

} // namespace tetra

#endif
