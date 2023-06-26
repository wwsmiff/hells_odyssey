#include "HO/editor.hpp"
#include "HO/config.hpp"
#include <tweaky/value.hpp>

namespace HO
{

Editor::Editor(Vec2<float> size) : mSize{size}
{
  auto map = tweaky::db::Map{
      {"Player size",
       tweaky::FloatData{.value = 160.0f,
                         .range = {.min = 32.0f, .max = 512.0f}}},
      {"Player hitbox",
       tweaky::FloatData{.value = 20.0f,
                         .range = {.min = 10.0f, .max = 512.0f}}},
      {"Player velocity",
       tweaky::FloatData{.value = 0.75f, .range = {.min = 0.0f, .max = 10.0f}}},
      {"Bullet velocity",
       tweaky::FloatData{.value = 1.3f, .range = {.min = 0.0f, .max = 15.0f}}},
      {"Bullet width",
       tweaky::FloatData{.value = 15.0f,
                         .range = {.min = 10.0f, .max = 100.0f}}},

      {"Bullet height",
       tweaky::FloatData{.value = 55.0f,
                         .range = {.min = 20.0f, .max = 100.0f}}},

      {"Debug view", tweaky::BoolData{.value = false}}};

  tweaky::db::overwrite(std::move(map));
  tweaky::db::load_or_create("../config.json");

  if constexpr (enable_debug_v)
  {
    this->mInstance = gvdi::Instance{{size.x, size.y}, "Editor"};
  }
}

void Editor::inspect()
{
  if constexpr (enable_debug_v)
  {
    struct Inspector : tweaky::Inspector
    {
      bool inspect(tweaky::Name const &name, tweaky::IntData &out) final
      {
        return ImGui::SliderInt(name.label().data(), &out.value, out.range.min,
                                out.range.max);
      }

      bool inspect(tweaky::Name const &name, tweaky::FloatData &out) final
      {
        return ImGui::SliderFloat(name.label().data(), &out.value,
                                  out.range.min, out.range.max);
      }

      bool inspect(tweaky::Name const &name, tweaky::BoolData &out) final
      {
        return ImGui::Checkbox(name.label().data(), &out.value);
      }
    };

    gvdi::Frame frame{this->mInstance};

    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({this->mSize.x, this->mSize.y});
    if (ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoTitleBar))
    {
      auto inspector = Inspector{};
      tweaky::db::inspect(inspector);
    }

    ImGui::End();
  }
}

Editor::~Editor() { tweaky::db::save(); }

}; // namespace HO