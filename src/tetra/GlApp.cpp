#include <glad/glad.h>

#include "GlApp.hpp"

#include "Countdown.hpp"
#include "Mat4x4.hpp"
#include "Turtle.hpp"
#include "gl/DebugLog.hpp"

#include <cmath>
#include <iostream>
#include <stack>
#include <thread>

using namespace tetra;
using namespace std;

string step(const string& seed)
{
    string result;
    for (char c : seed) {
        if (c == 'x') {
            switch (rand() % 2) {
            case 1:
                result += "f[+x][-x]";
                break;
            case 0:
                result += "x[+fx[+f][-x]][-fx]";
                break;
            }
        } else if (c == 'f') {
            result += "ff";
        } else {
            result += c;
        }
    }
    return result;
}

GlApp::GlApp() : view{identity()}, line{}
{
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&reportGlError, nullptr);
}
GlApp::~GlApp() {}

void GlApp::on_viewport_change(int width, int height)
{
    glViewport(0, 0, width, height);
    view = ortho(width, height, 20.0f);
}

void GlApp::render_frame()
{
    static vector<ColoredLine::Vertex> vertices{};

    constexpr int max = 7;
    static const string reset = "x";
    static string seed = step(step(step(step(step(step(reset))))));
    static int currentStep = 0;
    static Countdown newTree;
    static Countdown addStep;

    static float angleTime = 0.0f;
    static float frameAngle = 25.0f + 0.2 * sin(angleTime);
    static Countdown angleTurn;

    glClear(GL_COLOR_BUFFER_BIT);

    if (newTree.time_up()) {
        newTree.reset(chrono::milliseconds{5000});

        seed = step(reset);
        currentStep = 1;
    }

    if (addStep.time_up()) {
        addStep.reset(chrono::milliseconds{500});

        currentStep += 1;
        if (currentStep < max) {
            seed = step(seed);
        }
    }

    if (angleTurn.time_up()) {
        angleTurn.reset(chrono::milliseconds{16});
        angleTime += 0.01f;
        frameAngle = 25.0f + 1.0f * sin(angleTime);
    }

    Turtle turtle{vertices, {0.0f, -8.0f}};
    turtle.set_step_size(0.1f);
    stack<Turtle> turtles{};
    for (char c : seed) {
        switch (c) {
        case 'x':
            break;
        case 'f':
            turtle.forward();
            break;
        case '+':
            turtle.turn(frameAngle);
            break;
        case '-':
            turtle.turn(-frameAngle);
            break;
        case '[':
            turtles.push(turtle);
            break;
        case ']':
            turtle = turtles.top();
            turtles.pop();
            break;
        }
    }

    line.set_vertices(vertices);
    line.render(view.data());

    vertices.clear();
}
