//! This module hosts all 2d orthographic projections

use super::Viewport;
use nalgebra as na;

/// The world type represents the part of the world which is visible on screen.
/// World width and height are the arbitrary width and height of the visible
/// world. With any ortho setup, the top of the viewport will be mapped to
/// world.height/2 and the bottom will be mapped to -world.height/2. The same
/// holds true for the width.
#[derive(Copy, Clone, Debug)]
struct World {
    width: f32,
    height: f32,
    projection: na::Orthographic3<f32>,
}

impl World {
    /// Create a new world with dimensions
    pub fn with_size(width: f32, height: f32) -> World {
        World {
            width: width,
            height: height,
            projection: World::symmetric_ortho(width, height),
        }
    }

    /// Resize the world.
    /// Recomputes the world's projection into normalized-device-coordinates.
    pub fn resize(&mut self, width: f32, height: f32) {
        self.width = width;
        self.height = height;
        self.projection = World::symmetric_ortho(width, height);
    }

    fn symmetric_ortho(width: f32, height: f32) -> na::Orthographic3<f32> {
        na::Orthographic3::new(
            -width / 2.0,
            width / 2.0,
            -height / 2.0,
            height / 2.0,
            -1.0,
            1.0,
        )
    }
}

mod fixed_ortho;
pub use self::fixed_ortho::FixedOrtho;
