use sdl2;

use std::error::Error;

#[derive(Debug, Clone)]
pub struct AppFailure {
    message: String,
}

impl From<String> for AppFailure {
    fn from(msg: String) -> AppFailure {
        AppFailure { message: msg }
    }
}

impl From<sdl2::video::WindowBuildError> for AppFailure {
    fn from(err: sdl2::video::WindowBuildError) -> AppFailure {
        AppFailure {
            message: err.description().to_string(),
        }
    }
}
