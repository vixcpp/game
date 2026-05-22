# ====================================================================
# Vix.cpp - Game Module Tests
# ====================================================================

find_package(GTest CONFIG QUIET)

if (NOT TARGET GTest::gtest OR NOT TARGET GTest::gtest_main)
  include(FetchContent)

  message(STATUS "[game/tests] Fetching GoogleTest")

  FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
  )

  FetchContent_MakeAvailable(googletest)
endif()

set(VIX_GAME_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/AppTests.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/AssetManagerTests.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/EventBusTests.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/GameLoopTests.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/JobSystemTests.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/RegistryTests.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/SceneManagerTests.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/TimeStepTests.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/runtime_v4_architecture_tests.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/headless_runtime_tests.cpp"
)

add_executable(vix_game_tests
  ${VIX_GAME_TEST_SOURCES}
)

target_compile_features(vix_game_tests
  PRIVATE
    cxx_std_20
)

target_link_libraries(vix_game_tests
  PRIVATE
    vix::game
    GTest::gtest
    GTest::gtest_main
)

if (NOT MSVC)
  target_compile_options(vix_game_tests
    PRIVATE
      -Wall
      -Wextra
      -Wpedantic
  )
endif()

include(GoogleTest)
gtest_discover_tests(vix_game_tests)
