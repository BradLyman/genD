#ifndef MAT4_HPP
#define MAT4_HPP

#include <array>

namespace tetra
{
inline std::array<float, 16> identity()
{
    // clang-format off
    return {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    // clang-format on
}

inline std::array<float, 16>
ortho(int screenWidth, int screenHeight, float height)
{
    float aspect = (float)screenWidth / screenHeight;
    float vh = height;
    float vw = vh * aspect;
    // clang-format off
    return {
        2.0f/vw, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f/vh, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    // clang-format on
};
} // namespace tetra

#endif
