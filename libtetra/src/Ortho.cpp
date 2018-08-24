#include <tetra/Ortho.hpp>

#include <glm/gtc/matrix_transform.hpp>

using namespace tetra;
using glm::vec2;

namespace
{
glm::mat4 ortho_for(float aspect, float world_height)
{
    const float world_width = world_height * aspect;
    return glm::ortho(
        -world_width / 2.0f,
        world_width / 2.0f,
        -world_height / 2.0f,
        world_height / 2.0f);
}
} // namespace

Ortho::Ortho(float size) : world{size, glm::mat4{1.0f}}, window{1.0f, 1.0f} {}

const glm::mat4& Ortho::projection() const { return world.projection; }

void Ortho::resize_window(float width, float height)
{
    window = {width, height};
    world.projection = ortho_for(window.aspect(), world.size);
}

void Ortho::resize_world(float size)
{
    world.size = size;
    world.projection = ortho_for(window.aspect(), world.size);
}

vec2 Ortho::world_coords(const vec2& window_coords)
{
    const vec2 centered = {
        (window_coords.x * -1) + (window.width / 2.0f),
        window_coords.y - (window.height / 2.0f),
    };
    const vec2 normalized = {
        centered.x / window.width,
        centered.y / window.height,
    };
    const vec2 scaled = {
        normalized.x * world.size * window.aspect(),
        normalized.y * world.size,
    };
    return scaled;
}

float Ortho::Window::aspect() const { return width / height; }
