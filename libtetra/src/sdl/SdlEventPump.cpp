#include <tetra/sdl/SdlEventPump.hpp>

using namespace tetra;

void IWindowEvents::on_mouse_move(int x, int y) {}
void IWindowEvents::on_viewport_change(int width, int height) {}

SDLEventPump::SDLEventPump(Window& window, IWindowEvents& handler)
    : window{window}, event_handler{handler}, running{true}
{
}

void SDLEventPump::run_until_quit()
{
    while (running) {
        handle_all_pending_events();
        event_handler.on_frame_render();
        window.swap_gl();
    }
}

void SDLEventPump::handle_all_pending_events()
{
    SDL_Event e{};
    while (SDL_PollEvent(&e)) {
        handle_event(e);
    }
}

void SDLEventPump::handle_event(const SDL_Event& event)
{
    switch (event.type) {
    case SDL_QUIT:
        running = false;
        break;
    case SDL_WINDOWEVENT:
        handle_window_event(event);
        break;
    case SDL_MOUSEMOTION:
        event_handler.on_mouse_move(event.motion.x, event.motion.y);
        break;
    }
}

void SDLEventPump::handle_window_event(const SDL_Event& event)
{
    switch (event.window.event) {
    case SDL_WINDOWEVENT_RESIZED:
        event_handler.on_viewport_change(
            event.window.data1, event.window.data2);
        break;
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        event_handler.on_viewport_change(
            event.window.data1, event.window.data2);
        break;
    }
}
