use app_failure::AppFailure;
use core_gl::buffer::{Buffer, Target, Usage};
use core_gl::program::Program;
use core_gl::shader::{Shader, Type};
use core_gl::vao::VAO;
use gl;
use nalgebra::Matrix4;

const VERT_SRC: &'static str = r###"
    #version 460

    layout (location = 0) in vec2 pos;
    layout (location = 1) in vec4 color;

    layout (location = 0) uniform mat4 view;

    out vec4 varyColor;
    void main() {
        varyColor = color;
        gl_Position = view * vec4(pos, 0.0, 1.0);
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

#[repr(C)]
#[derive(Copy, Clone, Debug)]
pub struct Vertex {
    pub pos: [f32; 2],
    pub color: [f32; 4],
}

pub struct Triangles {
    buffer: Buffer,
    vao: VAO,
    program: Program,
    vertex_count: i32,
}

impl Triangles {
    pub fn new() -> Triangles {
        Triangles {
            buffer: Buffer::new(),
            vao: VAO::new(),
            program: Program::new(),
            vertex_count: 0,
        }
    }

    pub fn setup(&mut self) -> Result<(), AppFailure> {
        use core_gl::Object;
        self.buffer.set_debug_name("Triangles.buffer".to_string())?;
        self.vao.set_debug_name("Triangles.program".to_string())?;
        self.program
            .set_debug_name("Triangles.vertex_count".to_string())?;
        let vao = &mut self.vao;
        self.buffer.while_bound(Target::Array, || {
            vao.set_attrib(0, vertex_attrib!(Vertex, pos));
            vao.set_attrib(1, vertex_attrib!(Vertex, color));
        });
        self.program
            .attach(Shader::with_source(Type::Vertex, VERT_SRC.to_string()))
            .attach(Shader::with_source(Type::Fragment, FRAG_SRC.to_string()))
            .link()?;
        self.program.set_uniform(0, &Matrix4::identity());
        Ok(())
    }

    pub fn set_vertices(&mut self, vertices: &Vec<Vertex>) {
        self.buffer.write(Usage::StaticDraw, vertices);
        self.vertex_count = vertices.len() as i32;
    }

    pub fn set_view(&mut self, view: &Matrix4<f32>) {
        self.program.set_uniform(0, view);
    }

    pub fn draw(&mut self) {
        let vertex_count = self.vertex_count;
        let vao = &mut self.vao;
        self.program.while_bound(|| {
            vao.while_bound(|| unsafe {
                gl::DrawArrays(gl::TRIANGLES, 0, vertex_count);
            });
        });
    }
}
