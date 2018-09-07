use gl;
use std;

pub mod buffer;
pub mod program;
pub mod shader;

pub mod vao;

pub trait Object: Drop {
    fn raw(&self) -> gl::types::GLuint;
    fn set_debug_name(
        &mut self,
        name: String,
    ) -> Result<(), std::ffi::NulError>;
}
