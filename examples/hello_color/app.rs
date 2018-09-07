use gen_d::app_failure::AppFailure;
use gen_d::primitives::{Triangles, Vertex};
use gen_d::GlApp;
use gl;

pub struct MyApp {
    triangles: Triangles,
    viewport_width: i32,
    x: f32,
}

impl MyApp {
    pub fn build() -> MyApp {
        MyApp {
            triangles: Triangles::new(),
            viewport_width: 1,
            x: 0.0,
        }
    }
}

impl GlApp for MyApp {
    fn setup(&mut self) -> Result<(), AppFailure> {
        unsafe { gl::ClearColor(0.0, 0.0, 0.0, 1.0) };
        self.triangles.setup()?;
        Ok(())
    }

    fn on_viewport_resize(
        &mut self,
        width: i32,
        height: i32,
    ) -> Result<(), AppFailure> {
        unsafe { gl::Viewport(0, 0, width, height) };
        self.viewport_width = width;
        Ok(())
    }

    fn on_mouse_move(&mut self, x: i32, _: i32) -> Result<(), AppFailure> {
        self.x = {
            let normalized = x as f32 / self.viewport_width as f32;
            let centered = normalized - 0.5;
            centered * 2.0
        };
        Ok(())
    }

    fn render_frame(&mut self) -> Result<(), AppFailure> {
        unsafe { gl::Clear(gl::COLOR_BUFFER_BIT) };

        let color = [0.5, 0.0, 0.9, 1.0];
        self.triangles.set_vertices(&vec![
            Vertex {
                pos: [-0.5, -0.5],
                color: color,
            },
            Vertex {
                pos: [self.x, -0.5],
                color: color,
            },
            Vertex {
                pos: [0.0, 0.5],
                color: color,
            },
        ]);
        self.triangles.draw();
        return Ok(());
    }
}
