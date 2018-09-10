use gl;

pub struct Viewport {
    width: f32,
    height: f32,
}

impl Viewport {
    pub fn new() -> Viewport {
        Viewport::with_size(1.0, 1.0)
    }

    pub fn with_size(width: f32, height: f32) -> Viewport {
        Viewport {
            width: width,
            height: height,
        }
    }

    pub fn resize(&mut self, width: f32, height: f32) {
        self.width = width;
        self.height = height;
    }

    pub fn to_ndc(&self, viewport_coords: (f32, f32)) -> (f32, f32) {
        let (x, y) = viewport_coords;
        let ndc_x = {
            let norm = x / self.width;
            2.0 * (norm - 0.5)
        };
        let ndc_y = {
            let norm = y / self.height;
            2.0 * (0.5 - norm)
        };
        (ndc_x, ndc_y)
    }

    pub fn set_gl_viewport(&self) {
        unsafe { gl::Viewport(0, 0, self.width as i32, self.height as i32) };
    }
}

#[cfg(test)]
mod test {
    //! Test the behavior of the Viewport.
    //! A viewport is a rectangle with (0,0) as the top-left corner and
    //! (width, height) as the bottom right corner.

    use super::*;
    use nearly_equal;
    use std;

    const VIEWPORT_WIDTH: f32 = 10.0;
    const VIEWPORT_HEIGHT: f32 = 10.0;
    const TEST_VIEWPORT: Viewport = Viewport {
        width: VIEWPORT_WIDTH,
        height: VIEWPORT_HEIGHT,
    };

    #[test]
    fn left_of_viewport_should_map_to_left_ndc_coord() {
        let (x, _) = TEST_VIEWPORT.to_ndc((0.0, 0.0));
        assert!(nearly_equal(x, -1.0));
    }

    #[test]
    fn right_of_viewport_should_map_to_right_ndc_coord() {
        let (x, _) = TEST_VIEWPORT.to_ndc((VIEWPORT_WIDTH, 0.0));
        assert!(nearly_equal(x, 1.0));
    }

    #[test]
    fn horizontal_center_of_viewport_should_map_to_center_ndc_coord() {
        let (x, _) = TEST_VIEWPORT.to_ndc((VIEWPORT_WIDTH / 2.0, 0.0));
        assert!(nearly_equal(x, 0.0));
    }

    #[test]
    fn bottom_of_viewport_should_map_to_bottom_ndc_coord() {
        let (_, y) = TEST_VIEWPORT.to_ndc((0.0, VIEWPORT_HEIGHT));
        assert!(nearly_equal(y, -1.0));
    }

    #[test]
    fn top_of_viewport_should_map_to_top_ndc_coord() {
        let (_, y) = TEST_VIEWPORT.to_ndc((0.0, 0.0));
        assert!(nearly_equal(y, 1.0));
    }

    #[test]
    fn vertical_center_of_viewport_should_map_to_center_ndc_coord() {
        let (_, y) = TEST_VIEWPORT.to_ndc((0.0, VIEWPORT_HEIGHT / 2.0));
        assert!(nearly_equal(0.0, y));
    }
}
