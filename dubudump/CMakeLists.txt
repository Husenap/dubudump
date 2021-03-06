set(target_name "dubudump")

set(src_dubudump
    "src/DubuDump.h"
    "src/DubuDump.cpp")

set(src_files
    ${src_dubudump}
    "src/main.cpp")

# Project
add_executable(${target_name} ${src_files})

target_link_libraries(${target_name} CONAN_PKG::fmt CONAN_PKG::argparse)

set_target_properties(${target_name} PROPERTIES FOLDER ${PROJECT_NAME})
set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

install(TARGETS ${target_name})

# Tests
add_executable(${target_name}_test "test/test.cpp")

target_link_libraries(${target_name}_test CONAN_PKG::gtest)

set_target_properties(${target_name}_test PROPERTIES FOLDER ${PROJECT_NAME})

gtest_discover_tests(${target_name}_test)