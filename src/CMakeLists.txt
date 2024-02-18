
file(GLOB_RECURSE source
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.h
)


add_library(${UNIT_NAME}Target ${source})
target_include_directories(${UNIT_NAME}Target INTERFACE ${PROJECT_SOURCE_DIR}/src)