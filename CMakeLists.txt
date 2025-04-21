cmake_minimum_required(VERSION 3.10)
project(TimetableSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1) collect *all* your .cpp files
file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/src/*.cpp")

# 2) remove both mains from the core library
list(REMOVE_ITEM ALL_SOURCES
    "${CMAKE_SOURCE_DIR}/src/tests.cpp"
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

# 3) build a static core library of your implementation files
add_library(core STATIC
    ${ALL_SOURCES}
  "src/LectureManagementMenu.cpp"  "src/LecturerManager.cpp")
target_include_directories(core PUBLIC "${CMAKE_SOURCE_DIR}/src")

# 4) your application executable (uses main.cpp)
add_executable(timetable
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
  "src/LectureManagementMenu.cpp"  "src/LecturerManager.cpp")
target_link_libraries(timetable PRIVATE core)

# 5) your test executable (uses tests.cpp)
add_executable(run_tests
    "${CMAKE_SOURCE_DIR}/src/tests.cpp"
  "src/LectureManagementMenu.cpp"  "src/LecturerManager.cpp")
target_link_libraries(run_tests PRIVATE core)

option(ENABLE_COVERAGE "Build with gcov/coverage" OFF)

option(ENABLE_COVERAGE "Build with code‑coverage flags" OFF)

if(ENABLE_COVERAGE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Enabling Clang Coverage Instrumentation")
    # produce .profraw files
    add_compile_options(-O0 -g -fprofile-instr-generate -fcoverage-mapping)
    add_link_options   (-fprofile-instr-generate)
  else()
    message(FATAL_ERROR "Coverage only supported with Clang in this profile")
  endif()
endif()

enable_testing()
add_test(NAME AllTests COMMAND run_tests)

if(ENABLE_COVERAGE)
  add_custom_target(coverage
    COMMENT "→ running tests + llvm‑cov report"
    # 1) run tests (this will produce run_tests-*.profraw in the build dir)
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    # 2) merge all the .profraw
    COMMAND llvm-profdata merge -sparse *.profraw -o coverage.profdata
    # 3) emit a full HTML report
    COMMAND llvm-cov show run_tests
            -instr-profile=coverage.profdata
            -format=html
            -output-dir coverage_html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS run_tests
  )
endif()