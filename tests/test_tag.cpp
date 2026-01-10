#include <gtest/gtest.h>
#include "Tag.h"

// Tag 생성 테스트
TEST(TagTest, DefaultConstructor)
{
    Tag empty;
    EXPECT_TRUE(empty.IsNpos());
    EXPECT_EQ(empty, Tag::npos);
}

TEST(TagTest, SingleLevelTag)
{
    Tag tag("weapon");
    EXPECT_FALSE(tag.IsNpos());
    EXPECT_EQ(tag.GetDepth(), 1);
    EXPECT_EQ(tag.ToString(), "weapon");
}

TEST(TagTest, MultiLevelTag)
{
    Tag tag("weapon.sword.legendary");
    EXPECT_EQ(tag.GetDepth(), 3);
    EXPECT_EQ(tag.ToString(), "weapon.sword.legendary");
}

// 비교 연산자 테스트
TEST(TagTest, EqualityOperator)
{
    Tag tag1("weapon.sword");
    Tag tag2("weapon.sword");
    Tag tag3("weapon.bow");

    EXPECT_TRUE(tag1 == tag2);
    EXPECT_FALSE(tag1 == tag3);
}

TEST(TagTest, InequalityOperator)
{
    Tag tag1("weapon.sword");
    Tag tag2("weapon.bow");

    EXPECT_TRUE(tag1 != tag2);
    EXPECT_FALSE(tag1 != tag1);
}

// 계층 구조 테스트 - IsParent
TEST(TagTest, IsParent_BasicRelationship)
{
    Tag parent("weapon");
    Tag child("weapon.sword");

    auto result = parent.IsParent(child);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);
}

TEST(TagTest, IsParent_MultiLevel)
{
    Tag root("weapon");
    Tag grandchild("weapon.sword.legendary");

    auto result = root.IsParent(grandchild);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);
}

TEST(TagTest, IsParent_SameTag)
{
    Tag tag("weapon");

    auto result = tag.IsParent(tag);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);
}

TEST(TagTest, IsParent_NotParent)
{
    Tag tag1("weapon");
    Tag tag2("armor");

    auto result = tag1.IsParent(tag2);
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(*result);
}

TEST(TagTest, IsParent_WithNpos)
{
    Tag tag("weapon");
    Tag npos;

    EXPECT_FALSE(tag.IsParent(npos).has_value());
    EXPECT_FALSE(npos.IsParent(tag).has_value());
}

// 계층 구조 테스트 - IsChild
TEST(TagTest, IsChild_BasicRelationship)
{
    Tag parent("weapon");
    Tag child("weapon.sword");

    auto result = child.IsChild(parent);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);
}

TEST(TagTest, IsChild_NotChild)
{
    Tag tag1("weapon");
    Tag tag2("armor");

    auto result = tag2.IsChild(tag1);
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(*result);
}

// 계층 구조 테스트 - IsSibling
TEST(TagTest, IsSibling_BasicRelationship)
{
    Tag sword("weapon.sword");
    Tag bow("weapon.bow");

    auto result = sword.IsSibling(bow);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);
}

TEST(TagTest, IsSibling_SameTag)
{
    Tag tag("weapon.sword");

    auto result = tag.IsSibling(tag);
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(*result);
}

TEST(TagTest, IsSibling_DifferentDepth)
{
    Tag tag1("weapon");
    Tag tag2("weapon.sword");

    auto result = tag1.IsSibling(tag2);
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(*result);
}

TEST(TagTest, IsSibling_DifferentParent)
{
    Tag tag1("weapon.sword");
    Tag tag2("armor.helmet");

    auto result = tag1.IsSibling(tag2);
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(*result);
}

TEST(TagTest, IsSibling_RootLevel)
{
    Tag tag1("weapon");
    Tag tag2("armor");

    auto result = tag1.IsSibling(tag2);
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(*result);
}

// Tag 메서드 테스트 - GetDepth
TEST(TagTest, GetDepth)
{
    EXPECT_EQ(Tag::npos.GetDepth(), 0);
    EXPECT_EQ(Tag("weapon").GetDepth(), 1);
    EXPECT_EQ(Tag("weapon.sword").GetDepth(), 2);
    EXPECT_EQ(Tag("weapon.sword.legendary.excalibur").GetDepth(), 4);
}

// Tag 메서드 테스트 - GetRoot
TEST(TagTest, GetRoot)
{
    Tag tag("weapon.sword.legendary.excalibur");
    Tag root = tag.GetRoot();

    EXPECT_EQ(root.ToString(), "weapon");
}

TEST(TagTest, GetRoot_Npos)
{
    Tag npos;
    Tag root = npos.GetRoot();

    EXPECT_TRUE(root.IsNpos());
}

// Tag 메서드 테스트 - GetParent
TEST(TagTest, GetParent)
{
    Tag tag("weapon.sword.legendary.excalibur");

    EXPECT_EQ(tag.GetParent(0).ToString(), "weapon");
    EXPECT_EQ(tag.GetParent(1).ToString(), "weapon.sword");
    EXPECT_EQ(tag.GetParent(2).ToString(), "weapon.sword.legendary");
    EXPECT_EQ(tag.GetParent(3).ToString(), "weapon.sword.legendary.excalibur");
}

TEST(TagTest, GetParent_OutOfRange)
{
    Tag tag("weapon.sword");
    Tag parent = tag.GetParent(5);

    EXPECT_TRUE(parent.IsNpos());
}

// Tag 메서드 테스트 - GetSubTag
TEST(TagTest, GetSubTag)
{
    Tag tag("weapon.sword.legendary.excalibur");

    EXPECT_EQ(tag.GetSubTag(1).ToString(), "weapon");
    EXPECT_EQ(tag.GetSubTag(2).ToString(), "weapon.sword");
    EXPECT_EQ(tag.GetSubTag(3).ToString(), "weapon.sword.legendary");
}

TEST(TagTest, GetSubTag_LargerThanDepth)
{
    Tag tag("weapon.sword");
    Tag sub = tag.GetSubTag(10);

    EXPECT_EQ(sub, tag);
}

// MakeTag 팩토리 함수 테스트
TEST(TagTest, MakeTag_Valid)
{
    auto tag = MakeTag("weapon.sword");

    ASSERT_TRUE(tag.has_value());
    EXPECT_EQ(tag->ToString(), "weapon.sword");
}

TEST(TagTest, MakeTag_EmptyString)
{
    auto tag = MakeTag("");

    EXPECT_FALSE(tag.has_value());
}

TEST(TagTest, MakeTag_EmptyPart)
{
    EXPECT_FALSE(MakeTag("weapon..sword").has_value());
    EXPECT_FALSE(MakeTag(".weapon").has_value());
    EXPECT_FALSE(MakeTag("weapon.").has_value());
}

TEST(TagTest, MakeTag_SingleLevel)
{
    auto tag = MakeTag("weapon");

    ASSERT_TRUE(tag.has_value());
    EXPECT_EQ(tag->GetDepth(), 1);
    EXPECT_EQ(tag->ToString(), "weapon");
}
