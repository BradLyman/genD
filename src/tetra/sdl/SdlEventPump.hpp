#ifndef TETRA_SDL_EVENTPUMP_HPP
#define TETRA_SDL_EVENTPUMP_HPP

#include "Window.hpp"

namespace tetra
{
/**
 * Drive an SDL application with a single window.
 * Requires a template parameter Handler with the following methods:
 *
 * void render_frame();
 * void on_viewport_change(int width, int height);
 */
template<class Handler>
class SDLEventPump
{
  public:
    /**
     * Create a new event pump with the provided window and handler.
     * The window and handler references are retained for the lifetime of
     * the SDLEventPump.
     */
    SDLEventPump(Window& window, Handler& handler)
        : window{window}, handler{handler}, running{true}
    {
    }

    /**
     * Loop until an SDL_QUIT message is recieved.
     * Handle/Dispatch input, render frames, and swap the buffer.
     */
    void run_until_quit()
    {
        while (running) {
            handle_all_pending_events();
            handler.render_frame();
            window.swap_gl();
        }
    }

  private:
    void handle_all_pending_events()
    {
        SDL_Event e{};
        while (SDL_PollEvent(&e)) {
            handle_event(e);
        }
    }

    void handle_event(const SDL_Event& event)
    {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_WINDOWEVENT:
            handle_window_event(event);
            break;
        }
    }

    void handle_window_event(const SDL_Event& event)
    {
        switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED:
            handler.on_viewport_change(event.window.data1, event.window.data2);
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            handler.on_viewport_change(event.window.data1, event.window.data2);
            break;
        }
    }

  private:
    bool running;
    Handler& handler;
    Window& window;
};
} // namespace tetra

#endif
