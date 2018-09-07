use gen_d::app_failure::AppFailure;
use gen_d::primitives::{Triangles, Vertex};
use gen_d::GlApp;
use gl;
use std;

pub struct MyApp {
    triangles: Triangles,
}

impl MyApp {
    pub fn build() -> MyApp {
        MyApp {
            triangles: Triangles::new(),
        }
    }
}

impl GlApp for MyApp {
    fn setup(&mut self) -> Result<(), AppFailure> {
        unsafe { gl::ClearColor(0.0, 0.0, 0.0, 1.0) };
        self.triangles.setup()?;
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
                pos: [0.5, -0.5],
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
