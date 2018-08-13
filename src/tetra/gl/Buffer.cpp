#include "Buffer.hpp"

#include <utility>

using std::swap;
using tetra::Buffer;

Buffer::Buffer() : id{0} { glCreateBuffers(1, &id); }
Buffer::~Buffer() { glDeleteBuffers(1, &id); }

Buffer::Buffer(Buffer&& from) : id{0} { swap(id, from.id); }

Buffer& Buffer::operator=(Buffer&& from)
{
    swap(id, from.id);
    return *this;
}

GLuint Buffer::handle() const { return id; }
