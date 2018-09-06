use gl;

use app_failure::AppFailure;
use core_gl::Object;
use gl::types::{GLenum, GLuint};
use std;
use std::ffi::CString;

///
/// This type represents an OpenGL buffer object.
///
pub struct Buffer {
    id: gl::types::GLuint,
    size: usize,
}

/// Buffer usage
pub enum Usage {
    StreamDraw,
    StreamRead,
    StreamCopy,
    StaticDraw,
    StaticRead,
    StaticCopy,
    DynamicDraw,
    DynamicRead,
    DynamicCopy,
}

/// Buffer bind targets
pub enum Target {
    Array,
    AtomicCounter,
    CopyRead,
    CopyWrite,
    DispatchIndirect,
    DrawIndirect,
    ElementArray,
    PixelPack,
    PixelUnpack,
    Query,
    ShaderStorage,
    TextureBuffer,
    TransformFeedback,
    Uniform,
}

impl Buffer {
    ///
    /// Create a new buffer with no data.
    ///
    pub fn new() -> Buffer {
        let id = unsafe {
            let mut temp_id: GLuint = 0;
            gl::CreateBuffers(1, &mut temp_id);
            temp_id
        };
        Buffer { id: id, size: 0 }
    }

    ///
    /// Take some action while the buffer is bound to the specified target.
    ///
    pub fn while_bound<Func: FnMut() -> ()>(
        &mut self,
        target: Target,
        mut func: Func,
    ) {
        let raw_target = target_as_enum(target);
        unsafe { gl::BindBuffer(raw_target, self.id) };
        func();
        unsafe { gl::BindBuffer(raw_target, 0) };
    }

    ///
    /// Copy the provided data into the gpu buffer.
    ///
    pub fn write<T: Copy + Clone>(&mut self, usage: Usage, data: &Vec<T>) {
        self.size = data.len() * std::mem::size_of::<T>();
        let isize = self.size as isize;
        unsafe {
            gl::NamedBufferData(
                self.id,
                isize,
                data.as_ptr() as *const std::os::raw::c_void,
                usage_as_enum(usage),
            );
        };
    }

    ///
    /// Read the data from the buffer as the specified type.
    /// Fails if the provided type isn't a divisor of the buffer's byte size.
    ///
    pub fn read<T: Copy + Clone>(&mut self) -> Result<Vec<T>, AppFailure> {
        let count = self.count_of::<T>()?;
        let mut buffer = Vec::<T>::with_capacity(count);
        unsafe {
            gl::GetNamedBufferSubData(
                self.id,
                0,
                self.size as isize,
                buffer.as_mut_ptr() as *mut std::os::raw::c_void,
            );
            buffer.set_len(count);
        }
        Ok(buffer)
    }

    fn count_of<T: Copy + Clone>(&self) -> Result<usize, String> {
        let count = self.size % std::mem::size_of::<T>();
        if count != 0 {
            Err("Cannot read buffer into a non-divisible type".to_string())
        } else {
            Ok(self.size / std::mem::size_of::<T>())
        }
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
    /// The raw OpenGL buffer ID.
    fn raw(&self) -> GLuint {
        self.id
    }

    /// Set the buffer's name as used in gl debug messages.
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

/// Map the buffer usage to its matching GLenum.
fn usage_as_enum(usage: Usage) -> GLenum {
    match usage {
        Usage::StaticCopy => gl::STATIC_COPY,
        Usage::StaticDraw => gl::STATIC_DRAW,
        Usage::StaticRead => gl::STATIC_READ,
        Usage::DynamicCopy => gl::DYNAMIC_COPY,
        Usage::DynamicDraw => gl::DYNAMIC_DRAW,
        Usage::DynamicRead => gl::DYNAMIC_READ,
        Usage::StreamCopy => gl::STREAM_COPY,
        Usage::StreamDraw => gl::STREAM_DRAW,
        Usage::StreamRead => gl::STREAM_READ,
    }
}

fn target_as_enum(target: Target) -> GLenum {
    match target {
        Target::Array => gl::ARRAY_BUFFER,
        Target::AtomicCounter => gl::ATOMIC_COUNTER_BUFFER,
        Target::CopyRead => gl::COPY_READ_BUFFER,
        Target::CopyWrite => gl::COPY_WRITE_BUFFER,
        Target::DispatchIndirect => gl::DISPATCH_INDIRECT_BUFFER,
        Target::DrawIndirect => gl::DRAW_INDIRECT_BUFFER,
        Target::ElementArray => gl::ELEMENT_ARRAY_BUFFER,
        Target::PixelPack => gl::PIXEL_PACK_BUFFER,
        Target::PixelUnpack => gl::PIXEL_UNPACK_BUFFER,
        Target::Query => gl::QUERY_BUFFER,
        Target::ShaderStorage => gl::SHADER_STORAGE_BUFFER,
        Target::TextureBuffer => gl::TEXTURE_BUFFER,
        Target::TransformFeedback => gl::TRANSFORM_FEEDBACK_BUFFER,
        Target::Uniform => gl::UNIFORM_BUFFER,
    }
}
