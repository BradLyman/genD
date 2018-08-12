#include <glad/glad.h>

#include "GlApp.hpp"

using namespace tetra;

GlApp::GlApp() { glClearColor(0.2f, 0.2f, 0.4f, 1.0f); }
GlApp::~GlApp() {}

void GlApp::render_frame() { glClear(GL_COLOR_BUFFER_BIT); }
