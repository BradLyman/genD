extern crate gl;
extern crate nalgebra;
extern crate sdl2;

#[macro_use]
pub mod macros;

pub mod app_failure;
pub mod core_gl;
mod gl_app;
mod gl_debug;
pub mod primitives;
pub mod view;

pub use gl_app::drive_gl_app;
pub use gl_app::GlApp;
pub use gl_debug::gl_debug_to_stdout;

/// Floating-point comparison of two values.
#[cfg(test)]
fn nearly_equal(lhs: f32, rhs: f32) -> bool {
    let diff = (lhs.abs() - rhs.abs()).abs();
    if lhs == rhs {
        true
    } else if should_use_absolute(lhs, rhs) {
        diff < (std::f32::MIN_POSITIVE * std::f32::EPSILON)
    } else {
        let relative_diff = diff / (lhs.abs() + rhs.abs()).min(std::f32::MAX);
        relative_diff < std::f32::EPSILON
    }
}

// Return true if either value is exactly zero or the difference between
// the values is extremely small.
#[cfg(test)]
fn should_use_absolute(lhs: f32, rhs: f32) -> bool {
    let diff = (lhs.abs() - rhs.abs()).abs();
    lhs == 0.0 || rhs == 0.0 || diff < std::f32::MIN_POSITIVE
}
