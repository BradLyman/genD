#include <SdlError.hpp>

#include <glad/glad.h>
#include <SDL.h>

#include <iostream>
#include <memory>
#include <thread>
#include <stdexcept>

using namespace std;
using tetra::SdlError;

struct SdlGl
{
    inline ~SdlGl()
    {
        SDL_GL_DeleteContext(gl);
    }

    SDL_GLContext gl;
};

void app()
{
    auto window = unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>{
        SDL_CreateWindow(
            "float-me",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            600, 800,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
        ),
        &SDL_DestroyWindow
    };
    if (!window) SdlError::throwIfFound();

    SdlGl context{
        SDL_GL_CreateContext(window.get())
    };
    SDL_GL_MakeCurrent(window.get(), context.gl);

    if (!gladLoadGLLoader(&SDL_GL_GetProcAddress)) {
        throw std::runtime_error{"could not load opengl procs"};
    }
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    bool quit = false;
    while (!quit)
    {
        SDL_Event e{};
        while (SDL_PollEvent(&e) != 0)
        {
            quit = (e.type == SDL_QUIT);
        }
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window.get());
    }
}

int main(int argc, char* argv[])
{
    struct SdlAutoQuit
    {
        ~SdlAutoQuit() { SDL_Quit(); }
    };
    SdlAutoQuit quitOnExit{};
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "SDL Error: " << SDL_GetError() << endl;
        return 1;
    }
    app();
    return 0;
}

