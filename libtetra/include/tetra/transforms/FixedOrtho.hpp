#ifndef TETRA_FIXEDORTHO_HPP
#define TETRA_FIXEDORTHO_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace tetra::transforms
{
/**
 * Objects of this class represent an orthograhpic projection from a 2-d
 * world-space to normalized device coordinates.
 */
class FixedOrtho
{
  public:
    FixedOrtho(float width = 1.0f, float height = 1.0f);

    /**
     * Resize the viewport.
     */
    void viewport_resize(float width, float height);

    /**
     * Resize the world-space.
     */
    void resize_world(float width, float height);

    /**
     * Map coordinates in the viewport to world coordinates.
     */
    glm::vec2 world_coords(const glm::vec2& viewport_coords) const;

    /**
     * Get a reference to the projection matrix from world-space to normalized
     * device coordinates.
     */
    const glm::mat4& projection() const;

  private:
    struct World {
        float width;
        float height;
        glm::mat4 projection;
    } world;

    struct Viewport {
        float width;
        float height;

        float aspect() const;
    } viewport;
};
} // namespace tetra::transforms

#endif
