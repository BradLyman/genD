use app_failure::AppFailure;
use core_gl;
use core_gl::shader::Shader;
use gl;
use gl::types::{GLint, GLsizei, GLuint};
use nalgebra;
use std;
use std::ffi::CString;

///
/// This type represents an OpenGL Shader Program
///
pub struct Program {
    id: GLuint,
    shaders: Vec<Shader>,
}

///
/// Implement this trait for a type which can be used as a shader program
/// uniform.
///
pub trait Uniform {
    fn set_gl_value(&self, location: GLint);
}

impl Uniform for nalgebra::Matrix4<f32> {
    fn set_gl_value(&self, location: GLint) {
        unsafe {
            gl::UniformMatrix4fv(
                location,
                1,
                gl::FALSE,
                self.as_slice().as_ptr(),
            );
        }
    }
}

impl Program {
    pub fn new() -> Program {
        Program {
            id: unsafe { gl::CreateProgram() },
            shaders: Vec::new(),
        }
    }

    /// Attach a shader to this program.
    pub fn attach(&mut self, shader: Shader) -> &mut Program {
        self.shaders.push(shader);
        self
    }

    /// Link the attached shaders into a usable program.
    /// Any compile or link failures are reported as an Err.
    pub fn link(&mut self) -> Result<(), AppFailure> {
        self.compile_and_attach_shaders()?;
        unsafe { gl::LinkProgram(self.id) };
        if self.link_failed() {
            Err(AppFailure::from(self.link_failure_msg()?))
        } else {
            Ok(())
        }
    }

    /// Take some action while the program is active.
    pub fn while_bound<Func: FnMut() -> ()>(&mut self, mut func: Func) {
        unsafe { gl::UseProgram(self.id) };
        func();
        unsafe { gl::UseProgram(0) };
    }

    /// Set the value for a uniform at the specified location
    pub fn set_uniform<T: Uniform>(&mut self, location: i32, uniform: &T) {
        self.while_bound(|| uniform.set_gl_value(location));
    }

    /// Compile each shader and attach it to the underlying OpenGL Program
    fn compile_and_attach_shaders(&mut self) -> Result<(), AppFailure> {
        use core_gl::Object;
        for ref mut shader in &mut self.shaders {
            shader.compile()?;
            unsafe {
                gl::AttachShader(self.id, shader.raw());
            }
        }
        Ok(())
    }

    /// Return true if the link failed for any reason
    fn link_failed(&self) -> bool {
        let mut status: GLint = 0;
        unsafe { gl::GetProgramiv(self.id, gl::LINK_STATUS, &mut status) };
        status == (gl::FALSE as GLint)
    }

    fn link_failure_msg(&self) -> Result<String, std::string::FromUtf8Error> {
        let mut buffer: Vec<u8> =
            Vec::with_capacity(self.link_failure_msg_len());
        let mut actual_len: GLint = 0;
        unsafe {
            gl::GetProgramInfoLog(
                self.id,
                buffer.capacity() as GLsizei,
                &mut actual_len,
                buffer.as_mut_ptr() as *mut i8,
            );
            buffer.set_len(actual_len as usize);
        }
        String::from_utf8(buffer)
    }

    fn link_failure_msg_len(&self) -> usize {
        let mut length: GLint = 0;
        unsafe { gl::GetProgramiv(self.id, gl::INFO_LOG_LENGTH, &mut length) };
        length as usize
    }
}

impl Drop for Program {
    fn drop(&mut self) {
        unsafe { gl::DeleteProgram(self.id) }
    }
}

impl core_gl::Object for Program {
    fn raw(&self) -> GLuint {
        self.id
    }

    /// Set the program's name as used in gl debug messages.
    fn set_debug_name(
        &mut self,
        name: String,
    ) -> Result<(), std::ffi::NulError> {
        let bytes = CString::new(name)?.into_bytes();
        unsafe {
            gl::ObjectLabel(
                gl::PROGRAM,
                self.id,
                bytes.len() as i32,
                bytes.as_ptr() as *const i8,
            );
        }
        Ok(())
    }
}
