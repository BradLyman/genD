#include <glad/glad.h>

#include "tetra/GlApp.hpp"
#include "tetra/sdl/SdlError.hpp"
#include "tetra/sdl/Window.hpp"

#include <SDL.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>

using namespace std;
using tetra::GlApp;
using tetra::SdlError;
using tetra::Window;

void load_gl_functions();
void sdl_app_main();

/**
 * Program entrypoint.
 * CLI arguments are not used.
 */
int main(int argc, char* argv[])
{
  struct SdlAutoQuit {
    ~SdlAutoQuit() { SDL_Quit(); }
  } quitOnExit{};
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    cout << "SDL Error: " << SDL_GetError() << endl;
    return 1;
  }
  sdl_app_main();
  return 0;
}

/**
 * The entrypoint for my sdl application. the SDL library is initialized for the
 * entire lifetime of this function.
 */
void sdl_app_main()
{
  auto window = Window{SDL_CreateWindow(
    "float-me",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    1920,
    1080,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL)};
  window.make_context_current();
  load_gl_functions();

  GlApp myGlApp{};

  bool quit = false;
  while (!quit) {
    SDL_Event e{};
    while (SDL_PollEvent(&e) != 0) {
      quit = (e.type == SDL_QUIT);
    }
    myGlApp.render_frame();
    window.swap_gl();
  }
}

/** use SDL to load opengl function pointers for the current context */
void load_gl_functions()
{
  if (!gladLoadGLLoader(&SDL_GL_GetProcAddress)) {
    throw std::runtime_error{"could not load opengl procs"};
  }
}
