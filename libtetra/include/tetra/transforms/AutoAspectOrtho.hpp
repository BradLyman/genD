#ifndef TETRA_AUTO_ASPECT_ORTHO_HPP
#define TETRA_AUTO_ASPECT_ORTHO_HPP

#include <tetra/transforms/FixedOrtho.hpp>

namespace tetra::transforms
{
/**
 * Objects of this class represent an orthograhpic projection from a 2-d
 * world-space to normalized device coordinates.
 * With this transform only the world-height is specified, and the world-width
 * is automatically set using the aspect ratio.
 */
class AutoAspectOrtho
{
  public:
    AutoAspectOrtho(float height = 1.0f);

    /**
     * Resize the viewport.
     */
    void resize_viewport(float width, float height);

    /**
     * Resize the world-space.
     */
    void resize_world(float height);

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
    FixedOrtho ortho;
    float aspect;
    float world_size;
};
} // namespace tetra::transforms
#endif
