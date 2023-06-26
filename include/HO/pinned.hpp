#pragma once

namespace HO
{
struct Pinned
{
  Pinned() = default;

  Pinned(Pinned &&) = delete;
  Pinned &operator=(Pinned &&) = delete;

  Pinned(Pinned const &) = delete;
  Pinned &operator=(Pinned const &) = delete;
};
};