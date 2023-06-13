cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(Hells_Odyssey)
include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS FALSE)
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG ac13ca9
)

include(FetchContent)
FetchContent_MakeAvailable(SDL2)
FetchContent_GetProperties(SDL2
  SOURCE_DIRECTORY SDL2_SOURCE_DIR
)
FetchContent_Declare(
  SDL2_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
  GIT_TAG d3c6d59
)
set(SDL2IMAGE_INSTALL OFF)
set(SDL2_MAIN_LIBRARY ON)
FetchContent_MakeAvailable(SDL2_image)
FetchContent_GetProperties(SDL2
  SOURCE_DIRECTORY SDL2_IMAGE_SOURCE_DIR
)


FetchContent_Declare(
  gvdi
  GIT_REPOSITORY https://github.com/karnkaul/gvdi
  GIT_TAG 5566d28
)
FetchContent_MakeAvailable(gvdi)

add_executable(${PROJECT_NAME})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL2_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL2_IMAGE_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static gvdi::gvdi)
target_sources(${PROJECT_NAME} PRIVATE 
 src/main.cpp
 src/window.cpp
 src/entity.cpp
 src/texture.cpp
 src/deleters.cpp
 src/player.cpp
 src/input_manager.cpp
 src/bullet.cpp
 src/random.cpp
 src/particles.cpp
)
