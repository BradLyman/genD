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
}

pub fn drive_gl_app<T: GlApp>(
    mut event_pump: sdl2::EventPump,
    window: &sdl2::video::Window,
    app: &mut T,
) -> Result<(), AppFailure> {
    app.setup();
    let mut running = true;
    while (running) {
        running = handle_events(&mut event_pump);
        app.render_frame()?;
        window.gl_swap_window();
    }
    return Ok(());
}

fn handle_events(event_pump: &mut sdl2::EventPump) -> bool {
    for event in event_pump.poll_iter() {
        use sdl2::event::Event;
        match event {
            Event::Quit { .. } => return false,
            _ => (),
        }
    }
    return true;
}
