#include <gtest/gtest.h>
#include <sfun/type_traits.h>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

using namespace sfun;

struct Foo{};

TEST(Traits, IsOptional)
{
    EXPECT_FALSE(is_optional_v<int>);
    EXPECT_FALSE(is_optional_v<Foo>);
    EXPECT_TRUE(is_optional_v<std::optional<int>>);
    EXPECT_TRUE(is_optional_v<std::optional<Foo>>);
}

TEST(Traits, RemoveOptional)
{
    EXPECT_TRUE((std::is_same_v<remove_optional_t<int>, int>));
    EXPECT_TRUE((std::is_same_v<remove_optional_t<Foo>, Foo>));
    EXPECT_TRUE((std::is_same_v<remove_optional_t<std::optional<int>>, int>));
    EXPECT_TRUE((std::is_same_v<remove_optional_t<std::optional<Foo>>, Foo>));
}

TEST(Traits, IsSequenceContainer)
{
    EXPECT_FALSE(is_dynamic_sequence_container_v<int>);
    EXPECT_FALSE(is_dynamic_sequence_container_v<Foo>);
    EXPECT_FALSE((is_dynamic_sequence_container_v<std::array<int, 5>>));
    EXPECT_FALSE((is_dynamic_sequence_container_v<std::map<int, std::string>>));
    EXPECT_TRUE((is_dynamic_sequence_container_v<std::vector<int>>));
    EXPECT_TRUE((is_dynamic_sequence_container_v<std::list<Foo>>));
}

TEST(Traits, IsAssociativeContainer)
{
    EXPECT_FALSE(is_associative_container_v<int>);
    EXPECT_FALSE(is_associative_container_v<Foo>);
    EXPECT_FALSE((is_associative_container_v<std::array<int, 5>>));
    EXPECT_FALSE((is_associative_container_v<std::vector<int>>));
    EXPECT_FALSE((is_associative_container_v<std::list<Foo>>));
    EXPECT_TRUE((is_associative_container_v<std::map<int, std::string>>));
    EXPECT_TRUE((is_associative_container_v<std::unordered_map<int, Foo>>));
}