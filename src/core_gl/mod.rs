use app_failure::AppFailure;
use gl;
use std;
use std::ffi::CString;

use gl::types::{GLsizeiptr, GLuint, GLvoid};

pub mod buffer;

pub trait Object: Drop {
    fn raw(&self) -> gl::types::GLuint;
    fn set_debug_name(
        &mut self,
        name: String,
    ) -> Result<(), std::ffi::NulError>;
}
