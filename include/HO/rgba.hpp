#pragma once

#include <SDL.h>
#include <cstdint>

using std::uint32_t;
using std::uint8_t;

namespace HO
{
struct Rgba
{
  Rgba() : r(0), g(0), b(0), a(255) {}
  constexpr Rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
      : r(r), g(g), b(b), a(a)
  {
  }
  Rgba(uint32_t hex)
      : r(static_cast<uint8_t>((hex & 0xFF'00'00'00) >> 24)),
        g(static_cast<uint8_t>((hex & 0x00'FF'00'00) >> 16)),
        b(static_cast<uint8_t>((hex & 0x00'00'FF'00) >> 8)),
        a(static_cast<uint8_t>(hex & 0x00'00'00'FF))
  {
  }

  constexpr uint32_t hex() const
  {
    return ((this->r << 24) | (this->g << 16) | (this->b << 8) | (this->a));
  }
  uint8_t r{}, g{}, b{}, a{};
};
}; // namespace HO
