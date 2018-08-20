#include <tetra/gl/Texture.hpp>

#include <utility>

using namespace tetra;

Texture::Texture() { glGenTextures(1, &id); }

Texture::~Texture() { glDeleteTextures(1, &id); }

Texture::Texture(Texture&& from) : id{0} { std::swap(id, from.id); }

Texture& Texture::operator=(Texture&& from)
{
    std::swap(id, from.id);
    return *this;
}

GLuint Texture::handle() const { return id; }
