#include <tetra/Color.hpp>

#include <cmath>

using namespace tetra;
using namespace std;

namespace
{
array<float, 3> raw_rgb(float chroma, float hue_norm)
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

array<float, 3> hsl_to_rgb(float h, float s, float l)
{
    const float chroma = (1.0f - abs((2.0f * l) - 1.0f)) * s;
    const auto rgb = raw_rgb(chroma, h / 60.0f);
    const float match = l - (chroma / 2.0f);
    return {rgb[0] + match, rgb[1] + match, rgb[2] + match};
}
} // namespace

HSL::HSL(float h, float s, float l, float a) : h{h}, s{s}, l{l}, a{a} {}

array<float, 4> HSL::as_rgba()
{
    const auto rgb = hsl_to_rgb(h, s, l);
    return {rgb[0], rgb[1], rgb[2], a};
}
