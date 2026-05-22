# ====================================================================
# Vix.cpp - Game Module Examples
# ====================================================================

if (NOT TARGET vix::io)
  message(FATAL_ERROR "[game/examples] vix::io is required because examples use vix::print.")
endif()

set(VIX_GAME_EXAMPLES
  hello_game
  basic_loop
  scene_demo
  asset_loading
  jobs_demo
  input_demo
  null_window
  null_renderer
  render2d_demo
  async_asset_loading
  scene_serialization
  runtime_v3_demo
  export_project
)

set(VIX_GAME_SDL_EXAMPLES
  sdl_window_null_renderer_demo
  window_input_demo
  sdl_renderer_demo
  sprite_demo
)

if (VIX_GAME_ENABLE_SDL)
  list(APPEND VIX_GAME_EXAMPLES ${VIX_GAME_SDL_EXAMPLES})
endif()

foreach(example_name ${VIX_GAME_EXAMPLES})
  set(example_source "${CMAKE_CURRENT_SOURCE_DIR}/${example_name}.cpp")
  set(example_target "vix_game_${example_name}")

  if (NOT EXISTS "${example_source}")
    message(FATAL_ERROR "[game/examples] Example source not found: ${example_source}")
  endif()

  add_executable("${example_target}"
    "${example_source}"
  )

  target_compile_features("${example_target}"
    PRIVATE
      cxx_std_20
  )

  target_link_libraries("${example_target}"
    PRIVATE
      vix::game
      vix::io
  )

  if (NOT MSVC)
    target_compile_options("${example_target}"
      PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
  endif()
endforeach()

if (NOT VIX_GAME_ENABLE_SDL)
  message(STATUS "[game/examples] SDL examples disabled. Enable with -DVIX_GAME_ENABLE_SDL=ON.")
endif()
