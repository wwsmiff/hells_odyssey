#pragma once

#include "HO/pinned.hpp"
#include "HO/vec2.hpp"
#include <gvdi/gvdi.hpp>
#include <tweaky/value.hpp>

inline constexpr bool editor_v{true};

namespace HO
{
class Editor : public Pinned
{
public:
  Editor(Vec2<float> size);
  ~Editor();
  void inspect();

protected:
  gvdi::Instance mInstance{};
  Vec2<float> mSize{};
};
}; // namespace HO