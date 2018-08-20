#include <GL/glew.h>

#include <tetra/gl/DebugLog.hpp>

#include <iostream>
#include <unordered_map>

using namespace std;

namespace
{
const unordered_map<GLenum, string> severity_map = {
    {GL_DEBUG_SEVERITY_LOW, "severity[low]"},
    {GL_DEBUG_SEVERITY_LOW_AMD, "severity[low amd]"},
    {GL_DEBUG_SEVERITY_LOW_ARB, "severity[low arb]"},

    {GL_DEBUG_SEVERITY_MEDIUM, "severity[medium]"},
    {GL_DEBUG_SEVERITY_MEDIUM_AMD, "severity[medium amd]"},
    {GL_DEBUG_SEVERITY_MEDIUM_ARB, "severity[medium arb]"},

    {GL_DEBUG_SEVERITY_HIGH, "severity[high]"},
    {GL_DEBUG_SEVERITY_HIGH_AMD, "severity[high amd]"},
    {GL_DEBUG_SEVERITY_HIGH_ARB, "severity[high arb]"},

    {GL_DEBUG_SEVERITY_NOTIFICATION, "severity[notification]"},
};

const unordered_map<GLenum, string> type_map = {
    {GL_DEBUG_TYPE_MARKER, "type[marker]"},

    {GL_DEBUG_TYPE_POP_GROUP, "type[pop group]"},
    {GL_DEBUG_TYPE_PUSH_GROUP, "type[push group]"},

    {GL_DEBUG_TYPE_ERROR, "type[error]"},
    {GL_DEBUG_TYPE_ERROR_ARB, "type[error arb]"},

    {GL_DEBUG_TYPE_PERFORMANCE, "type[performance]"},
    {GL_DEBUG_TYPE_PERFORMANCE_ARB, "type[performance arb]"},

    {GL_DEBUG_TYPE_PORTABILITY, "type[portability]"},
    {GL_DEBUG_TYPE_PORTABILITY_ARB, "type[portability arb]"},

    {GL_DEBUG_TYPE_OTHER, "type[other]"},
    {GL_DEBUG_TYPE_OTHER_ARB, "type[other arb]"},

    {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "type[undefined behavior]"},
    {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB, "type[undefined behavior arb]"},

    {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "type[deprecated behavior]"},
    {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB, "type[deprecated behavior arb]"},
};

const unordered_map<GLenum, string> source_map = {
    {GL_DEBUG_SOURCE_OTHER, "source[other]"},
    {GL_DEBUG_SOURCE_OTHER_ARB, "source[other arb]"},

    {GL_DEBUG_SOURCE_SHADER_COMPILER, "source[shader compiler]"},
    {GL_DEBUG_SOURCE_SHADER_COMPILER_ARB, "source[shader compiler arb]"},

    {GL_DEBUG_SOURCE_API, "source[api]"},
    {GL_DEBUG_SOURCE_API_ARB, "source[api arb]"},

    {GL_DEBUG_SOURCE_APPLICATION, "source[application]"},
    {GL_DEBUG_SOURCE_APPLICATION_ARB, "source[application arb]"},

    {GL_DEBUG_SOURCE_THIRD_PARTY, "source[third party]"},
    {GL_DEBUG_SOURCE_THIRD_PARTY_ARB, "source[third party arb]"},

    {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "source[window system]"},
    {GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB, "source[window system arb]"},
};

template<typename Key>
const std::string& get_or_default(
    const unordered_map<Key, string>& map,
    const Key& key,
    const string& default_value)
{
    if (map.find(key) != map.end()) {
        return map.at(key);
    }
    return default_value;
}

} // namespace

void tetra::report_gl_error(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userparam)
{
    cout << get_or_default(severity_map, severity, "severity[unknown]") << " "
         << get_or_default(source_map, source, "source[unknown]") << " "
         << get_or_default(type_map, type, "type[unknown]") << " "
         << "id[" << id << "]: " << message << endl;
}
