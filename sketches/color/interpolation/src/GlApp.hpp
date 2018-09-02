#ifndef TETRA_GLAPP_HPP
#define TETRA_GLAPP_HPP

#include <tetra/primitives/Triangles.hpp>
#include <tetra/sdl/SdlEventPump.hpp>
#include <tetra/transforms/FixedOrtho.hpp>

class GlApp final : public tetra::IWindowEvents
{
  public:
    GlApp();
    ~GlApp();

    virtual void on_frame_render() override;
    virtual void on_viewport_change(int width, int height) override;
    virtual void on_mouse_move(int x, int y) override;

  private:
    tetra::transforms::FixedOrtho ortho;
    tetra::primitives::Triangles triangles;
};

#endif
