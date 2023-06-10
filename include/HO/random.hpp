#pragma once

#include <cstdint>

using std::int64_t;
using std::uint64_t;

namespace HO
{
namespace Random
{
int64_t Int64(int64_t start, int64_t end);
int32_t Int32(int32_t start, int32_t end);
float Float(float start, float end);
double Double(double start, double end);
}; // namespace Random
}; // namespace HO