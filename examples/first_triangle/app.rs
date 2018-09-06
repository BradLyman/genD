use gen_d::app_failure::AppFailure;
use gen_d::core_gl::buffer::{Buffer, Target, Usage};
use gen_d::core_gl::program::Program;
use gen_d::core_gl::shader::{Shader, Type};
use gen_d::core_gl::vao::VAO;
use gen_d::core_gl::Object;
use gen_d::GlApp;
use gl;
use gl::types::GLsizei;
use std;

const VERT_SRC: &'static str = r###"
    #version 460

    layout (location = 0) in vec2 pos;
    layout (location = 1) in vec4 color;
    out vec4 varyColor;
    void main() {
        varyColor = color;
        gl_Position = vec4(pos, 0.0, 1.0);
    }
"###;

const FRAG_SRC: &'static str = r###"
    #version 460

    in vec4 varyColor;
    out vec4 fragColor;
    void main() {
        fragColor = varyColor;
    }
"###;

pub struct MyApp {
    vert_buffer: Buffer,
    vao: VAO,
    program: Program,
}

impl MyApp {
    pub fn build() -> MyApp {
        MyApp {
            vert_buffer: Buffer::new(),
            vao: VAO::new(),
            program: Program::new(),
        }
    }
}

#[derive(Copy, Clone, Debug)]
#[repr(C)]
struct Vertex {
    pos: [f32; 2],
    color: [f32; 4],
}

impl GlApp for MyApp {
    fn setup(&mut self) -> Result<(), AppFailure> {
        unsafe {
            gl::ClearColor(0.0, 0.0, 0.0, 1.0);
        }
        self.vert_buffer
            .set_debug_name("MyVertBuffer".to_string())?;
        self.vao.set_debug_name("MyVAO".to_string())?;
        self.program.set_debug_name("MyProgram".to_string())?;

        self.vert_buffer.write(
            Usage::StaticDraw,
            &mut vec![
                Vertex {
                    pos: [0.0, 0.0],
                    color: [1.0, 1.0, 1.0, 1.0],
                },
                Vertex {
                    pos: [0.5, 0.0],
                    color: [0.5, 0.0, 0.0, 1.0],
                },
                Vertex {
                    pos: [0.0, 0.5],
                    color: [0.0, 0.3, 0.8, 1.0],
                },
            ],
        );
        let vert_buffer = &mut self.vert_buffer;
        self.vao.while_bound(|| {
            vert_buffer.while_bound(Target::Array, || unsafe {
                gl::EnableVertexAttribArray(0);
                gl::VertexAttribPointer(
                    0,
                    2,
                    gl::FLOAT,
                    gl::FALSE,
                    std::mem::size_of::<Vertex>() as GLsizei,
                    0 as *const std::os::raw::c_void,
                );
                gl::EnableVertexAttribArray(1);
                gl::VertexAttribPointer(
                    1,
                    4,
                    gl::FLOAT,
                    gl::FALSE,
                    std::mem::size_of::<Vertex>() as GLsizei,
                    std::mem::size_of::<[f32; 2]>()
                        as *const std::os::raw::c_void,
                );
            });
        });

        self.program
            .attach(Shader::with_source(Type::Vertex, VERT_SRC.to_string()))
            .attach(Shader::with_source(Type::Fragment, FRAG_SRC.to_string()))
            .link()?;

        Ok(())
    }

    fn render_frame(&mut self) -> Result<(), AppFailure> {
        unsafe {
            gl::Clear(gl::COLOR_BUFFER_BIT);
            gl::UseProgram(self.program.raw());
        };
        self.vao.while_bound(|| unsafe {
            gl::DrawArrays(gl::TRIANGLES, 0, 3);
        });
        return Ok(());
    }
}
