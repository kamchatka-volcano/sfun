#include <gtest/gtest.h>
#include <sfun/string_utils.h>

using namespace sfun::string_utils;

TEST(String, TrimFront)
{
    ASSERT_EQ(trimFront("  Hello world"), "Hello world");
    ASSERT_EQ(trimFront("  Hello world "), "Hello world ");
    ASSERT_EQ(trimFront(" \n\t Hello world"), "Hello world");
    ASSERT_EQ(trimFront("Hello world "), "Hello world ");
    ASSERT_EQ(trimFront(" \n \t   "), "");
    ASSERT_EQ(trimFront("\n\t"), "");
    ASSERT_EQ(trimFront(""), "");

}

TEST(String, TrimBack)
{
    ASSERT_EQ(trimBack("Hello world  "), "Hello world");
    ASSERT_EQ(trimBack(" Hello world  "), " Hello world");
    ASSERT_EQ(trimBack("Hello world \n\t"), "Hello world");
    ASSERT_EQ(trimBack(" \n \t   "), "");
    ASSERT_EQ(trimBack("\n\t"), "");
    ASSERT_EQ(trimBack(""), "");
}

TEST(String, Trim)
{
    ASSERT_EQ(trim("  Hello world"), "Hello world");
    ASSERT_EQ(trim("  Hello world "), "Hello world");
    ASSERT_EQ(trim("Hello world "), "Hello world");
    ASSERT_EQ(trim(" \n\t Hello world"), "Hello world");
    ASSERT_EQ(trim(" \n\t   "), "");
    ASSERT_EQ(trim("\n\t"), "");
    ASSERT_EQ(trim(""), "");
}

TEST(String, Split)
{
    ASSERT_EQ(split("hello world"),
              (std::vector<std::string>{"hello", "world"}));
    ASSERT_EQ(split("hello world", ","),
              (std::vector<std::string>{"hello world"}));
    ASSERT_EQ(split("hello world, nice weather", ","),
              (std::vector<std::string>{"hello world", "nice weather"}));
    ASSERT_EQ(split("hello world\n, nice weather", ",", false),
              (std::vector<std::string>{"hello world\n", " nice weather"}));
    ASSERT_EQ(split(""),
              (std::vector<std::string>{""}));
    ASSERT_EQ(split("hello world", ""),
              (std::vector<std::string>{"hello world"}));
    ASSERT_EQ(split("", ""),
              (std::vector<std::string>{""}));
}

TEST(String, Replace)
{
    ASSERT_EQ(replace("hello world","world", "space"), "hello space");
    ASSERT_EQ(replace("hello world and other world", "world", "space"), "hello space and other space");
    ASSERT_EQ(replace("hello world", "moon", "space"), "hello world");
    ASSERT_EQ(replace("hello world", "world", ""), "hello ");
    ASSERT_EQ(replace("hello world", "", "space"), "hello world");
    ASSERT_EQ(replace("", "moon", "space"), "");
    ASSERT_EQ(replace("", "", ""), "");
}

TEST(String, StartsWith)
{
    ASSERT_TRUE(startsWith("hello world", "hell"));
    ASSERT_FALSE(startsWith("hello world", "moon"));
    ASSERT_FALSE(startsWith("", "moon"));
    ASSERT_TRUE(startsWith("hello world", ""));
    ASSERT_TRUE(startsWith("", ""));
}

TEST(String, EndsWith)
{
    ASSERT_TRUE(endsWith("hello world", "world"));
    ASSERT_FALSE(endsWith("hello world", "moon"));
    ASSERT_FALSE(endsWith("", "moon"));
    ASSERT_TRUE(endsWith("hello world", ""));
    ASSERT_TRUE(endsWith("", ""));
}

TEST(String, Before)
{
    ASSERT_EQ(before("hello world", "world"), "hello ");
    ASSERT_EQ(before("hello world", "moon"), "hello world");
    ASSERT_EQ(before("hello world", ""), "");
    ASSERT_EQ(before("", "moon"), "");
    ASSERT_EQ(before("", ""), "");
}

TEST(String, After)
{
    ASSERT_EQ(after("hello world", "hello"), " world");
    ASSERT_EQ(after("hello world", "moon"), "");
    ASSERT_EQ(after("hello world", ""), "hello world");
    ASSERT_EQ(after("", "moon"), "");
    ASSERT_EQ(after("", ""), "");
}

TEST(String, Between)
{
    ASSERT_EQ(between("hello world!", "hello", "!"), " world");
    ASSERT_EQ(between("hello world!", "hello", "moon"), " world!");
    ASSERT_EQ(between("hello world!", "moon", "!"), "");
    ASSERT_EQ(between("hello world!", "", "!"), "hello world");
    ASSERT_EQ(between("hello world!", "hello", ""), "");
    ASSERT_EQ(between("", "hello", "moon"), "");
    ASSERT_EQ(between("", "", "moon"), "");
    ASSERT_EQ(between("", "moon", ""), "");
    ASSERT_EQ(between("", "", ""), "");
}
