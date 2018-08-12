#include "Window.hpp"

#include "SdlError.hpp"

using tetra::SdlError;
using tetra::Window;

Window::Window(SDL_Window* window)
    : window(window, &SDL_DestroyWindow), gl{SDL_GL_CreateContext(window)}
{
  if (!window)
    SdlError::throwIfFound();
}

void Window::make_context_current()
{
  SDL_GL_MakeCurrent(this->window.get(), this->gl);
}

void Window::swap_gl() { SDL_GL_SwapWindow(window.get()); }
