#include <tetra/gl/Framebuffer.hpp>

#include <utility>

using namespace tetra;

Framebuffer::Framebuffer() { glGenFramebuffers(1, &id); }

Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &id); }

Framebuffer::Framebuffer(Framebuffer&& from) : id{0} { std::swap(id, from.id); }

GLuint Framebuffer::handle() const { return id; }

Framebuffer& Framebuffer::operator=(Framebuffer&& from)
{
    std::swap(id, from.id);
    return *this;
}
