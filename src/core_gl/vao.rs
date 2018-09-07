use core_gl;
use gl;
use gl::types::{GLint, GLsizei, GLuint};
use std;
use std::ffi::CString;

///
/// This type represents an OpenGL Vertex Array Object.
///
pub struct VAO {
    id: GLuint,
}

///
/// This type represents a vertex attribute.
/// The vertex_attrib!() macro can be used to build this struct automatically
/// for a struct with [f32; n] members.
///
pub struct VertexAttrib {
    /// The number of components for this attribute must be in [1, 4]
    pub components: i8,

    /// The byte offset between generic vertex attributes (usually the size of
    /// the vertex struct)
    pub stride: i32,

    /// Offset from the beginning of the vertex to the actual part of the
    /// structure used for this attribute.
    pub offset: i32,
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

    ///
    /// Enable and set a vertex attribute pointer.
    ///
    pub fn set_attrib(&mut self, location: u32, attrib: VertexAttrib) {
        self.while_bound(|| unsafe {
            gl::EnableVertexAttribArray(location as GLuint);
            gl::VertexAttribPointer(
                location as GLuint,
                attrib.components as GLint,
                gl::FLOAT,
                gl::FALSE,
                attrib.stride as GLsizei,
                attrib.offset as *const std::os::raw::c_void,
            );
        });
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

#[cfg(test)]
mod test_vertex_attrib {
    use super::*;

    #[derive(Debug, Copy, Clone)]
    #[repr(C, packed)]
    struct Vertex {
        pos: [f32; 2],
        col: [f32; 4],
        singular: [f32; 1],
    }

    #[test]
    fn vertex_attrib_should_have_known_stride() {
        let VertexAttrib { stride, .. } = vertex_attrib!(Vertex, pos);
        assert_eq!(stride, std::mem::size_of::<Vertex>() as i32);
    }

    #[test]
    fn vertex_attrib_should_have_known_component_count() {
        let VertexAttrib { components, .. } = vertex_attrib!(Vertex, pos);
        assert_eq!(components, 2);
    }

    #[test]
    fn vertex_attrib_should_have_known_offset() {
        let VertexAttrib { offset, .. } = vertex_attrib!(Vertex, pos);
        assert_eq!(offset, 0);
    }

    #[test]
    fn computed_component_count_should_match_known_counts() {
        assert_eq!(2, vertex_attrib!(@component_count Vertex, pos));
        assert_eq!(4, vertex_attrib!(@component_count Vertex, col));
        assert_eq!(1, vertex_attrib!(@component_count Vertex, singular));
    }

    #[test]
    fn offset_computed_with_the_macro_should_match_manual_offset() {
        let other_offset = vertex_attrib!(@offset Vertex, col);
        assert_eq!(std::mem::size_of::<[f32; 2]>() as i32, other_offset);
    }

    #[test]
    fn the_offset_computation_technique_should_work() {
        let offset = unsafe {
            let vert = std::mem::uninitialized::<Vertex>();
            let addr = (&vert as *const Vertex) as i32;
            let col_addr = (&vert.col as *const [f32; 4]) as i32;
            std::mem::forget(vert);
            col_addr - addr
        };
        assert_eq!(std::mem::size_of::<[f32; 2]>() as i32, offset);
    }

}
