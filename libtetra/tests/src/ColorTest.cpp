#include <tetra/Color.hpp>

#include "catch.hpp"

using namespace tetra;
using namespace std;

namespace
{
using col = array<int, 4>;
col as_ints(array<float, 4> raw)
{
    return {
        (int)round(raw[0] * 255),
        (int)round(raw[1] * 255),
        (int)round(raw[2] * 255),
        (int)round(raw[3] * 255),
    };
}
} // namespace

SCENARIO("Converting HSLA Colors to RGBA")
{
    GIVEN("Given full alpha colors")
    {
        auto black = HSL{0, 0, 0}.as_rgba();
        REQUIRE(as_ints(black) == col{0, 0, 0, 255});

        auto white = HSL{0, 0, 1}.as_rgba();
        REQUIRE(as_ints(white) == col{255, 255, 255, 255});

        auto red = HSL{}.as_rgba();
        REQUIRE(as_ints(red) == col{255, 0, 0, 255});

        auto lime = HSL{120}.as_rgba();
        REQUIRE(as_ints(lime) == col{0, 255, 0, 255});

        auto blue = HSL{240}.as_rgba();
        REQUIRE(as_ints(blue) == col{0, 0, 255, 255});

        auto yellow = HSL{60}.as_rgba();
        REQUIRE(as_ints(yellow) == col{255, 255, 0, 255});

        auto cyan = HSL{180}.as_rgba();
        REQUIRE(as_ints(cyan) == col{0, 255, 255, 255});

        auto magenta = HSL{300}.as_rgba();
        REQUIRE(as_ints(magenta) == col{255, 0, 255, 255});

        auto silver = HSL{0, 0, 0.751f}.as_rgba();
        REQUIRE(as_ints(silver) == col{192, 192, 192, 255});

        auto gray = HSL{0, 0, 0.5f}.as_rgba();
        REQUIRE(as_ints(gray) == col{128, 128, 128, 255});

        auto maroon = HSL{0, 1, 0.25f}.as_rgba();
        REQUIRE(as_ints(maroon) == col{128, 0, 0, 255});

        auto olive = HSL{60, 1, 0.25f}.as_rgba();
        REQUIRE(as_ints(olive) == col{128, 128, 0, 255});

        auto green = HSL{120, 1, 0.25f}.as_rgba();
        REQUIRE(as_ints(green) == col{0, 128, 0, 255});

        auto purple = HSL{300, 1, 0.25f}.as_rgba();
        REQUIRE(as_ints(purple) == col{128, 0, 128, 255});

        auto teal = HSL{180, 1, 0.25f}.as_rgba();
        REQUIRE(as_ints(teal) == col{0, 128, 128, 255});

        auto navy = HSL{240, 1, 0.25f}.as_rgba();
        REQUIRE(as_ints(navy) == col{0, 0, 128, 255});
    }
}
