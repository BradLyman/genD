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
    println!(
        "{:?} {:?} {:?} {:?} {:?}",
        msg_severity(severity),
        msg_source(source),
        msg_type(message_type),
        id,
        message
    );
}

fn msg_source(source: GLenum) -> &'static str {
    match source {
        gl::DEBUG_SOURCE_API => "source[api]",
        gl::DEBUG_SOURCE_WINDOW_SYSTEM => "source[window system]",
        gl::DEBUG_SOURCE_SHADER_COMPILER => "source[shader compiler]",
        gl::DEBUG_SOURCE_THIRD_PARTY => "source[third party]",
        gl::DEBUG_SOURCE_APPLICATION => "source[application]",
        gl::DEBUG_SOURCE_OTHER => "source[other]",
        _ => "source[unknown]",
    }
}

fn msg_type(msg_type: GLenum) -> &'static str {
    match msg_type {
        gl::DEBUG_TYPE_ERROR => "type[error]",
        gl::DEBUG_TYPE_DEPRECATED_BEHAVIOR => "type[deprecated behavior]",
        gl::DEBUG_TYPE_UNDEFINED_BEHAVIOR => "type[undefined behavior]",
        gl::DEBUG_TYPE_PORTABILITY => "type[portability]",
        gl::DEBUG_TYPE_PERFORMANCE => "type[performance]",
        gl::DEBUG_TYPE_MARKER => "type[marker]",
        gl::DEBUG_TYPE_PUSH_GROUP => "type[push group]",
        gl::DEBUG_TYPE_POP_GROUP => "type[pop group]",
        gl::DEBUG_TYPE_OTHER => "type[other]",
        _ => "type[unknown]",
    }
}

fn msg_severity(severity: GLenum) -> &'static str {
    match severity {
        gl::DEBUG_SEVERITY_HIGH => "severity[high]",
        gl::DEBUG_SEVERITY_MEDIUM => "severity[medium]",
        gl::DEBUG_SEVERITY_LOW => "severity[low]",
        gl::DEBUG_SEVERITY_NOTIFICATION => "severity[notification]",
        _ => "severity[unknown]",
    }
}
