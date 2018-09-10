//! This module contains functions and structs for managing view-space
//! transforms used in the example sketches.
//! Common examples are fixed-dimension orthographic projections and ortho
//! projections which automatically match the window's aspect ratio.

mod ortho;
mod viewport;

pub use self::ortho::FixedOrtho;
pub use self::viewport::Viewport;
