#include "VAO.hpp"

#include <utility>

using namespace tetra;
using namespace std;

VAO::VAO() : id{0} { glCreateVertexArrays(1, &id); }
VAO::~VAO() { glDeleteVertexArrays(1, &id); }

VAO::VAO(VAO&& from) : id{0} { swap(id, from.id); }

VAO& VAO::operator=(VAO&& from)
{
    swap(id, from.id);
    return *this;
}

GLuint VAO::handle() const { return id; }
