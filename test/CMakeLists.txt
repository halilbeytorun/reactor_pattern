

file(GLOB_RECURSE test_source
    ${PROJECT_SOURCE_DIR}/test/*.cpp
    ${PROJECT_SOURCE_DIR}/test/*.h
)



include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(${UNIT_NAME}TestTarget ${test_source})


target_link_libraries(
${UNIT_NAME}TestTarget
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${UNIT_NAME}TestTarget)



