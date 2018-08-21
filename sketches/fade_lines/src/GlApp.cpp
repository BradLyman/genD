#include "GlApp.hpp"

#include <tetra/Countdown.hpp>
#include <tetra/Mat4x4.hpp>
#include <tetra/Turtle.hpp>
#include <tetra/gl/DebugLog.hpp>

#include <cmath>
#include <iostream>
#include <stack>
#include <thread>

using namespace tetra;
using namespace std;

const char* vertQuad = R"src(
#version 450

layout (location=0) in vec2 pos;
layout (location=1) in vec2 uv;

out vec2 varyUv;

void main() {
    varyUv = uv;
    gl_Position = vec4(pos, 0.0f, 1.0f);
}
)src";

const char* fragQuad = R"src(
#version 450

layout (location=0) uniform sampler2D img;
layout (location=1) uniform vec2 screen;
layout (location=2) uniform int vert;

uniform float fade = 0.999999f;
uniform float weight[5] = float[](
    0.22702702702702704,
    0.1945945945945946,
    0.12162162162162163,
    0.05405405405405406,
    0.016216216216216217
);

in vec2 varyUv;
out vec4 vFragColor;

vec2 xOffset(int i) {
    return vec2(i / screen.x, 0.0f);
}

vec2 yOffset(int i) {
    return vec2(0.0f, i / screen.y);
}

void main() {
    vec4 accum = texture2D(img, varyUv) * weight[0];
    for (int i = 1; i < 5; i++) {
        vec2 step = xOffset(i);
        if (vert == 1) step = yOffset(i);

        accum += texture2D(img, varyUv + step) * weight[i];
        accum += texture2D(img, varyUv - step) * weight[i];
    }
    vFragColor = vec4(accum.rgb, accum.a*fade);
}
)src";

GlApp::GlApp() : view{identity()}, line{}, dims{1.0f, 1.0f}
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&tetra::report_gl_error, nullptr);

    surface[0].render([&]() { glClear(GL_COLOR_BUFFER_BIT); });
    surface[1].render([&]() { glClear(GL_COLOR_BUFFER_BIT); });

    constexpr array<float, 4> color = {1.0f, 1.0f, 1.0f, 1.0f};
    line.set_vertices({{{0.0f, 0.0f}, color}, {{1.0f, 0.0f}, color}});

    fullScreenRender.attach(Shader(vertQuad, Shader::Type::Vertex));
    fullScreenRender.attach(Shader(fragQuad, Shader::Type::Fragment));
    fullScreenRender.link();

    fullScreenRender.while_bound([]() { glUniform1i(0, 0); });
}
GlApp::~GlApp() {}

void GlApp::on_viewport_change(int width, int height)
{
    dims = {(float)width, (float)height};
    glViewport(0, 0, width, height);
    view = ortho(width, height, 2.3f);
    surface[0].resize(width, height);
    surface[1].resize(width, height);

    fullScreenRender.while_bound(
        [&]() { glUniform2f(1, dims.width, dims.height); });
}

namespace
{
std::array<float, 3> raw_rgb(float chroma, float hue_norm)
{
    const float x = chroma * (1.0f - abs(fmod(hue_norm, 2) - 1));

    if (0 <= hue_norm && hue_norm < 1)
        return {chroma, x, 0};
    else if (1 <= hue_norm && hue_norm < 2)
        return {x, chroma, 0};
    else if (2 <= hue_norm && hue_norm < 3)
        return {0, chroma, x};
    else if (3 <= hue_norm && hue_norm < 4)
        return {0, x, chroma};
    else if (4 <= hue_norm && hue_norm < 5)
        return {x, 0, chroma};
    else
        return {chroma, 0, x};
}

std::array<float, 3> hsl_to_rgb(std::array<float, 3> hsl)
{
    using std::abs;
    using std::fmod;
    const float chroma = (1.0f - abs(2.0f * hsl[2])) * hsl[1];
    const float match = hsl[2] - (chroma / 2.0f);
    const std::array<float, 3> rgb = raw_rgb(chroma, hsl[0] / 60.0f);
    return {rgb[0] + match, rgb[1] + match, rgb[2] + match};
}

}; // namespace

void GlApp::render_frame()
{
    static array<float, 4> color = {0.5f, 0.5f, 1.0f, 1.0f};
    static Countdown newpoint;
    static float offset = 0.0f;

    static Countdown colorDrift;
    static float drift = 0.0f;

    static Countdown jumpTime;
    static float jump = 0.0f;

    if (colorDrift.time_up()) {
        colorDrift.reset(chrono::milliseconds{500});
        drift += 5.0f;
        auto rgb = hsl_to_rgb({fmod(drift, 360.0f), 1.0f, 1.0f});
        color = {rgb[0], rgb[1], rgb[2], 1.0f};
    }

    if (jumpTime.time_up()) {
        jumpTime.reset(chrono::milliseconds{700});
        jump += 0.1f;
    }

    if (newpoint.time_up()) {
        newpoint.reset(chrono::milliseconds{16});
        offset += 0.1;

        vector<ColoredLine::Vertex> vertices;
        for (float i = 0.0f; i < 3.0f; i += 0.5f) {
            float oi = offset + i;
            float o1 = offset + i + 0.2;

            vertices.push_back(
                {{cos(oi * (float)fmod(jump, 2)) * sin(1.2f * oi), sin(oi)},
                 color});
            vertices.push_back(
                {{cos(o1 * (float)fmod(jump, 2)) * sin(1.2f * o1), sin(o1)},
                 color});
        }
        line.set_vertices(vertices);

        // render scene
        surface[target].render([&]() { line.render(view.data()); });

        // horizontal pass
        fullScreenRender.while_bound([]() { glUniform1i(2, 0); });
        ping_pong([&]() {
            glClear(GL_COLOR_BUFFER_BIT);
            fullScreenRender.while_bound([&]() { quad.draw(); });
        });

        // vertical pass
        fullScreenRender.while_bound([]() { glUniform1i(2, 1); });
        ping_pong([&]() {
            glClear(GL_COLOR_BUFFER_BIT);
            fullScreenRender.while_bound([&]() { quad.draw(); });
        });

    } else {
        std::this_thread::sleep_for(chrono::milliseconds{1});
    }

    surface[target].blit_to_screen();
}

FullScreenQuad::FullScreenQuad()
{
    struct QuadVert {
        array<float, 2> pos;
        array<float, 2> uv;
    };
    vao.whileBound([&]() {
        glBindBuffer(GL_ARRAY_BUFFER, quad.handle());
        tetra::attribPointer(0, &QuadVert::pos);
        tetra::attribPointer(1, &QuadVert::uv);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void FullScreenQuad::draw()
{
    vao.whileBound([]() { glDrawArrays(GL_TRIANGLES, 0, 6); });
}
