enable_testing()

# Download GTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TEST_TARGET "${PROJECT_NAME}Tests")

add_executable(${TEST_TARGET} main.cpp)
target_link_libraries(${TEST_TARGET} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${TEST_TARGET})

add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})

add_custom_target(
  run_tests ALL
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  DEPENDS ${TEST_TARGET})
