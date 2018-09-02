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

constexpr float mixf(float x, float y, float v)
{
    return x * (1.0f - v) + y * v;
}

float mod360(float value)
{
    const float modded = fmod(value, 360.0f);
    if (modded < 0) {
        return 360.0f + modded;
    } else {
        return modded;
    }
}

float lerp360(float x, float y, float v)
{
    float d = abs(x - y);
    if (d > 180) {
        d = 360 - d;
    }
    if ((x - y) < 0) {
        d = -d;
    }
    return x + (v * d);
}

} // namespace

HSL::HSL(float h, float s, float l, float a) : h{mod360(h)}, s{s}, l{l}, a{a} {}

array<float, 4> HSL::as_rgba()
{
    const auto rgb = hsl_to_rgb(h, s, l);
    return {rgb[0], rgb[1], rgb[2], a};
}

HSL HSL::mix(const HSL& x, const HSL& y, float v)
{
    return {
        lerp360(x.h, y.h, v),
        mixf(x.s, y.s, v),
        mixf(x.l, y.l, v),
        mixf(x.a, y.a, v),
    };
}

RGBA::RGBA(float i, float a) : r{i}, g{i}, b{i}, a{a} {}
RGBA::RGBA(float r, float g, float b, float a) : r{r}, g{g}, b{b}, a{a} {}

array<float, 4> RGBA::as_rgba() noexcept { return {r, g, b, a}; }

RGBA RGBA::mix(const RGBA& left, const RGBA& right, float v)
{
    return {
        mixf(left.r, right.r, v),
        mixf(left.g, right.g, v),
        mixf(left.b, right.b, v),
        mixf(left.a, right.a, v),
    };
}
