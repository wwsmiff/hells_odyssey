#include <HO/HO.hpp>
#include <SDL.h>
#include <cstdint>
#include <gvdi/gvdi.hpp>

int main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) !=
      0)
  {
    std::cout << "Failed to initialize SDL2, " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_DisplayMode primaryDisplay{};
  SDL_GetDesktopDisplayMode(0, &primaryDisplay);

  constexpr int32_t camera_width_v{800};
  // const int32_t camera_height_v{primaryDisplay.h};

  HO::Window mainWindow{"Hell's Odyssey",
                        HO::Vec2<int32_t>{primaryDisplay.w, primaryDisplay.h},
                        SDL_WINDOW_FULLSCREEN_DESKTOP};
  bool running{true};

  SDL_Rect gameBackground{0, 0, camera_width_v, 0};

  gameBackground.h = primaryDisplay.h;
  gameBackground.x =
      static_cast<int32_t>((primaryDisplay.w / 2) - (gameBackground.w / 2));

  HO::Vec2<uint32_t> gameBoundsHorizontal{
      static_cast<uint32_t>(gameBackground.x),
      static_cast<uint32_t>(gameBackground.x + gameBackground.w)};
  HO::Vec2<uint32_t> gameBoundsVertical{
      static_cast<uint32_t>(gameBackground.y),
      static_cast<uint32_t>(gameBackground.y + gameBackground.h)};

  // auto fromLeft = [=](float n) { return gameBoundsHorizontal.x + n; };
  // auto fromRight = [=](float n) { return gameBoundsHorizontal.y - n; };
  // auto fromTop = [=](float n) { return gameBoundsVertical.x + n; };
  auto fromBottom = [=](float n) { return gameBoundsVertical.y - n; };

  HO::Player player{
      HO::Vec2<float>{
          static_cast<float>(((gameBoundsHorizontal.x + (camera_width_v / 2)) -
                              (HO::Config::playerBlocksize / 2))),
          static_cast<float>(fromBottom(HO::Config::playerBlocksize + 50))},
      HO::Vec2<float>{HO::Config::playerBlocksize,
                      HO::Config::playerBlocksize}};
  player.setColor(HO::Rgba{0x2c'ee'2c'ff});

  player.setHitbox(
      HO::Vec2<float>{HO::Config::playerHitbox, HO::Config::playerHitbox});
  player.setSize(HO::Vec2<float>{HO::Config::playerBlocksize,
                                 HO::Config::playerBlocksize});

  player.setBounds(gameBoundsHorizontal, gameBoundsVertical);

  player.loadTexture(mainWindow.getRenderer(),
                     "../assets/sprites/player_ship.png");

  gvdi::Instance instance{};
  if constexpr (enable_debug_v)
    instance = gvdi::Instance{{500, 600}, "Tweaks"};

  HO::InputManager windowInput{};

  auto elapsedTime{0};

  // auto start{std::chrono::steady_clock::now()};
  auto start{SDL_GetTicks()};
  auto frame{0};
  auto fps{0};
  while (running)
  {
    // auto current{std::chrono::steady_clock::now()};
    // auto delta{std::chrono::duration<float, std::milli>(current - start)};

    auto current{SDL_GetTicks()};
    auto delta{current - start};

    windowInput.beginNewFrame();
    if (windowInput.eventOccurred(SDL_QUIT))
      running = false;

    player.handleEvents(windowInput);

    if constexpr (enable_debug_v)
    {
      player.setHitbox(
          HO::Vec2<float>{HO::Config::playerHitbox, HO::Config::playerHitbox});
      player.setSize(HO::Vec2<float>{HO::Config::playerBlocksize,
                                     HO::Config::playerBlocksize});
    }

    player.update(delta);

    mainWindow.clear(HO::Rgba{0x00'00'00'ff});

    SDL_SetRenderDrawColor(mainWindow.getRenderer(), 16, 16, 16, 255);
    SDL_RenderFillRect(mainWindow.getRenderer(), &gameBackground);

    player.render(mainWindow.getRenderer());

    mainWindow.render();

    if constexpr (enable_debug_v)
    {
      gvdi::Frame frame{instance};
      ImGui::SetNextWindowPos({0, 0});
      ImGui::SetNextWindowSize({500, 600});
      ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_NoTitleBar);
      ImGui::Checkbox("Debug View", &HO::Config::debugView);

      ImGui::SliderFloat("Player velocity", &HO::Config::playerVelocity, 0.0f,
                         50.0f);

      int32_t playerBlocksizeInt{
          static_cast<int32_t>(HO::Config::playerBlocksize)};
      ImGui::SliderInt("Block size", &playerBlocksizeInt, 0, 1024);
      HO::Config::playerBlocksize = static_cast<uint32_t>(playerBlocksizeInt);

      int32_t playerHitboxInt{static_cast<int32_t>(HO::Config::playerHitbox)};
      ImGui::SliderInt("Hitbox size", &playerHitboxInt, 0, 1024);
      HO::Config::playerHitbox = static_cast<uint32_t>(playerHitboxInt);
      ImGui::End();
    }

    elapsedTime += delta;
    frame++;
    if (elapsedTime > 1000)
    {
      fps = frame;
      elapsedTime = 0;
      frame = 0;
    }
    // std::cout << fps << std::endl;
    start = current;
  }

  return EXIT_SUCCESS;
}