#ifndef TETRA_GL_VAO_HPP
#define TETRA_GL_VAO_HPP

#include <GL/glew.h>

#include <array>

namespace tetra
{
class VAO
{
  public:
    VAO();
    ~VAO();
    VAO(VAO& from) = delete;
    VAO(VAO&& from);

    VAO& operator=(VAO&& from);

    GLuint handle() const;

    template<typename F>
    void whileBound(F f)
    {
        glBindVertexArray(handle());
        f();
        glBindVertexArray(0);
    }

  private:
    GLuint id;
};

/**
 * Bind an attribute index to an offset defined by a member of a struct.
 * Calls glEnableVertexAttribArray automatically.
 * It only supports POD structs and attributes which are std::array's.
 * The pointer will be set as an offset into whatever is currently bound
 * to the GL_ARRAY_BUFFER slot (standard behavior for glVertexAttribPointer).
 *
 * # Example usage:
 *    struct Vertex {
 *      std::array<float, 2> position;
 *      std::array<float, 4> color;
 *    };
 *
 *    vao.whileBound([&]() {
 *        glBindBuffer(GL_ARRAY_BUFFER, myvertexbuffer);
 *        attribPointer(myPositionIndex, &Vertex::position);
 *        attribPointer(myColorIndex, &Vertex::position);
 *    });
 */
template<std::array<float, 1>::size_type length, typename T>
void attribPointer(GLint index, std::array<float, length> T::*memberPtr)
{
    static_assert(
        std::is_pod<T>::value,
        "only POD data types can be used for vertex attrib pointers");

    glEnableVertexAttribArray(index);

    // use this null pointer to get the offset of the member from 0
    // it's a hack that only works for POD, so notice the static_assert
    static T* nullObj = nullptr;
    glVertexAttribPointer(
        index,
        length,
        GL_FLOAT,
        GL_FALSE,
        sizeof(T),
        static_cast<const GLvoid*>(&(nullObj->*memberPtr)));
}
} // namespace tetra

#endif
