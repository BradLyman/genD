use core_gl;
use gl;
use gl::types::GLuint;
use std;
use std::ffi::CString;

///
/// This type represents an OpenGL Vertex Array Object.
///
pub struct VAO {
    id: GLuint,
}

impl VAO {
    ///
    /// Create a new vertex array object
    ///
    pub fn new() -> VAO {
        let mut id: GLuint = 0;
        unsafe {
            gl::CreateVertexArrays(1, &mut id);
        }
        VAO { id: id }
    }

    ///
    /// Perform some action while the vertex array object is bound.
    ///
    pub fn while_bound<Func: FnMut() -> ()>(&mut self, mut func: Func) {
        unsafe { gl::BindVertexArray(self.id) }
        func();
        unsafe { gl::BindVertexArray(0) }
    }
}

impl Drop for VAO {
    fn drop(&mut self) {
        unsafe {
            gl::DeleteVertexArrays(1, &self.id);
        }
    }
}

impl core_gl::Object for VAO {
    fn raw(&self) -> GLuint {
        self.id
    }

    fn set_debug_name(
        &mut self,
        name: String,
    ) -> Result<(), std::ffi::NulError> {
        let bytes = CString::new(name)?.into_bytes();
        unsafe {
            gl::ObjectLabel(
                gl::VERTEX_ARRAY,
                self.id,
                bytes.len() as i32,
                bytes.as_ptr() as *const i8,
            );
        }
        Ok(())
    }
}
