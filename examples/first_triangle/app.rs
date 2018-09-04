use gen_d::app_failure::AppFailure;
use gen_d::core_gl::buffer::{Buffer, Usage};
use gen_d::core_gl::Object;
use gen_d::GlApp;
use gl;

pub struct MyApp {
    vert_buffer: Buffer,
}

impl MyApp {
    pub fn build() -> MyApp {
        MyApp {
            vert_buffer: Buffer::new(),
        }
    }
}

#[derive(Copy, Clone, Debug)]
#[repr(C)]
struct Vertex {
    pos: [f32; 2],
}

impl GlApp for MyApp {
    fn setup(&mut self) -> Result<(), AppFailure> {
        unsafe {
            gl::ClearColor(0.0, 0.0, 0.0, 1.0);
        }

        self.vert_buffer
            .set_debug_name("MyVertBuffer".to_string())?;

        self.vert_buffer.write(
            Usage::StaticDraw,
            &mut vec![
                Vertex { pos: [0.0, 0.0] },
                Vertex { pos: [0.5, 0.0] },
                Vertex { pos: [0.0, 0.5] },
            ],
        );
        Ok(())
    }

    fn render_frame(&mut self) -> Result<(), AppFailure> {
        unsafe {
            gl::Clear(gl::COLOR_BUFFER_BIT);
        }
        return Ok(());
    }
}
