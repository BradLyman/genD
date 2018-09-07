///
/// Build a vertex attribute struct for a structure with an [f32; n] array
/// component.
///
#[macro_export]
macro_rules! vertex_attrib {
    (@offset $s:ty, $f:ident) => {
        unsafe {
            let vert = ::std::mem::uninitialized::<$s>();
            let addr = (&vert as *const $s) as i32;
            let col_addr = (&vert.$f as *const _) as i32;
            ::std::mem::forget(vert);
            col_addr - addr
        }
    };
    (@component_count $s:ty, $f:ident) => {
        unsafe {
            let vert = ::std::mem::uninitialized::<$s>();
            let size = vert.$f.len();
            ::std::mem::forget(vert);
            debug_assert!(
                size <= 4,
                "Vertex attribute must have four or fewer components"
            );
            size
        }
    };
    ($s:ty, $f:ident) => {
        $crate::core_gl::vao::VertexAttrib {
            components: vertex_attrib!(@component_count $s, $f) as i8,
            offset: vertex_attrib!(@offset $s, $f),
            stride: ::std::mem::size_of::<$s>() as i32,
        }
    };
}
