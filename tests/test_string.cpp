#include <gtest/gtest.h>
#include <sfun/string_utils.h>

using namespace sfun::string_utils;

TEST(String, TrimFront)
{
    EXPECT_EQ(trimFront("  Hello world"), "Hello world");
    EXPECT_EQ(trimFront("  Hello world "), "Hello world ");
    EXPECT_EQ(trimFront(" \n\t Hello world"), "Hello world");
    EXPECT_EQ(trimFront("Hello world "), "Hello world ");
    EXPECT_EQ(trimFront(" \n \t   "), "");
    EXPECT_EQ(trimFront("\n\t"), "");
    EXPECT_EQ(trimFront(""), "");

}

TEST(String, TrimBack)
{
    EXPECT_EQ(trimBack("Hello world  "), "Hello world");
    EXPECT_EQ(trimBack(" Hello world  "), " Hello world");
    EXPECT_EQ(trimBack("Hello world \n\t"), "Hello world");
    EXPECT_EQ(trimBack(" \n \t   "), "");
    EXPECT_EQ(trimBack("\n\t"), "");
    EXPECT_EQ(trimBack(""), "");
}

TEST(String, Trim)
{
    EXPECT_EQ(trim("  Hello world"), "Hello world");
    EXPECT_EQ(trim("  Hello world "), "Hello world");
    EXPECT_EQ(trim("Hello world "), "Hello world");
    EXPECT_EQ(trim(" \n\t Hello world"), "Hello world");
    EXPECT_EQ(trim(" \n\t   "), "");
    EXPECT_EQ(trim("\n\t"), "");
    EXPECT_EQ(trim(""), "");
}

TEST(String, Split)
{
    EXPECT_EQ(split("hello world"),
              (std::vector<std::string>{"hello", "world"}));
    EXPECT_EQ(split("hello world", ","),
              (std::vector<std::string>{"hello world"}));
    EXPECT_EQ(split("hello world, nice weather", ","),
              (std::vector<std::string>{"hello world", "nice weather"}));
    EXPECT_EQ(split("hello world\n, nice weather", ",", false),
              (std::vector<std::string>{"hello world\n", " nice weather"}));
    EXPECT_EQ(split(""),
              (std::vector<std::string>{""}));
    EXPECT_EQ(split("hello world", ""),
              (std::vector<std::string>{"hello world"}));
    EXPECT_EQ(split("", ""),
              (std::vector<std::string>{""}));
}

TEST(String, Replace)
{
    EXPECT_EQ(replace("hello world","world", "space"), "hello space");
    EXPECT_EQ(replace("hello world and other world", "world", "space"), "hello space and other space");
    EXPECT_EQ(replace("hello world", "moon", "space"), "hello world");
    EXPECT_EQ(replace("hello world", "world", ""), "hello ");
    EXPECT_EQ(replace("hello world", "", "space"), "hello world");
    EXPECT_EQ(replace("", "moon", "space"), "");
    EXPECT_EQ(replace("", "", ""), "");
}

TEST(String, StartsWith)
{
    EXPECT_TRUE(startsWith("hello world", "hell"));
    EXPECT_FALSE(startsWith("hello world", "moon"));
    EXPECT_FALSE(startsWith("", "moon"));
    EXPECT_TRUE(startsWith("hello world", ""));
    EXPECT_TRUE(startsWith("", ""));
}

TEST(String, EndsWith)
{
    EXPECT_TRUE(endsWith("hello world", "world"));
    EXPECT_FALSE(endsWith("hello world", "moon"));
    EXPECT_FALSE(endsWith("", "moon"));
    EXPECT_TRUE(endsWith("hello world", ""));
    EXPECT_TRUE(endsWith("", ""));
}

TEST(String, Before)
{
    EXPECT_EQ(before("hello world", "world"), "hello ");
    EXPECT_EQ(before("hello world", "moon"), "hello world");
    EXPECT_EQ(before("hello world", ""), "");
    EXPECT_EQ(before("", "moon"), "");
    EXPECT_EQ(before("", ""), "");
}

TEST(String, After)
{
    EXPECT_EQ(after("hello world", "hello"), " world");
    EXPECT_EQ(after("hello world", "moon"), "");
    EXPECT_EQ(after("hello world", ""), "hello world");
    EXPECT_EQ(after("", "moon"), "");
    EXPECT_EQ(after("", ""), "");
}

TEST(String, Between)
{
    EXPECT_EQ(between("hello world!", "hello", "!"), " world");
    EXPECT_EQ(between("hello world!", "hello", "moon"), " world!");
    EXPECT_EQ(between("hello world!", "moon", "!"), "");
    EXPECT_EQ(between("hello world!", "", "!"), "hello world");
    EXPECT_EQ(between("hello world!", "hello", ""), "");
    EXPECT_EQ(between("", "hello", "moon"), "");
    EXPECT_EQ(between("", "", "moon"), "");
    EXPECT_EQ(between("", "moon", ""), "");
    EXPECT_EQ(between("", "", ""), "");
}
