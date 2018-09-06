use gen_d::app_failure::AppFailure;
use gen_d::core_gl::buffer::{Buffer, Target, Usage};
use gen_d::core_gl::shader::{Shader, Type};
use gen_d::core_gl::vao::VAO;
use gen_d::core_gl::Object;
use gen_d::GlApp;
use gl;

const VERT_SRC: &'static str = r###"
    #version 460

    layout (location = 0) in vec2 pos;
    void main() {
        gl_Position = vec4(pos, 0.0, 1.0);
    }
"###;

const FRAG_SRC: &'static str = r###"
    #version 460

    out vec4 fragColor;
    void main() {
        fragColor = vec4(1.0);
    }
"###;

pub struct MyApp {
    vert_buffer: Buffer,
    vao: VAO,
}

impl MyApp {
    pub fn build() -> MyApp {
        MyApp {
            vert_buffer: Buffer::new(),
            vao: VAO::new(),
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
        self.vao.set_debug_name("MyVAO".to_string())?;

        self.vert_buffer.write(
            Usage::StaticDraw,
            &mut vec![
                Vertex { pos: [0.0, 0.0] },
                Vertex { pos: [0.5, 0.0] },
                Vertex { pos: [0.0, 0.5] },
            ],
        );
        let vert_buffer = &mut self.vert_buffer;
        self.vao.while_bound(|| {
            vert_buffer.while_bound(Target::Array, || {
                //
            });
        });

        let mut vert = Shader::with_source(Type::Vertex, VERT_SRC.to_string());
        vert.compile()?;

        let mut frag =
            Shader::with_source(Type::Fragment, FRAG_SRC.to_string());
        frag.compile()?;
        Ok(())
    }

    fn render_frame(&mut self) -> Result<(), AppFailure> {
        unsafe {
            gl::Clear(gl::COLOR_BUFFER_BIT);
        };
        return Ok(());
    }
}
