use app_failure::AppFailure;
use sdl2;

///
/// Types which implement this trait represent an application which renders
/// with OpenGL.
///
pub trait GlApp {
    /// This method is used for any OpenGL-dependent setup.
    fn setup(&mut self) -> Result<(), AppFailure>;

    /// Render a single frame.
    fn render_frame(&mut self) -> Result<(), AppFailure>;

    /// Called when the screen is resized
    fn on_viewport_resize(
        &mut self,
        _width: i32,
        _height: i32,
    ) -> Result<(), AppFailure> {
        Ok(())
    }

    /// Called when the mouse moves.
    /// X and Y coords are relative to the window viewport.
    /// x = 0 when the cursor is at the left of the viewport and
    /// x = viewport-width when the cursor is at the right of the viewport.
    /// y = 0 when the cursor at the top of the viewport and
    /// y = viewport-height when the cursor is at the right of the viewport.
    /// bottom of the screen.
    fn on_mouse_move(&mut self, _x: i32, _y: i32) -> Result<(), AppFailure> {
        Ok(())
    }
}

///
/// Process SDL events calling methods on the GL App until a Quit message is
/// recieved.
///
pub fn drive_gl_app<T: GlApp>(
    mut event_pump: sdl2::EventPump,
    window: &sdl2::video::Window,
    app: &mut T,
) -> Result<(), AppFailure> {
    app.setup()?;

    {
        let (w, h) = window.drawable_size();
        app.on_viewport_resize(w as i32, h as i32)?;
    }

    let mut running = true;
    while running {
        running = handle_events(app, &mut event_pump)?;
        app.render_frame()?;
        window.gl_swap_window();
    }
    return Ok(());
}

///
/// Handle all pending events in the SDL event queue.
/// Returns a bool to indicate if the application should terminate.
///
fn handle_events<T: GlApp>(
    app: &mut T,
    event_pump: &mut sdl2::EventPump,
) -> Result<bool, AppFailure> {
    let mut should_continue = true;
    for event in event_pump.poll_iter() {
        use sdl2::event::Event;
        match event {
            Event::Quit { .. } => should_continue = false,
            Event::MouseMotion { x, y, .. } => app.on_mouse_move(x, y)?,
            Event::Window { win_event: win, .. } => {
                handle_window_event(app, &win)?
            }
            _ => (),
        }
    }
    return Ok(should_continue);
}

///
/// Handle a window-event by calling the relevant methods for the GlApp.
///
fn handle_window_event<T: GlApp>(
    app: &mut T,
    event: &sdl2::event::WindowEvent,
) -> Result<(), AppFailure> {
    use sdl2::event::WindowEvent;
    match event {
        WindowEvent::Resized(w, h) => app.on_viewport_resize(*w, *h)?,
        WindowEvent::SizeChanged(w, h) => app.on_viewport_resize(*w, *h)?,
        _ => (),
    }
    Ok(())
}
