#include "GlApp.hpp"

#include <tetra/Color.hpp>
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

    uniform float fade = 0.99999f;
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

GlApp::GlApp() : view{identity()}, line{}
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&tetra::report_gl_error, nullptr);

    constexpr array<float, 4> color = {1.0f, 1.0f, 1.0f, 1.0f};
    line.set_vertices({{{0.0f, 0.0f}, color}, {{1.0f, 0.0f}, color}});

    gaussian_fade.attach(Shader(vertQuad, Shader::Type::Vertex));
    gaussian_fade.attach(Shader(fragQuad, Shader::Type::Fragment));
    gaussian_fade.link();

    // assume that the source texture is in GL_TEXTURE0
    gaussian_fade.while_bound([]() { glUniform1i(0, 0); });
}
GlApp::~GlApp() {}

void GlApp::on_viewport_change(int width, int height)
{
    glViewport(0, 0, width, height);
    ping_pong_surface.on_viewport_change(width, height);
    view = ortho(width, height, 2.3f);
    gaussian_fade.while_bound(
        [&]() { glUniform2f(1, (float)width, (float)height); });
}

void GlApp::on_frame_render()
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
        color = HSL{fmod(drift, 360.0f), 0.85}.as_rgba();
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
            float o1 = offset + i + 0.5;

            vertices.push_back(
                {{cos(oi * (float)fmod(jump, 2)) * sin(1.2f * oi), sin(oi)},
                 color});
            vertices.push_back(
                {{cos(o1 * (float)fmod(jump, 2)) * sin(1.2f * o1), sin(o1)},
                 color});
        }
        line.set_vertices(vertices);

        // render scene
        ping_pong_surface.render([&]() { line.render(view.data()); });

        // horizontal pass
        gaussian_fade.while_bound([]() { glUniform1i(2, 0); });
        ping_pong_surface.post_process_with(gaussian_fade);

        // vertical pass
        gaussian_fade.while_bound([]() { glUniform1i(2, 1); });
        ping_pong_surface.post_process_with(gaussian_fade);

    } else {
        std::this_thread::sleep_for(chrono::milliseconds{1});
    }

    ping_pong_surface.blit_to_screen();
}
