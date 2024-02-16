file(GLOB_RECURSE source
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.h
)


add_executable(${UNIT_NAME}Target ${source})