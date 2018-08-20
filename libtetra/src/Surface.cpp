#include <tetra/Surface.hpp>

using namespace tetra;
using namespace std;

Surface::Surface(Surface::AllocateTexture alloc_fctn)
    : allocate_empty_texture{alloc_fctn}
{
    allocate_empty_texture(1, 1, tex);
    frame.while_bound(Framebuffer::Target::DRAW, [&]() {
        glFramebufferTexture2D(
            GL_DRAW_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            tex.handle(),
            0);
    });
}

void Surface::resize(int width, int height)
{
    dims.width = width;
    dims.height = height;
    allocate_empty_texture(dims.width, dims.height, tex);
}

void Surface::blit_to_screen() const
{
    glBlitNamedFramebuffer(
        frame.handle(),
        0,
        0,
        0,
        dims.width,
        dims.height,
        0,
        0,
        dims.width,
        dims.height,
        GL_COLOR_BUFFER_BIT,
        GL_LINEAR);
}

Texture& Surface::raw_tex() { return tex; }

void tetra::allocate_float_texture(int width, int height, Texture& tex)
{
    tex.whileBound(Texture::Target::Tex2d, [&]() {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA32F,
            width,
            height,
            0,
            GL_RGBA,
            GL_FLOAT,
            nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    });
}
