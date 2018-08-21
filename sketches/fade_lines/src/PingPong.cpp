#include "GlApp.hpp"

using namespace tetra;
using namespace std;

PingPong::PingPong()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    surface[0].render([]() { glClear(GL_COLOR_BUFFER_BIT); });
    surface[1].render([]() { glClear(GL_COLOR_BUFFER_BIT); });
}

void PingPong::post_process_with(Program& program)
{
    glActiveTexture(GL_TEXTURE0);
    surface[target].raw_tex().while_bound(Texture::Target::Tex2d, [&]() {
        surface[back].render([&]() {
            glClear(GL_COLOR_BUFFER_BIT);
            program.while_bound([&]() { quad.draw(); });
        });
    });
    std::swap(back, target);
}

void PingPong::blit_to_screen() const { surface[target].blit_to_screen(); }

void PingPong::on_viewport_change(int width, int height)
{
    surface[0].resize(width, height);
    surface[1].resize(width, height);
}

PingPong::FullScreenQuad::FullScreenQuad()
{
    struct QuadVert {
        array<float, 2> pos;
        array<float, 2> uv;
    };
    vao.while_bound([&]() {
        quad.while_bound(Buffer::Target::Array, [&]() {
            tetra::set_attrib_pointer(0, &QuadVert::pos);
            tetra::set_attrib_pointer(1, &QuadVert::uv);
        });
    });
    quad.write<QuadVert>({
        {{-1.0f, -1.0f}, {0.0f, 0.0f}},
        {{1.0f, -1.0f}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-1.0f, 1.0f}, {0.0f, 1.0f}},
        {{1.0f, -1.0f}, {1.0f, 0.0f}},
        {{1.0f, 1.0f}, {1.0f, 1.0f}},
    });
}

void PingPong::FullScreenQuad::draw()
{
    vao.while_bound([]() { glDrawArrays(GL_TRIANGLES, 0, 6); });
}
