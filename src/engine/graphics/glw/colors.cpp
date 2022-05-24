#include "glw/color.h"

namespace OGL {
    Color Color::operator+(const Color& o) const {
        Color c = {r + o.r, g + o.g, b + o.b,  a + o.a};
        return c;
    }
    Color& Color::operator+=(const Color& o) {
        r += o.r;
        g += o.g;
        b += o.b;
        a += o.a;

        return *this;
    }
    Color& Color::operator=(const Color& o) {
        r = o.r;
        g = o.g;
        b = o.b;
        a = o.a;

        return *this;
    }
    Color Color::operator*(const float o) const {
        Color c = { r * o, g * o, b * o,  a * o };
        return c;
    }
    Color& Color::operator*=(const float o) {
        r *= o;
        g *= o;
        b *= o; 
        a *= o;

        return *this;
    }
    bool Color::operator==(const Color& o) const {
        return (r == o.r &&
            g == o.g &&
            b == o.b &&
            a == o.a);
    }

    const Color Color::White = { 1.0f, 1.0f, 1.0f, 1.0f };
    const Color Color::WhiteSmoke = { 0.96f, 0.96f, 0.96f, 1.0f };
    const Color Color::Gainsboro = { 0.80f, 0.80f, 0.80f, 1.0f };
    const Color Color::LightGray = { 0.76f, 0.76f, 0.76f, 1.0f };
    const Color Color::Silver = { 0.69f, 0.69f, 0.69f, 1.0f };;
    const Color Color::DarkGray = { 0.60f, 0.60f, 0.60f, 1.0f };
    const Color Color::Gray = { 0.50f, 0.50f, 0.50f, 1.0f };
    const Color Color::DimGray = { 0.41f, 0.41f, 0.41f, 1.0f };
    const Color Color::DimDimGray = { 0.21f, 0.21f, 0.21f, 1.0f };
    const Color Color::Black = { 0.0f, 0.0f, 0.0f, 1.0f };
    const Color Color::Red = { 1.0f, 0.0f, 0.0f, 1.0f };
    const Color Color::Orange = { 1.f, 0.647f, 0.0f , 1.0f };
    const Color Color::Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
    const Color Color::FireBrick = { 0.64f, 0.13f, 0.13f, 1.0f };
    const Color Color::Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
    const Color Color::Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
    const Color Color::Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
    const Color Color::Green = { 0.0f, 0.50, 0.0f, 1.0f };
    const Color Color::Lime = { 0.0f, 1.0f, 0.0f, 1.0f };
    const Color Color::DarkGreen = { 0.0f, 0.39f, 0.0f, 1.0f };
    const Color Color::Indigo = { 0.29f, 0.0f, 0.51f, 1.0f };
    const Color Color::DarkViolet = { 0.58f, 0.0f, 0.76f, 1.0f };
    const Color Color::Purple = { 0.50f, 0.0f, 0.50f, 1.0f };
    const Color Color::SkyBlue = { 0.00f, 0.75f, 1.0f, 1.0f };
}