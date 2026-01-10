#include <gtest/gtest.h>
#include "base/string.h"

TEST(StringTest, MakeLower)
{
    EXPECT_EQ(MakeLower("HELLO"), "hello");
    EXPECT_EQ(MakeLower("WoRlD"), "world");
    EXPECT_EQ(MakeLower("123ABC"), "123abc");
    EXPECT_EQ(MakeLower(""), "");
}

TEST(StringTest, SplitSingleDelimiter)
{
    auto result = Split("a.b.c", '.');
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");
}

TEST(StringTest, SplitMultipleDelimitersString)
{
    auto result = Split("a.b,c;d", ".,;");
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");
    EXPECT_EQ(result[3], "d");
}

TEST(StringTest, SplitMultipleDelimitersVector)
{
    auto result = Split("a-b_c", std::vector<char>{'-', '_'});
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");
}

TEST(StringTest, SplitEmptyString)
{
    auto result = Split("", '.');
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "");
}

TEST(StringTest, SplitNoDelimiter)
{
    auto result = Split("abc", '.');
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "abc");
}

TEST(StringTest, SplitConsecutiveDelimiters)
{
    auto result = Split("a..b", '.');
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "");
    EXPECT_EQ(result[2], "b");
}

TEST(StringTest, SplitStartsWithDelimiter)
{
    auto result = Split(".abc", '.');
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "");
    EXPECT_EQ(result[1], "abc");
}

TEST(StringTest, SplitEndsWithDelimiter)
{
    auto result = Split("abc.", '.');
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "abc");
    EXPECT_EQ(result[1], "");
}
