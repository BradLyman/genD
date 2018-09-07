use gen_d::app_failure::AppFailure;
use gen_d::primitives::{Triangles, Vertex};
use gen_d::GlApp;
use gl;

pub struct MyApp {
    triangles: Triangles,
    viewport_width: i32,
    viewport_height: i32,
    x: f32,
    y: f32,
}

impl MyApp {
    pub fn build() -> MyApp {
        MyApp {
            triangles: Triangles::new(),
            viewport_width: 1,
            viewport_height: 1,
            x: 0.0,
            y: 0.0,
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
        self.viewport_height = height;
        Ok(())
    }

    fn on_mouse_move(&mut self, x: i32, y: i32) -> Result<(), AppFailure> {
        self.x = {
            let normalized = x as f32 / self.viewport_width as f32;
            let centered = normalized - 0.5;
            centered * 2.0
        };
        self.y = {
            let normalized = y as f32 / self.viewport_height as f32;
            let centered = 0.5 - normalized;
            centered * 2.0
        };
        Ok(())
    }

    fn render_frame(&mut self) -> Result<(), AppFailure> {
        unsafe { gl::Clear(gl::COLOR_BUFFER_BIT) };

        let color = [0.1, 0.15, 0.2, 1.0];
        self.triangles.set_vertices(&vec![
            Vertex {
                pos: [-0.5, -0.5],
                color: color,
            },
            Vertex {
                pos: [self.x, self.y],
                color: [self.x.abs(), 0.2, self.y.abs(), 1.0],
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
