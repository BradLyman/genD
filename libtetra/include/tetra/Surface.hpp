#ifndef TETRA_SURFACE_HPP
#define TETRA_SURFACE_HPP

#include <tetra/gl/Framebuffer.hpp>
#include <tetra/gl/Texture.hpp>

#include <functional>

namespace tetra
{
/**
 * Allocate a new 2D float texture as a surface color target.
 */
void allocate_float_texture(int width, int height, Texture& tex);

/**
 * Objects of this class represent a renderable surface which targets a
 * texture.
 */
class Surface
{
  public:
    using AllocateTexture = std::function<void(int, int, Texture&)>;

  public:
    /**
     * Create a new surface.
     * The texture-allocation function is used to allocate the requried
     * memory for the color-target texture.
     */
    Surface(AllocateTexture alloc_fctn = &allocate_float_texture);
    Surface(Surface& from) = delete;
    Surface(Surface&& from) = default;

    /**
     * Resize the surface target.
     * Contents are discarded.
     */
    void resize(int width, int height);

    /**
     * Render to the surface target.
     * The viewport is automatically configured to cover the entire surface.
     */
    template<typename Fctn>
    void render(Fctn fctn)
    {
        frame.whileBound(Framebuffer::Target::DRAW, [&]() {
            glViewport(0, 0, dims.width, dims.height);
            fctn();
        });
    }

    void blit_to_screen();

    /**
     * The raw texture target for this surface.
     */
    Texture& raw_tex();

  private:
    struct ScreenDims {
        int width;
        int height;
    } dims = {1, 1};

    std::function<void(int, int, Texture&)> allocate_empty_texture;
    Framebuffer frame{};
    Texture tex{};
};
} // namespace tetra

#endif
