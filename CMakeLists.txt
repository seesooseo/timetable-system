cmake_minimum_required(VERSION 3.10)
project(TimetableSystem LANGUAGES CXX VERSION 1.0)

# — Enable C++ exception support for both MSVC and Clang —
if (MSVC)
  message(STATUS "Enabling C++ exceptions (/EHsc) for MSVC")
  add_compile_options(/EHsc)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(STATUS "Enabling C++ exceptions (-fexceptions) for Clang")
  add_compile_options(-fexceptions)
endif()

# — Make sure every TU gets exceptions support under Clang —
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(STATUS "Appending -fexceptions to CMAKE_CXX_FLAGS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
endif()

# — Similarly for MSVC —  
if (MSVC)
  message(STATUS "Appending /EHsc to CMAKE_CXX_FLAGS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# —————————————
# Static analysis setup (cppcheck only)
# —————————————
find_program(CPPCHECK_EXE NAMES cppcheck)
if (CPPCHECK_EXE)
  message(STATUS "Found cppcheck: ${CPPCHECK_EXE}")
  add_custom_target(cppcheck
    COMMAND ${CPPCHECK_EXE}
            --enable=all
            --inconclusive
            --std=c++17
            --quiet
            --suppress=missingIncludeSystem
            ${CMAKE_SOURCE_DIR}/src
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "→ Running cppcheck static analysis"
  )
endif()

# —————————————
# Coverage option
# —————————————
option(ENABLE_COVERAGE "Build with code‑coverage flags" OFF)
if (ENABLE_COVERAGE)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Enabling Clang Coverage Instrumentation")
    add_compile_options(-O0 -g -fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-fprofile-instr-generate)
  else()
    message(FATAL_ERROR "Coverage only supported with Clang in this profile")
  endif()
endif()

# —————————————
# Source & Targets
# —————————————
file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM ALL_SOURCES
    "${CMAKE_SOURCE_DIR}/src/tests.cpp"
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

add_library(core STATIC
  ${ALL_SOURCES}
  ${CORE_SOURCES}
)

target_include_directories(core PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/lib
)

add_executable(timetable
    src/main.cpp
    src/LectureManagementMenu.cpp
    src/LecturerManager.cpp
    src/LectureManagementMenu.hpp
    src/Timetable.cpp
    src/TimeUtility.hpp
 "src/Persistence.cpp" "src/TimeValidation.cpp" "src/SampleData.cpp")
target_link_libraries(timetable PRIVATE core)

# — Enable testing and set up test executable —
enable_testing()
add_executable(run_tests
    tests/tests.cpp
    src/LectureManagementMenu.cpp
    src/LecturerManager.cpp
    src/LectureManagementMenu.hpp
    src/Timetable.cpp
    src/TimeUtility.hpp
 "src/Persistence.cpp" "src/TimeValidation.cpp" "src/SampleData.cpp")
target_link_libraries(run_tests PRIVATE core)
add_test(NAME AllTests COMMAND run_tests)

# — Attach coverage environment to the test —
if (ENABLE_COVERAGE)
  set_tests_properties(AllTests PROPERTIES
    ENVIRONMENT "LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/coverage.profraw"
  )
endif()

# — Coverage reporting target —
if (ENABLE_COVERAGE)
  add_custom_target(coverage
    COMMENT "→ running tests + llvm‑cov report"
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMAND llvm-profdata merge -sparse coverage.profraw -o coverage.profdata
    COMMAND llvm-cov show
            $<TARGET_FILE:run_tests>
            -instr-profile=coverage.profdata
            -ignore-filename-regex "${CMAKE_SOURCE_DIR}/lib/.*"
            -ignore-filename-regex ".*[/\\\\]json\\.hpp$"
            -format=html
            -output-dir coverage_html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS run_tests
  )
endif()

# —————————————
# Package (ZIP) target
# —————————————
include(InstallRequiredSystemLibraries)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src
        DESTINATION SRC)

# install tests (if you have a tests folder)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/tests
        DESTINATION TESTS
        FILES_MATCHING PATTERN "*.cpp")

# install just the build-system files, excluding CPack/Ninja internals
install(DIRECTORY ${CMAKE_BINARY_DIR}/
        DESTINATION BUILD
        PATTERN ".git"                    EXCLUDE
        PATTERN "_CPack_Packages"         EXCLUDE
        PATTERN "_CPack_Packages/*"       EXCLUDE
        PATTERN ".ninja*"                 EXCLUDE
        PATTERN "*.obj"                   EXCLUDE
        PATTERN "*.o"                     EXCLUDE
        PATTERN "*.exe"                   EXCLUDE
        PATTERN "coverage_html"           EXCLUDE
)

# CPack configuration
set(CPACK_GENERATOR        "ZIP")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
include(CPack)
