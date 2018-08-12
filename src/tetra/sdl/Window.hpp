#ifndef TETRA_SDL_WINDOW_HPP
#define TETRA_SDL_WINDOW_HPP

#include <SDL.h>
#include <memory>

namespace tetra
{
class Window
{
public:
  Window(SDL_Window* window);

  /** make the opengl context current */
  void make_context_current();

  /** swap the opengl buffer for the current window */
  void swap_gl();

private:
  SDL_GLContext gl;
  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
};
} // namespace tetra

#endif
