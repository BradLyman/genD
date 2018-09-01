#include <tetra/transforms/FixedOrtho.hpp>

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;
using namespace tetra::transforms;

namespace
{
glm::mat4 symmetric_ortho(float width, float height)
{
    return glm::ortho(
        -width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f);
}
}; // namespace

FixedOrtho::FixedOrtho(float width, float height)
    : world{width, height, symmetric_ortho(width, height)}, viewport{1.0f, 1.0f}
{
}

void FixedOrtho::viewport_resize(float width, float height)
{
    viewport = {width, height};
}

void FixedOrtho::resize_world(float width, float height)
{
    world = {
        width,
        height,
        symmetric_ortho(width, height),
    };
}

glm::vec2 FixedOrtho::world_coords(const glm::vec2& viewport_coords) const
{
    const vec2 normalized{
        viewport_coords.x / viewport.width,
        viewport_coords.y / viewport.height,
    };
    const vec2 centered{
        normalized.x - 0.5f,
        0.5f - normalized.y,
    };
    const vec2 scaled = centered * 2.0f;
    return {
        scaled.x * world.width,
        scaled.y * world.height,
    };
}

const glm::mat4& FixedOrtho::projection() const { return world.projection; }
