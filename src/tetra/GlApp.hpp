#ifndef TETRA_GLAPP_HPP
#define TETRA_GLAPP_HPP

#include "ColoredLine.hpp"

namespace tetra
{
class GlApp
{
  public:
    GlApp();
    ~GlApp();

    void render_frame();

    void on_viewport_change(int width, int height);

  private:
    ColoredLine line;
    std::array<float, 16> view;
};
} // namespace tetra

#endif
