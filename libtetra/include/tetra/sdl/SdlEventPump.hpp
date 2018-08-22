#ifndef TETRA_SDL_EVENTPUMP_HPP
#define TETRA_SDL_EVENTPUMP_HPP

#include <tetra/sdl/Window.hpp>

namespace tetra
{
/**
 * Objects which implement this interface can respond to SDL events.
 */
class IWindowEvents
{
  public:
    /**
     * Render a single frame for the current window.
     * The back-buffer is automatically swapped once this method completes.
     */
    virtual void on_frame_render() = 0;

    /**
     * When the screen size changes this is invoked with the new width and
     * height provided.
     */
    virtual void on_viewport_change(int width, int height);

    /**
     * Called when the mouse moves within the window.
     * Parameters X and Y are integer pixel coordinates relative to the window
     * width and height.
     */
    virtual void on_mouse_move(int x, int y);
};

/**
 * Drive an SDL application with a single window.
 */
class SDLEventPump
{
  public:
    /**
     * Create a new event pump with the provided window and handler.
     * The window and handler references are retained for the lifetime of
     * the SDLEventPump.
     */
    SDLEventPump(Window& window, IWindowEvents& handler);

    /**
     * Loop until an SDL_QUIT message is recieved.
     * Handle/Dispatch input, render frames, and swap the buffer.
     */
    void run_until_quit();

  private:
    void handle_all_pending_events();
    void handle_event(const SDL_Event& event);
    void handle_window_event(const SDL_Event& event);

  private:
    bool running;
    IWindowEvents& event_handler;
    Window& window;
};
} // namespace tetra

#endif
