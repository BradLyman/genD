use gl;
use gl::types::{GLchar, GLenum, GLsizei, GLuint};
use std;

///
/// Dump a human-readable representation of the debug info to stdout.
/// Printing is done immediately, so this is likely to be a performance
/// bottleneck.
///
pub extern "system" fn gl_debug_to_stdout(
    source: GLenum,
    message_type: GLenum,
    id: GLuint,
    severity: GLenum,
    length: GLsizei,
    message: *const GLchar,
    user: *mut std::os::raw::c_void,
) -> () {
    let safe_msg = {
        let cstr = unsafe { std::ffi::CStr::from_ptr(message) };

        match cstr.to_owned().into_string() {
            Ok(msg) => msg,
            Err(_) => "unable to read GL debug string".to_string(),
        }
    };
    println!(
        "severity[{}] source[{}] type[{}] id[{}]: {}",
        msg_severity(severity),
        msg_source(source),
        msg_type(message_type),
        id,
        safe_msg
    );
}

fn msg_source(source: GLenum) -> &'static str {
    match source {
        gl::DEBUG_SOURCE_API => "api",
        gl::DEBUG_SOURCE_WINDOW_SYSTEM => "window system",
        gl::DEBUG_SOURCE_SHADER_COMPILER => "shader compiler",
        gl::DEBUG_SOURCE_THIRD_PARTY => "third party",
        gl::DEBUG_SOURCE_APPLICATION => "application",
        gl::DEBUG_SOURCE_OTHER => "other",
        _ => "unknown",
    }
}

fn msg_type(msg_type: GLenum) -> &'static str {
    match msg_type {
        gl::DEBUG_TYPE_ERROR => "error",
        gl::DEBUG_TYPE_DEPRECATED_BEHAVIOR => "deprecated behavior",
        gl::DEBUG_TYPE_UNDEFINED_BEHAVIOR => "undefined behavior",
        gl::DEBUG_TYPE_PORTABILITY => "portability",
        gl::DEBUG_TYPE_PERFORMANCE => "performance",
        gl::DEBUG_TYPE_MARKER => "marker",
        gl::DEBUG_TYPE_PUSH_GROUP => "push group",
        gl::DEBUG_TYPE_POP_GROUP => "pop group",
        gl::DEBUG_TYPE_OTHER => "other",
        _ => "unknown",
    }
}

fn msg_severity(severity: GLenum) -> &'static str {
    match severity {
        gl::DEBUG_SEVERITY_HIGH => "high",
        gl::DEBUG_SEVERITY_MEDIUM => "medium",
        gl::DEBUG_SEVERITY_LOW => "low",
        gl::DEBUG_SEVERITY_NOTIFICATION => "notification",
        _ => "unknown",
    }
}
