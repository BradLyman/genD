extern crate gen_d;
extern crate gl;
extern crate sdl2;

use gen_d::app_failure::AppFailure;
use gen_d::{drive_gl_app, GlApp};

struct MyApp {}

impl GlApp for MyApp {
    fn setup(&mut self) -> Result<(), AppFailure> {
        unsafe {
            gl::Enable(gl::DEBUG_OUTPUT);
            gl::ClearColor(1.0, 1.0, 1.0, 1.0);
            gl::DebugMessageCallback(gen_d::gl_debug_to_stdout, 0 as _);
        }
        Ok(())
    }

    fn render_frame(&mut self) -> Result<(), AppFailure> {
        // no-op
        unsafe {
            gl::Clear(gl::COLOR_BUFFER_BIT);
        }
        return Ok(());
    }

    fn on_viewport_resize(
        &mut self,
        width: i32,
        height: i32,
    ) -> Result<(), AppFailure> {
        println!("resized {:?}, {:?}", width, height);
        Ok(())
    }
}

fn main() -> Result<(), AppFailure> {
    let sdl = sdl2::init()?;
    let video = sdl.video()?;
    set_ogl_attribs(&mut video.gl_attr());

    let window =
        sdl2::video::WindowBuilder::new(&video, "float-me", 1920, 1080)
            .resizable()
            .opengl()
            .build()?;
    let context = window.gl_create_context()?;
    window.gl_make_current(&context)?;
    gl::load_with(|s| video.gl_get_proc_address(s) as _);

    drive_gl_app(sdl.event_pump()?, &window, &mut MyApp {})?;

    return Ok(());
}

fn set_ogl_attribs(gl_attr: &mut sdl2::video::gl_attr::GLAttr) {
    gl_attr.set_context_profile(sdl2::video::GLProfile::Core);
    gl_attr.set_context_version(4, 6);
    gl_attr.set_multisample_buffers(1);
    gl_attr.set_multisample_samples(4);
}
