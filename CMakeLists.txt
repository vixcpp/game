# ====================================================================
#  @file CMakeLists.txt
#  @author Gaspard Kirira
#
#  Copyright 2026, Gaspard Kirira.
#  All rights reserved.
#  https://github.com/vixcpp/game
#
#  Use of this source code is governed by a MIT license
#  that can be found in the LICENSE file.
#
# ====================================================================
# Vix.cpp - Game Module
# ====================================================================

cmake_minimum_required(VERSION 3.20)

project(vix_game
  VERSION 0.1.0
  LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# --------------------------------------------------------------------
# Global settings
# --------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# --------------------------------------------------------------------
# Export set selection
# --------------------------------------------------------------------
if (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD)
  set(VIX_GAME_EXPORT_SET VixTargets)
else()
  set(VIX_GAME_EXPORT_SET vix_gameTargets)
endif()

# --------------------------------------------------------------------
# Options
# --------------------------------------------------------------------
option(VIX_GAME_FETCH_ERROR      "Auto-fetch vix::error if missing" ON)
option(VIX_GAME_FETCH_TIME       "Auto-fetch vix::time if missing" ON)
option(VIX_GAME_FETCH_FS         "Auto-fetch vix::fs if missing" ON)
option(VIX_GAME_FETCH_PATH       "Auto-fetch vix::path if missing" ON)
option(VIX_GAME_FETCH_JSON       "Auto-fetch vix::json if missing" ON)
option(VIX_GAME_FETCH_LOG        "Auto-fetch vix::log if missing" ON)
option(VIX_GAME_FETCH_THREADPOOL "Auto-fetch vix::threadpool if missing" ON)
option(VIX_GAME_FETCH_ASYNC      "Auto-fetch vix::async if missing" ON)
option(VIX_GAME_FETCH_IO "Auto-fetch vix::io if missing for examples" ON)

option(VIX_GAME_BUILD_TESTS      "Build game module tests" OFF)
option(VIX_GAME_BUILD_EXAMPLES   "Build game module examples" OFF)
option(VIX_GAME_ENABLE_SDL        "Enable SDL backend for vix/game" OFF)
option(VIX_GAME_ENABLE_SDL_OPENGL "Enable SDL OpenGL renderer backend for vix/game" OFF)
set(VIX_GAME_HAS_LOCAL_DEPENDENCIES OFF)

# --------------------------------------------------------------------
# Umbrella build policy
# --------------------------------------------------------------------
if (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD)
  set(VIX_GAME_FETCH_ERROR      OFF CACHE BOOL "Auto-fetch vix::error if missing" FORCE)
  set(VIX_GAME_FETCH_TIME       OFF CACHE BOOL "Auto-fetch vix::time if missing" FORCE)
  set(VIX_GAME_FETCH_FS         OFF CACHE BOOL "Auto-fetch vix::fs if missing" FORCE)
  set(VIX_GAME_FETCH_PATH       OFF CACHE BOOL "Auto-fetch vix::path if missing" FORCE)
  set(VIX_GAME_FETCH_JSON       OFF CACHE BOOL "Auto-fetch vix::json if missing" FORCE)
  set(VIX_GAME_FETCH_LOG        OFF CACHE BOOL "Auto-fetch vix::log if missing" FORCE)
  set(VIX_GAME_FETCH_THREADPOOL OFF CACHE BOOL "Auto-fetch vix::threadpool if missing" FORCE)
  set(VIX_GAME_FETCH_ASYNC      OFF CACHE BOOL "Auto-fetch vix::async if missing" FORCE)
  set(VIX_GAME_FETCH_IO OFF CACHE BOOL "Auto-fetch vix::io if missing for examples" FORCE)
endif()

# --------------------------------------------------------------------
# Sources discovery
# --------------------------------------------------------------------
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE GAME_SDL_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/backends/sdl/*.cpp"
)

if (GAME_SDL_SOURCES)
  list(REMOVE_ITEM GAME_SOURCES ${GAME_SDL_SOURCES})
endif()

file(GLOB_RECURSE GAME_SDL_OPENGL_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/backends/sdl/SDLOpenGLRenderer.cpp"
)

if (GAME_SDL_OPENGL_SOURCES)
  list(REMOVE_ITEM GAME_SOURCES ${GAME_SDL_OPENGL_SOURCES})
endif()

# --------------------------------------------------------------------
# Dependency resolver for game public dependencies
# --------------------------------------------------------------------
function(vix_game_resolve_dependency module_name target_name package_name fetch_option git_url git_tag)
  if (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD)
    if (NOT TARGET ${target_name})
      message(FATAL_ERROR "[game] Umbrella build: ${target_name} must be provided before game.")
    endif()

    return()
  endif()

  if (TARGET ${target_name})
    return()
  endif()

  find_package(${package_name} CONFIG QUIET)

  if (TARGET ${target_name})
    return()
  endif()

  get_filename_component(_VIX_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
  set(_VIX_DEP_DIR "${_VIX_MODULES_DIR}/${module_name}")

  if (EXISTS "${_VIX_DEP_DIR}/CMakeLists.txt")
    message(STATUS "[game] Adding local sibling ${module_name}: ${_VIX_DEP_DIR}")
    set(VIX_GAME_HAS_LOCAL_DEPENDENCIES ON PARENT_SCOPE)
    add_subdirectory("${_VIX_DEP_DIR}" "${CMAKE_BINARY_DIR}/_vix_${module_name}")
  endif()

  if (TARGET ${target_name})
    return()
  endif()

  if (${fetch_option})
    include(FetchContent)

    message(STATUS "[game] Fetching ${target_name} via FetchContent")

    FetchContent_Declare(${package_name}
      GIT_REPOSITORY ${git_url}
      GIT_TAG        ${git_tag}
    )

    FetchContent_MakeAvailable(${package_name})
  else()
    message(FATAL_ERROR
      "${target_name} not found.\n"
      "Expected one of:\n"
      "  - existing target: ${target_name}\n"
      "  - installed package: find_package(${package_name} CONFIG)\n"
      "  - local sibling module: ../${module_name}\n"
      "  - FetchContent enabled through ${fetch_option}=ON\n"
    )
  endif()

  if (NOT TARGET ${target_name})
    message(FATAL_ERROR "[game] ${target_name} not found after dependency resolution.")
  endif()
endfunction()

# --------------------------------------------------------------------
# Dependency resolver for io examples
# --------------------------------------------------------------------
function(vix_game_resolve_io_for_examples)
  if (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD)
    if (NOT TARGET vix::io)
      message(FATAL_ERROR "[game/examples] Umbrella build: vix::io must be provided before game examples.")
    endif()

    return()
  endif()

  if (TARGET vix::io)
    return()
  endif()

  get_filename_component(_VIX_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
  set(_VIX_IO_DIR "${_VIX_MODULES_DIR}/io")

  if (EXISTS "${_VIX_IO_DIR}/CMakeLists.txt")
    message(STATUS "[game/examples] Adding local sibling io: ${_VIX_IO_DIR}")
    set(VIX_GAME_HAS_LOCAL_DEPENDENCIES ON PARENT_SCOPE)
    add_subdirectory("${_VIX_IO_DIR}" "${CMAKE_BINARY_DIR}/_vix_io")
  endif()

  if (TARGET vix::io)
    return()
  endif()

  find_package(vix_io CONFIG QUIET)

  if (TARGET vix::io)
    return()
  endif()

  if (VIX_GAME_FETCH_IO)
    include(FetchContent)

    message(STATUS "[game/examples] Fetching vix::io via FetchContent")

    FetchContent_Declare(vix_io
      GIT_REPOSITORY https://github.com/vixcpp/io.git
      GIT_TAG        v0.1.0
    )

    FetchContent_MakeAvailable(vix_io)
  endif()

  if (NOT TARGET vix::io)
    message(FATAL_ERROR
      "vix::io not found for game examples.\n"
      "Examples use vix::print, so they require vix::io.\n"
      "Expected one of:\n"
      "  - existing target: vix::io\n"
      "  - local sibling module: ../io\n"
      "  - installed package: find_package(vix_io CONFIG)\n"
      "  - FetchContent enabled through VIX_GAME_FETCH_IO=ON\n"
    )
  endif()
endfunction()

# --------------------------------------------------------------------
# Required dependencies
# --------------------------------------------------------------------
vix_game_resolve_dependency(
  error
  vix::error
  vix_error
  VIX_GAME_FETCH_ERROR
  https://github.com/vixcpp/error.git
  v0.1.0
)

vix_game_resolve_dependency(
  time
  vix::time
  vix_time
  VIX_GAME_FETCH_TIME
  https://github.com/vixcpp/time.git
  v0.1.0
)

vix_game_resolve_dependency(
  fs
  vix::fs
  vix_fs
  VIX_GAME_FETCH_FS
  https://github.com/vixcpp/fs.git
  v0.1.0
)

vix_game_resolve_dependency(
  path
  vix::path
  vix_path
  VIX_GAME_FETCH_PATH
  https://github.com/vixcpp/path.git
  v0.1.0
)

vix_game_resolve_dependency(
  json
  vix::json
  vix_json
  VIX_GAME_FETCH_JSON
  https://github.com/vixcpp/json.git
  v0.1.0
)

vix_game_resolve_dependency(
  log
  vix::log
  vix_log
  VIX_GAME_FETCH_LOG
  https://github.com/vixcpp/log.git
  v0.1.0
)

vix_game_resolve_dependency(
  threadpool
  vix::threadpool
  vix_threadpool
  VIX_GAME_FETCH_THREADPOOL
  https://github.com/vixcpp/threadpool.git
  v0.1.0
)

vix_game_resolve_dependency(
  async
  vix::async
  async
  VIX_GAME_FETCH_ASYNC
  https://github.com/vixcpp/async.git
  v0.1.0
)

set(VIX_ERROR_TARGET      vix::error)
set(VIX_TIME_TARGET       vix::time)
set(VIX_FS_TARGET         vix::fs)
set(VIX_PATH_TARGET       vix::path)
set(VIX_JSON_TARGET       vix::json)
set(VIX_LOG_TARGET        vix::log)
set(VIX_THREADPOOL_TARGET vix::threadpool)
set(VIX_ASYNC_TARGET      vix::async)

# --------------------------------------------------------------------
# Library target
# --------------------------------------------------------------------
if (GAME_SOURCES)
  message(STATUS "[game] Building STATIC library with detected sources.")

  add_library(vix_game STATIC ${GAME_SOURCES})
  add_library(vix::game ALIAS vix_game)

  target_compile_features(vix_game PUBLIC cxx_std_20)

  target_include_directories(vix_game
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

   target_link_libraries(vix_game
    PUBLIC
      $<BUILD_INTERFACE:${VIX_ERROR_TARGET}>
      $<BUILD_INTERFACE:${VIX_TIME_TARGET}>
      $<BUILD_INTERFACE:${VIX_FS_TARGET}>
      $<BUILD_INTERFACE:${VIX_PATH_TARGET}>
      $<BUILD_INTERFACE:${VIX_JSON_TARGET}>
      $<BUILD_INTERFACE:${VIX_LOG_TARGET}>
      $<BUILD_INTERFACE:${VIX_THREADPOOL_TARGET}>
      $<BUILD_INTERFACE:${VIX_ASYNC_TARGET}>
      $<INSTALL_INTERFACE:vix::error>
      $<INSTALL_INTERFACE:vix::time>
      $<INSTALL_INTERFACE:vix::fs>
      $<INSTALL_INTERFACE:vix::path>
      $<INSTALL_INTERFACE:vix::json>
      $<INSTALL_INTERFACE:vix::log>
      $<INSTALL_INTERFACE:vix::threadpool>
      $<INSTALL_INTERFACE:vix::async>
  )

  if (VIX_GAME_ENABLE_SDL)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)

    target_sources(vix_game
      PRIVATE
        src/backends/sdl/SDLInputMap.cpp
        src/backends/sdl/SDLRenderer.cpp
        src/backends/sdl/SDLWindow.cpp
    )

    target_compile_definitions(vix_game
      PUBLIC
        VIX_GAME_HAS_SDL=1
    )

    if (TARGET SDL2::SDL2)
      target_link_libraries(vix_game
        PUBLIC
          SDL2::SDL2
      )
    else()
      target_include_directories(vix_game
        PUBLIC
          ${SDL2_INCLUDE_DIRS}
      )

      target_link_libraries(vix_game
        PUBLIC
          ${SDL2_LIBRARIES}
      )
    endif()

    if (TARGET SDL2_image::SDL2_image)
      target_link_libraries(vix_game
        PUBLIC
          SDL2_image::SDL2_image
      )
    else()
      target_include_directories(vix_game
        PUBLIC
          ${SDL2_IMAGE_INCLUDE_DIRS}
      )

      target_link_libraries(vix_game
        PUBLIC
          ${SDL2_IMAGE_LIBRARIES}
      )
    endif()
  endif()

  if (VIX_GAME_ENABLE_SDL_OPENGL)
    find_package(OpenGL REQUIRED)

    target_sources(vix_game
      PRIVATE
        ${GAME_SDL_OPENGL_SOURCES}
    )

    target_compile_definitions(vix_game
      PUBLIC
        VIX_GAME_HAS_SDL_OPENGL=1
    )

    target_link_libraries(vix_game
      PUBLIC
        OpenGL::GL
    )
  endif()

  if (VIX_GAME_ENABLE_SDL_OPENGL AND NOT VIX_GAME_ENABLE_SDL)
    message(FATAL_ERROR "[game] SDL OpenGL renderer requires VIX_GAME_ENABLE_SDL=ON.")
  endif()

  if (NOT MSVC)
    target_compile_options(vix_game PRIVATE
      -Wall
      -Wextra
      -Wpedantic
    )
  endif()

  if (VIX_ENABLE_SANITIZERS AND NOT MSVC)
    target_compile_options(vix_game PRIVATE
      -fno-omit-frame-pointer
      -fsanitize=address,undefined
    )

    target_link_options(vix_game PRIVATE
      -fsanitize=address,undefined
    )
  endif()

  set_target_properties(vix_game PROPERTIES
    OUTPUT_NAME vix_game
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    EXPORT_NAME game
  )

  install(TARGETS vix_game
    EXPORT ${VIX_GAME_EXPORT_SET}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

else()
  message(STATUS "[game] Building HEADER-ONLY library with no sources.")

  add_library(vix_game INTERFACE)
  add_library(vix::game ALIAS vix_game)

  target_compile_features(vix_game INTERFACE cxx_std_20)

  target_include_directories(vix_game INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  set(VIX_GAME_BUILD_DEPENDENCIES
    ${VIX_ERROR_TARGET}
    ${VIX_TIME_TARGET}
    ${VIX_FS_TARGET}
    ${VIX_PATH_TARGET}
    ${VIX_JSON_TARGET}
    ${VIX_LOG_TARGET}
    ${VIX_THREADPOOL_TARGET}
    ${VIX_ASYNC_TARGET}
  )

  target_link_libraries(vix_game
    INTERFACE
      ${VIX_GAME_BUILD_DEPENDENCIES}
  )
endif()

# --------------------------------------------------------------------
# Install headers
# --------------------------------------------------------------------
if (NOT (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD))
  install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
      PATTERN "*.hpp"
      PATTERN "*.h"
  )
endif()

# --------------------------------------------------------------------
# Standalone package config/export
# --------------------------------------------------------------------
if (NOT (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD))
  if (VIX_GAME_HAS_LOCAL_DEPENDENCIES)
    message(STATUS "[game] Local sibling dependencies detected; skipping standalone package export.")
    message(STATUS "[game] Installable package export requires installed dependency packages.")
  else()
    configure_package_config_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/cmake/vix_gameConfig.cmake.in"
      "${CMAKE_CURRENT_BINARY_DIR}/vix_gameConfig.cmake"
      INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/vix_game"
    )

    write_basic_package_version_file(
      "${CMAKE_CURRENT_BINARY_DIR}/vix_gameConfigVersion.cmake"
      VERSION ${PROJECT_VERSION}
      COMPATIBILITY SameMajorVersion
    )

    install(FILES
      "${CMAKE_CURRENT_BINARY_DIR}/vix_gameConfig.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/vix_gameConfigVersion.cmake"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/vix_game"
    )

    install(EXPORT vix_gameTargets
      FILE vix_gameTargets.cmake
      NAMESPACE vix::
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/vix_game"
    )
  endif()
endif()
# --------------------------------------------------------------------
# Tests
# --------------------------------------------------------------------
if (VIX_GAME_BUILD_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()

# --------------------------------------------------------------------
# Examples
# --------------------------------------------------------------------
if (VIX_GAME_BUILD_EXAMPLES)
  vix_game_resolve_io_for_examples()
  set(VIX_IO_TARGET vix::io)
  add_subdirectory(examples)
endif()

# --------------------------------------------------------------------
# Summary
# --------------------------------------------------------------------
message(STATUS "------------------------------------------------------")
message(STATUS "vix::game configured (${PROJECT_VERSION})")

if (GAME_SOURCES)
  message(STATUS "Mode: STATIC / sources found")
else()
  message(STATUS "Mode: HEADER-ONLY / no sources")
endif()

message(STATUS "Export set:        ${VIX_GAME_EXPORT_SET}")
message(STATUS "Error target:      ${VIX_ERROR_TARGET}")
message(STATUS "Time target:       ${VIX_TIME_TARGET}")
message(STATUS "Fs target:         ${VIX_FS_TARGET}")
message(STATUS "Path target:       ${VIX_PATH_TARGET}")
message(STATUS "Json target:       ${VIX_JSON_TARGET}")
message(STATUS "Log target:        ${VIX_LOG_TARGET}")
message(STATUS "Threadpool target: ${VIX_THREADPOOL_TARGET}")
message(STATUS "Async target:      ${VIX_ASYNC_TARGET}")

if (VIX_GAME_BUILD_EXAMPLES)
  message(STATUS "IO target:         ${VIX_IO_TARGET}")
endif()

message(STATUS "Tests:             ${VIX_GAME_BUILD_TESTS}")
message(STATUS "Examples:          ${VIX_GAME_BUILD_EXAMPLES}")
message(STATUS "SDL backend:       ${VIX_GAME_ENABLE_SDL}")
message(STATUS "SDL OpenGL:        ${VIX_GAME_ENABLE_SDL_OPENGL}")
message(STATUS "Include dir:       ${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "Build type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "------------------------------------------------------")
