cmake_minimum_required(VERSION 3.14)
project(nyu-appsec-unit1 C)

set(CMAKE_C_STANDARD 99)

#From: https://stackoverflow.com/questions/31356199/multiple-make-targets-in-the-same-cmake-project
set(COMMON_SOURCES spell.c dictionary.c)
set(TARGET_SOURCES main.c)
set(TEST_SOURCES  test_main.c)
add_executable(spell_check ${COMMON_SOURCES} ${TARGET_SOURCES})
add_executable(spell_check_tests ${COMMON_SOURCES} ${TEST_SOURCES})
