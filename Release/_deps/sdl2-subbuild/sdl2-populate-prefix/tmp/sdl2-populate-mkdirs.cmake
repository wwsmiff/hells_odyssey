# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/arvk/Code/hells_odyssey/Release/_deps/sdl2-src"
  "/home/arvk/Code/hells_odyssey/Release/_deps/sdl2-build"
  "/home/arvk/Code/hells_odyssey/Release/_deps/sdl2-subbuild/sdl2-populate-prefix"
  "/home/arvk/Code/hells_odyssey/Release/_deps/sdl2-subbuild/sdl2-populate-prefix/tmp"
  "/home/arvk/Code/hells_odyssey/Release/_deps/sdl2-subbuild/sdl2-populate-prefix/src/sdl2-populate-stamp"
  "/home/arvk/Code/hells_odyssey/Release/_deps/sdl2-subbuild/sdl2-populate-prefix/src"
  "/home/arvk/Code/hells_odyssey/Release/_deps/sdl2-subbuild/sdl2-populate-prefix/src/sdl2-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/arvk/Code/hells_odyssey/Release/_deps/sdl2-subbuild/sdl2-populate-prefix/src/sdl2-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/arvk/Code/hells_odyssey/Release/_deps/sdl2-subbuild/sdl2-populate-prefix/src/sdl2-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
