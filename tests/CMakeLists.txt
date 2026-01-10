# Test executable
set(TEST_SOURCES
    test_string.cpp
    test_tag.cpp
    test_tag_helper.cpp
)

add_executable(BattleTest_Tests ${TEST_SOURCES}
    ../src/Tag.cpp
    ../src/TagHelper.cpp
    ../src/base/string.cpp
)

target_include_directories(BattleTest_Tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(BattleTest_Tests
    PRIVATE
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(BattleTest_Tests)
