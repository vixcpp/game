# ====================================================================
# Vix.cpp - Game Module Examples
# ====================================================================

if (NOT TARGET vix::io)
  message(FATAL_ERROR "[game/examples] vix::io is required because examples use vix::print.")
endif()

set(VIX_GAME_EXAMPLES
  asset_loading
  basic_loop
  hello_game
  jobs_demo
  scene_demo
)

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
