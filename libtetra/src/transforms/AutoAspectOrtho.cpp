#include <tetra/transforms/AutoAspectOrtho.hpp>

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;
using namespace tetra::transforms;

AutoAspectOrtho::AutoAspectOrtho(float size)
    : ortho{size, size}, world_size{size}, aspect{1.0f}
{
}

void AutoAspectOrtho::resize_viewport(float width, float height)
{
    aspect = width / height;
    ortho.resize_viewport(width, height);
    ortho.resize_world(world_size * aspect, world_size);
}

void AutoAspectOrtho::resize_world(float size)
{
    world_size = size;
    ortho.resize_world(world_size * aspect, world_size);
}

glm::vec2 AutoAspectOrtho::world_coords(const glm::vec2& viewport_coords) const
{
    return ortho.world_coords(viewport_coords);
}

const glm::mat4& AutoAspectOrtho::projection() const
{
    return ortho.projection();
}
