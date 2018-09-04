extern crate gen_d;
extern crate gl;
extern crate sdl2;

use gen_d::app_failure::AppFailure;
use gen_d::core_gl;
use gen_d::core_gl::Object;
use gen_d::{drive_gl_app, GlApp};

#[derive(Copy, Clone)]
#[repr(C)]
struct Vertex {
    pos: [f32; 2],
}

struct MyApp {
    vert_buffer: core_gl::Buffer,
}

impl MyApp {
    fn build() -> MyApp {
        MyApp {
            vert_buffer: core_gl::Buffer::new(),
        }
    }
}

impl GlApp for MyApp {
    fn setup(&mut self) -> Result<(), AppFailure> {
        unsafe {
            gl::ClearColor(0.0, 0.0, 0.0, 1.0);
        }

        self.vert_buffer
            .set_debug_name("MyVertBuffer".to_string())?;

        self.vert_buffer.write(&mut vec![
            Vertex { pos: [0.0, 0.0] },
            Vertex { pos: [0.5, 0.0] },
            Vertex { pos: [0.0, 0.5] },
        ]);
        Ok(())
    }

    fn render_frame(&mut self) -> Result<(), AppFailure> {
        // no-op
        unsafe {
            gl::Clear(gl::COLOR_BUFFER_BIT);
        }
        return Ok(());
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

    unsafe {
        gl::Enable(gl::DEBUG_OUTPUT);
        gl::DebugMessageCallback(gen_d::gl_debug_to_stdout, 0 as _);
    }

    drive_gl_app(sdl.event_pump()?, &window, &mut MyApp::build())?;

    return Ok(());
}

fn set_ogl_attribs(gl_attr: &mut sdl2::video::gl_attr::GLAttr) {
    gl_attr.set_context_profile(sdl2::video::GLProfile::Core);
    gl_attr.set_context_version(4, 6);
    gl_attr.set_multisample_buffers(1);
    gl_attr.set_multisample_samples(4);
}
