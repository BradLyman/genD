#ifndef TETRA_GLAPP_HPP
#define TETRA_GLAPP_HPP

#include "gl/Buffer.hpp"
#include "gl/Program.hpp"
#include "gl/VAO.hpp"

namespace tetra
{
class GlApp
{
  public:
    GlApp();
    ~GlApp();

    void render_frame();

  private:
    Program program;
    Buffer vertices;
    VAO vao;
};
} // namespace tetra

#endif
