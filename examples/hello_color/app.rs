use gen_d::app_failure::AppFailure;
use gen_d::primitives::{Triangles, Vertex};
use gen_d::view::FixedOrtho;
use gen_d::GlApp;
use gl;

pub struct MyApp {
    triangles: Triangles,
    x: f32,
    y: f32,
    view: FixedOrtho,
}

impl MyApp {
    pub fn build() -> MyApp {
        MyApp {
            triangles: Triangles::new(),
            x: 0.0,
            y: 0.0,
            view: FixedOrtho::with_size(1024.0, 768.0),
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
        self.view.resize_viewport(width as f32, height as f32);
        self.triangles.set_view(self.view.projection());
        Ok(())
    }

    fn on_mouse_move(&mut self, x: i32, y: i32) -> Result<(), AppFailure> {
        let (w_x, w_y) = self.view.to_world_coords((x as f32, y as f32));
        self.x = w_x;
        self.y = w_y;
        Ok(())
    }

    fn render_frame(&mut self) -> Result<(), AppFailure> {
        self.view.set_gl_viewport();
        unsafe { gl::Clear(gl::COLOR_BUFFER_BIT) };
        let color = [0.1, 0.15, 0.2, 1.0];
        self.triangles.set_vertices(&vec![
            Vertex {
                pos: [-250.0, -250.0],
                color: color,
            },
            Vertex {
                pos: [self.x, self.y],
                color: [0.0, 0.8, 0.0, 1.0],
            },
            Vertex {
                pos: [0.0, 250.0],
                color: color,
            },
        ]);
        self.triangles.draw();
        return Ok(());
    }
}
