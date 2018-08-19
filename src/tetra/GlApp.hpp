#ifndef TETRA_GLAPP_HPP
#define TETRA_GLAPP_HPP

#include "ColoredLine.hpp"
#include "Surface.hpp"
#include "gl/Framebuffer.hpp"
#include "gl/Texture.hpp"

namespace tetra
{
class FullScreenQuad
{
  public:
    FullScreenQuad();
    void draw();

  private:
    VAO vao;
    Buffer quad;
};

class GlApp
{
  public:
    GlApp();
    ~GlApp();

    void render_frame();

    void on_viewport_change(int width, int height);

  private:
    template<typename Fctn>
    void ping_pong(Fctn f)
    {
        surface[fade].render([&]() {
            glActiveTexture(GL_TEXTURE0);
            surface[target].raw_tex().whileBound(Texture::Target::Tex2d, f);
        });
        std::swap(fade, target);
    }

  private:
    ColoredLine line;
    std::array<float, 16> view;

    struct ScreenDims {
        float width;
        float height;
    } dims;

    FullScreenQuad quad;
    Program fullScreenRender;

    Surface surface[2];
    int target = 0;
    int fade = 1;
};
} // namespace tetra

#endif
