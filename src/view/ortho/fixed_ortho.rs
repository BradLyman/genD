use super::Viewport;
use super::World;
use nalgebra as na;

/// The FixedOrtho type represents an orthographic coordinate space which
/// does not change.
/// This means that the world-coordinates stay the same regardless of the
/// viewport size/aspect ratio. Static coordinates like this can make geometry
/// distort when the viewport is resized.
pub struct FixedOrtho {
    viewport: Viewport,
    world: World,
}

impl FixedOrtho {
    /// Create a new instance with the specified world size.
    pub fn with_size(width: f32, height: f32) -> FixedOrtho {
        FixedOrtho {
            viewport: Viewport::new(),
            world: World::with_size(width, height),
        }
    }

    pub fn resize_viewport(&mut self, width: f32, height: f32) {
        self.viewport.resize(width, height);
    }

    pub fn resize_world(&mut self, width: f32, height: f32) {
        self.world.resize(width, height);
    }

    pub fn set_gl_viewport(&self) {
        self.viewport.set_gl_viewport();
    }

    /// Get the Projection from world space to ndc space.
    /// Use this for shaders and similar.
    pub fn projection(&self) -> &na::Matrix4<f32> {
        self.world.projection.as_matrix()
    }

    /// Map viewport coordinates into world space.
    pub fn to_world_coords(&self, viewport_coords: (f32, f32)) -> (f32, f32) {
        let (ndc_x, ndc_y) = self.viewport.to_ndc(viewport_coords);
        let world_coord = self
            .world
            .projection
            .unproject_point(&na::Point3::new(ndc_x, ndc_y, 0.0));
        (world_coord.x, world_coord.y)
    }
}

#[cfg(test)]
mod test {
    use super::*;
    use nearly_equal;

    const VIEWPORT_WIDTH: f32 = 100.0;
    const VIEWPORT_HEIGHT: f32 = 100.0;
    const WORLD_WIDTH: f32 = 41.0;
    const WORLD_HEIGHT: f32 = 37.0;

    fn fixed_ortho() -> FixedOrtho {
        let mut ortho = FixedOrtho::with_size(WORLD_WIDTH, WORLD_HEIGHT);
        ortho.resize_viewport(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
        ortho
    }

    #[test]
    fn left_of_viewport_should_map_to_left_of_world() {
        let (x, _) = fixed_ortho().to_world_coords((0.0, 0.0));
        assert!(nearly_equal(x, -WORLD_WIDTH / 2.0));
    }

    #[test]
    fn right_of_viewport_should_map_to_right_of_world() {
        let (x, _) = fixed_ortho().to_world_coords((VIEWPORT_WIDTH, 0.0));
        assert!(nearly_equal(x, WORLD_WIDTH / 2.0));
    }

    #[test]
    fn horizontal_center_of_viewport_should_map_to_center_of_world() {
        let (x, _) = fixed_ortho().to_world_coords((VIEWPORT_WIDTH / 2.0, 0.0));
        assert!(nearly_equal(x, 0.0));
    }

    #[test]
    fn top_of_viewport_should_map_to_top_of_world() {
        let (_, y) = fixed_ortho().to_world_coords((0.0, 0.0));
        assert!(nearly_equal(y, WORLD_HEIGHT / 2.0));
    }

    #[test]
    fn bottom_of_viewport_should_map_to_bottom_of_world() {
        let (_, y) = fixed_ortho().to_world_coords((0.0, VIEWPORT_HEIGHT));
        assert!(nearly_equal(y, -WORLD_HEIGHT / 2.0));
    }

    #[test]
    fn vertical_center_of_viewport_should_map_to_center_of_world() {
        let (_, y) =
            fixed_ortho().to_world_coords((0.0, VIEWPORT_HEIGHT / 2.0));
        assert!(nearly_equal(y, 0.0));
    }
}
