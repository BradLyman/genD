use app_failure::AppFailure;
use core_gl;
use gl;
use gl::types::{GLenum, GLint, GLsizei, GLuint};
use std;
use std::ffi::CString;
use std::string::FromUtf8Error;

///
/// Shader Types
///
#[derive(Debug, Clone, Copy)]
pub enum Type {
    Compute,
    Vertex,
    TessControl,
    TessEvaluation,
    Geometry,
    Fragment,
}

///
/// This type represents an OpenGL Shader Object.
///
pub struct Shader {
    id: GLuint,
    source: String,
    shader_type: Type,
}

impl Shader {
    /// Create a shader with the specified source
    pub fn with_source(shader_type: Type, source: String) -> Shader {
        let id = unsafe { gl::CreateShader(type_as_enum(shader_type)) };
        Shader {
            id: id,
            source: source.clone(),
            shader_type: shader_type,
        }
    }

    /// Compile the shader.
    pub fn compile(&mut self) -> Result<(), AppFailure> {
        self.compile_or_fail().map_err(|err| {
            AppFailure::from(format!(
                "Could not compile {:?} shader: {:?}",
                self.shader_type, err
            ))
        })
    }

    /// Attempt to compile the shader.
    /// If compiling fails for any reason then the result is surfaced as an
    /// AppFailure.
    fn compile_or_fail(&mut self) -> Result<(), AppFailure> {
        self.save_source()?;
        unsafe { gl::CompileShader(self.id) };
        if self.compile_failed() {
            Err(AppFailure::from(self.failure_message()?))
        } else {
            Ok(())
        }
    }

    /// Save the source to the OpenGL Shader Object.
    fn save_source(&mut self) -> Result<(), std::ffi::NulError> {
        let ffi_source = CString::new(self.source.clone())?;
        let src_ptr: *const std::os::raw::c_char = ffi_source.as_ptr();
        unsafe {
            gl::ShaderSource(self.id, 1, &src_ptr, 0 as _);
        }
        Ok(())
    }

    /// Check to see if compilation failed.
    fn compile_failed(&self) -> bool {
        let mut param: GLint = gl::TRUE as i32;
        unsafe {
            gl::GetShaderiv(self.id, gl::COMPILE_STATUS, &mut param);
        }
        param == (gl::FALSE as i32)
    }

    /// Get the OpenGL-generated compiler error as an owned string.
    fn failure_message(&self) -> Result<String, FromUtf8Error> {
        let mut buffer: Vec<u8> =
            Vec::with_capacity(self.failure_message_len() as usize);
        unsafe {
            let mut actual_len: GLsizei = 0;
            gl::GetShaderInfoLog(
                self.id,
                buffer.capacity() as GLsizei,
                &mut actual_len,
                buffer.as_mut_ptr() as *mut i8,
            );
            buffer.set_len(actual_len as usize);
        }
        Ok(String::from_utf8(buffer)?)
    }

    fn failure_message_len(&self) -> GLint {
        let mut length: GLint = 0;
        unsafe {
            gl::GetShaderiv(self.id, gl::INFO_LOG_LENGTH, &mut length);
        }
        return length;
    }
}

impl Drop for Shader {
    fn drop(&mut self) {
        unsafe { gl::DeleteShader(self.id) }
    }
}

impl core_gl::Object for Shader {
    /// The raw id for the Shader
    fn raw(&self) -> GLuint {
        self.id
    }

    /// Set the shader's name as used in gl debug messages.
    fn set_debug_name(
        &mut self,
        name: String,
    ) -> Result<(), std::ffi::NulError> {
        let bytes = CString::new(name)?.into_bytes();
        unsafe {
            gl::ObjectLabel(
                gl::SHADER,
                self.id,
                bytes.len() as i32,
                bytes.as_ptr() as *const i8,
            );
        }
        Ok(())
    }
}

fn type_as_enum(shader_type: Type) -> GLenum {
    match shader_type {
        Type::Compute => gl::COMPUTE_SHADER,
        Type::Vertex => gl::VERTEX_SHADER,
        Type::TessControl => gl::TESS_CONTROL_SHADER,
        Type::TessEvaluation => gl::TESS_EVALUATION_SHADER,
        Type::Geometry => gl::GEOMETRY_SHADER,
        Type::Fragment => gl::FRAGMENT_SHADER,
    }
}
