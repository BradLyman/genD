#include <glad/glad.h>

#include "tetra/sdl/SdlError.hpp"
#include "tetra/sdl/Window.hpp"

#include <SDL.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>

using namespace std;
using tetra::SdlError;
using tetra::Window;

/** use SDL to load opengl function pointers for the current context */
void load_gl_functions()
{
  if (!gladLoadGLLoader(&SDL_GL_GetProcAddress)) {
    throw std::runtime_error{"could not load opengl procs"};
  }
}

void app()
{
  auto window = Window{SDL_CreateWindow(
    "float-me",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    600,
    800,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL)};
  window.make_context_current();

  load_gl_functions();

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  bool quit = false;
  while (!quit) {
    SDL_Event e{};
    while (SDL_PollEvent(&e) != 0) {
      quit = (e.type == SDL_QUIT);
    }
    glClear(GL_COLOR_BUFFER_BIT);
    window.swap_gl();
  }
}

int main(int argc, char* argv[])
{
  struct SdlAutoQuit {
    ~SdlAutoQuit() { SDL_Quit(); }
  } quitOnExit{};
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    cout << "SDL Error: " << SDL_GetError() << endl;
    return 1;
  }
  app();
  return 0;
}
