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
              (std::vector<std::string_view>{"hello", "world"}));
    EXPECT_EQ(split("hello world", ","),
              (std::vector<std::string_view>{"hello world"}));
    EXPECT_EQ(split("hello world, nice weather", ","),
              (std::vector<std::string_view>{"hello world", "nice weather"}));
    EXPECT_EQ(split("hello world\n, nice weather", ",", false),
              (std::vector<std::string_view>{"hello world\n", " nice weather"}));
    EXPECT_EQ(split(""),
              (std::vector<std::string_view>{""}));
    EXPECT_EQ(split("hello world", ""),
              (std::vector<std::string_view>{"hello world"}));
    EXPECT_EQ(split("", ""),
              (std::vector<std::string_view>{""}));
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

TEST(String, Join)
{
    {
        auto stringList = std::vector<std::string>{"a", "b", "c"};
        EXPECT_EQ(join(stringList, ""), "abc");
        EXPECT_EQ(join(stringList, ", "), "a, b, c");

        stringList = {"a"};
        EXPECT_EQ(join(stringList, ""), "a");
        EXPECT_EQ(join(stringList, ", "), "a");

        stringList = {};
        EXPECT_EQ(join(stringList, ""), "");
        EXPECT_EQ(join(stringList, ", "), "");
    }
    {
        auto viewList = std::vector<std::string_view>{"a", "b" , "c"};
        EXPECT_EQ(join(viewList, ""), "abc");
        EXPECT_EQ(join(viewList, ", "), "a, b, c");

        viewList = {"a"};
        EXPECT_EQ(join(viewList, ""), "a");
        EXPECT_EQ(join(viewList, ", "), "a");

        viewList = {};
        EXPECT_EQ(join(viewList, ""), "");
        EXPECT_EQ(join(viewList, ", "), "");
    }
    {
        EXPECT_EQ(join(std::vector<std::string>{"a", "b", "c"}, ""), "abc");
        EXPECT_EQ(join(std::vector<std::string>{"a", "b", "c"}, ", "), "a, b, c");

        EXPECT_EQ(join(std::vector<std::string>{"a"}, ""), "a");
        EXPECT_EQ(join(std::vector<std::string>{"a"}, ", "), "a");

        EXPECT_EQ(join(std::vector<std::string>{}, ""), "");
        EXPECT_EQ(join(std::vector<std::string>{}, ", "), "");
    }
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

TEST(String, cctypeWrappers)
{
    EXPECT_TRUE(isalnum('9'));
    EXPECT_TRUE(isalnum('z'));
    EXPECT_FALSE(isalnum('!'));

    EXPECT_TRUE(isalpha('a'));
    EXPECT_FALSE(isalpha('9'));

    EXPECT_TRUE(isblank(' '));
    EXPECT_FALSE(isblank('9'));

    EXPECT_TRUE(iscntrl(0x7F));
    EXPECT_FALSE(iscntrl('9'));

    EXPECT_TRUE(isdigit('9'));
    EXPECT_FALSE(isdigit('a'));

    EXPECT_TRUE(isgraph('!'));
    EXPECT_TRUE(isgraph('9'));
    EXPECT_FALSE(isgraph(' '));
    EXPECT_FALSE(isgraph(0x7F));

    EXPECT_TRUE(islower('a'));
    EXPECT_FALSE(islower('A'));

    EXPECT_TRUE(isprint('!'));
    EXPECT_TRUE(isprint('9'));
    EXPECT_TRUE(isprint(' '));
    EXPECT_FALSE(isprint(0x7F));

    EXPECT_TRUE(ispunct(','));
    EXPECT_FALSE(ispunct('A'));

    EXPECT_TRUE(isspace(' '));
    EXPECT_FALSE(isspace('A'));

    EXPECT_TRUE(isupper('A'));
    EXPECT_FALSE(isupper('a'));

    EXPECT_TRUE(isxdigit('9'));
    EXPECT_TRUE(isxdigit('A'));
    EXPECT_FALSE(isxdigit('U'));

    EXPECT_EQ(tolower('a'), 'a');
    EXPECT_EQ(tolower('A'), 'a');
    EXPECT_NE(tolower('Z'), 'a');

    EXPECT_EQ(toupper('a'), 'A');
    EXPECT_EQ(toupper('A'), 'A');
    EXPECT_NE(toupper('z'), 'z');
}