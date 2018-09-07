extern crate gl;
extern crate sdl2;

#[macro_use]
pub mod macros;

pub mod app_failure;
pub mod core_gl;
mod gl_app;
mod gl_debug;
pub mod primitives;

pub use gl_app::drive_gl_app;
pub use gl_app::GlApp;
pub use gl_debug::gl_debug_to_stdout;
