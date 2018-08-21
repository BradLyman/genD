#ifndef TETRA_COLOR_HPP
#define TETRA_COLOR_HPP

#include <array>

namespace tetra
{
/**
 * Colors are things which can be represented as an rgba array of floats.
 */
class Color
{
  public:
    /**
     * The color as an array of rgba values where each coponent is in [0, 1].
     */
    virtual std::array<float, 4> as_rgba() = 0;
};

/**
 * Represents a color using the Hue-Saturation-Lightness model.
 * https://en.wikipedia.org/wiki/HSL_and_HSV#Converting_to_RGB
 */
class HSL final : public Color
{
  public:
    HSL(float h = 0.0f, float s = 1.0f, float l = 0.5f, float a = 1.0f);
    HSL(const HSL& from) = default;

    /**
     * The color as an array of rgba values where each component is in [0, 1].
     */
    virtual std::array<float, 4> as_rgba() override;

  private:
    float h, s, l, a;
};
} // namespace tetra

#endif
