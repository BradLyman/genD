use gl;
use std;
use std::ffi::CString;

use gl::types::{GLsizeiptr, GLuint, GLvoid};

pub trait Object: Drop {
    fn raw(&self) -> gl::types::GLuint;
    fn set_debug_name(
        &mut self,
        name: String,
    ) -> Result<(), std::ffi::NulError>;
}

pub struct Buffer {
    id: gl::types::GLuint,
}

impl Buffer {
    pub fn new() -> Buffer {
        let id = unsafe {
            let mut temp_id: GLuint = 0;
            gl::CreateBuffers(1, &mut temp_id);
            temp_id
        };
        Buffer { id: id }
    }

    pub fn write<T: Copy + Clone>(&mut self, data: &Vec<T>) {
        let usize = data.len() * std::mem::size_of::<T>();
        let isize = usize as isize;
        unsafe {
            gl::NamedBufferData(
                self.id,
                isize,
                data.as_ptr() as *const std::os::raw::c_void,
                gl::STATIC_DRAW,
            );
        };
    }
}

impl Drop for Buffer {
    fn drop(&mut self) {
        unsafe {
            gl::DeleteBuffers(1, &mut self.id);
        }
    }
}

impl Object for Buffer {
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
                gl::BUFFER,
                self.id,
                bytes.len() as i32,
                bytes.as_ptr() as *const i8,
            );
        }
        Ok(())
    }
}
