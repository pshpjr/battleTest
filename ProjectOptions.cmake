include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


include(CheckCXXSourceCompiles)


macro(battleTest_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)

    message(STATUS "Sanity checking UndefinedBehaviorSanitizer, it should be supported on this platform")
    set(TEST_PROGRAM "int main() { return 0; }")

    # Check if UndefinedBehaviorSanitizer works at link time
    set(CMAKE_REQUIRED_FLAGS "-fsanitize=undefined")
    set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=undefined")
    check_cxx_source_compiles("${TEST_PROGRAM}" HAS_UBSAN_LINK_SUPPORT)

    if(HAS_UBSAN_LINK_SUPPORT)
      message(STATUS "UndefinedBehaviorSanitizer is supported at both compile and link time.")
      set(SUPPORTS_UBSAN ON)
    else()
      message(WARNING "UndefinedBehaviorSanitizer is NOT supported at link time.")
      set(SUPPORTS_UBSAN OFF)
    endif()
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    if (NOT WIN32)
      message(STATUS "Sanity checking AddressSanitizer, it should be supported on this platform")
      set(TEST_PROGRAM "int main() { return 0; }")

      # Check if AddressSanitizer works at link time
      set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
      set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=address")
      check_cxx_source_compiles("${TEST_PROGRAM}" HAS_ASAN_LINK_SUPPORT)

      if(HAS_ASAN_LINK_SUPPORT)
        message(STATUS "AddressSanitizer is supported at both compile and link time.")
        set(SUPPORTS_ASAN ON)
      else()
        message(WARNING "AddressSanitizer is NOT supported at link time.")
        set(SUPPORTS_ASAN OFF)
      endif()
    else()
      set(SUPPORTS_ASAN ON)
    endif()
  endif()
endmacro()

macro(battleTest_setup_options)
  option(battleTest_ENABLE_HARDENING "Enable hardening" ON)
  option(battleTest_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    battleTest_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    battleTest_ENABLE_HARDENING
    OFF)

  battleTest_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR battleTest_PACKAGING_MAINTAINER_MODE)
    option(battleTest_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(battleTest_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(battleTest_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(battleTest_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(battleTest_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(battleTest_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(battleTest_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(battleTest_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(battleTest_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(battleTest_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(battleTest_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(battleTest_ENABLE_PCH "Enable precompiled headers" OFF)
    option(battleTest_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(battleTest_ENABLE_IPO "Enable IPO/LTO" ON)
    option(battleTest_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(battleTest_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(battleTest_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(battleTest_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(battleTest_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(battleTest_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(battleTest_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(battleTest_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(battleTest_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(battleTest_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(battleTest_ENABLE_PCH "Enable precompiled headers" OFF)
    option(battleTest_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      battleTest_ENABLE_IPO
      battleTest_WARNINGS_AS_ERRORS
      battleTest_ENABLE_USER_LINKER
      battleTest_ENABLE_SANITIZER_ADDRESS
      battleTest_ENABLE_SANITIZER_LEAK
      battleTest_ENABLE_SANITIZER_UNDEFINED
      battleTest_ENABLE_SANITIZER_THREAD
      battleTest_ENABLE_SANITIZER_MEMORY
      battleTest_ENABLE_UNITY_BUILD
      battleTest_ENABLE_CLANG_TIDY
      battleTest_ENABLE_CPPCHECK
      battleTest_ENABLE_COVERAGE
      battleTest_ENABLE_PCH
      battleTest_ENABLE_CACHE)
  endif()

  battleTest_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (battleTest_ENABLE_SANITIZER_ADDRESS OR battleTest_ENABLE_SANITIZER_THREAD OR battleTest_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(battleTest_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(battleTest_global_options)
  if(battleTest_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    battleTest_enable_ipo()
  endif()

  battleTest_supports_sanitizers()

  if(battleTest_ENABLE_HARDENING AND battleTest_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR battleTest_ENABLE_SANITIZER_UNDEFINED
       OR battleTest_ENABLE_SANITIZER_ADDRESS
       OR battleTest_ENABLE_SANITIZER_THREAD
       OR battleTest_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${battleTest_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${battleTest_ENABLE_SANITIZER_UNDEFINED}")
    battleTest_enable_hardening(battleTest_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(battleTest_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(battleTest_warnings INTERFACE)
  add_library(battleTest_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  battleTest_set_project_warnings(
    battleTest_warnings
    ${battleTest_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(battleTest_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    battleTest_configure_linker(battleTest_options)
  endif()

  include(cmake/Sanitizers.cmake)
  battleTest_enable_sanitizers(
    battleTest_options
    ${battleTest_ENABLE_SANITIZER_ADDRESS}
    ${battleTest_ENABLE_SANITIZER_LEAK}
    ${battleTest_ENABLE_SANITIZER_UNDEFINED}
    ${battleTest_ENABLE_SANITIZER_THREAD}
    ${battleTest_ENABLE_SANITIZER_MEMORY})

  set_target_properties(battleTest_options PROPERTIES UNITY_BUILD ${battleTest_ENABLE_UNITY_BUILD})

  if(battleTest_ENABLE_PCH)
    target_precompile_headers(
      battleTest_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(battleTest_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    battleTest_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(battleTest_ENABLE_CLANG_TIDY)
    battleTest_enable_clang_tidy(battleTest_options ${battleTest_WARNINGS_AS_ERRORS})
  endif()

  if(battleTest_ENABLE_CPPCHECK)
    battleTest_enable_cppcheck(${battleTest_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(battleTest_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    battleTest_enable_coverage(battleTest_options)
  endif()

  if(battleTest_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(battleTest_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(battleTest_ENABLE_HARDENING AND NOT battleTest_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR battleTest_ENABLE_SANITIZER_UNDEFINED
       OR battleTest_ENABLE_SANITIZER_ADDRESS
       OR battleTest_ENABLE_SANITIZER_THREAD
       OR battleTest_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    battleTest_enable_hardening(battleTest_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
