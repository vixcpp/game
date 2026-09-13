cmake_minimum_required(VERSION 3.20)

project(vix_example_game LANGUAGES CXX)

if (NOT TARGET vix::game)
  find_package(Vix CONFIG REQUIRED)
endif()

# SDL/OpenGL examples require backends intentionally disabled in the installed SDK CI.
set(_vix_game_examples
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
  tiny_adventure_reference
)

set(_vix_game_print_examples
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

foreach (_example IN LISTS _vix_game_examples)
  add_executable("game_${_example}" "${_example}.cpp")
  target_compile_features("game_${_example}" PRIVATE cxx_std_20)
  target_link_libraries("game_${_example}" PRIVATE vix::game)

  if (_example IN_LIST _vix_game_print_examples)
    target_link_libraries("game_${_example}" PRIVATE vix::io)
  endif()
endforeach()

# Only headless, deterministic examples are exercised automatically.
enable_testing()
foreach (_example IN ITEMS
    hello_game
    input_demo
    null_window
    null_renderer
    render2d_demo
    asset_loading
    async_asset_loading
    scene_serialization
    export_project
    tiny_adventure_reference)
  add_test(NAME "game.${_example}" COMMAND "game_${_example}")
  set_tests_properties("game.${_example}" PROPERTIES
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endforeach()
