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

set(PRESTO_TEST_TARGET "${PROJECT_NAME}Tests")

add_executable(${PRESTO_TEST_TARGET} main.cpp pch.cpp)

target_precompile_headers(${PRESTO_TEST_TARGET} PRIVATE pch.h)
target_link_libraries(${PRESTO_TEST_TARGET} PRIVATE ${PRESTO_ENGINE_NAME}
                                                    GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PRESTO_TEST_TARGET})

add_test(NAME ${PRESTO_TEST_TARGET} COMMAND ${PRESTO_TEST_TARGET})

add_custom_target(
  run_tests ALL
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  DEPENDS ${PRESTO_TEST_TARGET})
