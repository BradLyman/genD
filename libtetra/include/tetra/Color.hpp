#ifndef TETRA_COLOR_HPP
#define TETRA_COLOR_HPP

#include <array>

namespace tetra
{
/**
 * Colors are things which can be represented as an rgba array of floats.
 */
class IColor
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
class HSL final : public IColor
{
  public:
    HSL(float h = 0.0f, float s = 1.0f, float l = 0.5f, float a = 1.0f);
    HSL(const HSL& from) = default;

    /**
     * The color as an array of rgba values where each component is in [0, 1].
     */
    virtual std::array<float, 4> as_rgba() override;

    /**
     * Mix two hsl colors with the function x*(1.0f-v) + y*v.
     */
    static HSL mix(const HSL& x, const HSL& y, float v);

  private:
    float h, s, l, a;
};

/**
 * Represents an RGBA color.
 */
class RGBA : public IColor
{
  public:
    RGBA(float i, float a);
    RGBA(float r, float g, float b, float a);

    virtual std::array<float, 4> as_rgba() noexcept override;

    static RGBA mix(const RGBA& left, const RGBA& right, float v);

  private:
    float r, g, b, a;
};

} // namespace tetra

#endif
