#ifndef TETRA_ORTHO_HPP
#define TETRA_ORTHO_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace tetra
{
/**
 * Objects of this class represent an orthograhpic projection of window-space
 * to world-space.
 * Window-space is in pixels (typically) and represents the raw window size.
 * World-space is arbitrary but specified by a single number: size.
 * The world-size number is the number of world-units from the bottom of
 * the screen to the top of the screen.
 */
class Ortho
{
  public:
    Ortho(float size = 1.0f);

    /**
     * Resize the window-space.
     */
    void resize_window(float width, float height);

    /**
     * Resize the world-space.
     */
    void resize_world(float size);

    /**
     * Map window coordinates to world coordinates.
     */
    glm::vec2 world_coords(const glm::vec2& window);

    /**
     * Get a reference to the projection matrix from world-space to normalized
     * device coordinates.
     */
    const glm::mat4& projection() const;

  private:
    struct World {
        float size;
        glm::mat4 projection;
    } world;

    struct Window {
        float width;
        float height;

        float aspect() const;
    } window;
};
} // namespace tetra

#endif
