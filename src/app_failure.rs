use sdl2;
use std;

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

impl From<std::ffi::NulError> for AppFailure {
    fn from(err: std::ffi::NulError) -> AppFailure {
        AppFailure {
            message: "ffi NulError: ".to_owned() + err.description(),
        }
    }
}

impl From<std::string::FromUtf8Error> for AppFailure {
    fn from(err: std::string::FromUtf8Error) -> AppFailure {
        AppFailure {
            message: format!("Failed to convert Utf8: {}", err),
        }
    }
}
