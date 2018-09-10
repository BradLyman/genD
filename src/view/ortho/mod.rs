use super::Viewport;
use nalgebra as na;

#[derive(Copy, Clone, Debug)]
struct World {
    width: f32,
    height: f32,
    projection: na::Orthographic3<f32>,
}

impl World {
    pub fn with_size(width: f32, height: f32) -> World {
        World {
            width: width,
            height: height,
            projection: World::symmetric_ortho(width, height),
        }
    }

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
