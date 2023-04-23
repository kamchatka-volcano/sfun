#include <sfun/type_traits.h>
#include <gtest/gtest.h>
#include <array>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace sfun;

struct Foo {
    int val;
};

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

TEST(Traits, DecayTuple)
{
    EXPECT_TRUE((std::is_same_v<decay_tuple_t<std::tuple<const int&, double&&>>, std::tuple<int, double>>));
}

TEST(Traits, ProvidesMemberAccess)
{
    EXPECT_FALSE(provides_member_access_v<int>);
    EXPECT_FALSE(provides_member_access_v<Foo>);
    EXPECT_FALSE(provides_member_access_v<Foo*>);
    EXPECT_TRUE(provides_member_access_v<std::unique_ptr<int>>);
    EXPECT_TRUE(provides_member_access_v<std::optional<int>>);
}

TEST(Traits, IsDereferencable)
{
    EXPECT_FALSE(is_dereferencable_v<int>);
    EXPECT_FALSE(is_dereferencable_v<Foo>);
    EXPECT_TRUE(is_dereferencable_v<Foo*>);
    EXPECT_TRUE(is_dereferencable_v<std::unique_ptr<int>>);
    EXPECT_TRUE(is_dereferencable_v<std::optional<int>>);
}

TEST(Traits, ProvidesArrayElementAccess)
{
    EXPECT_FALSE(provides_array_element_access_v<int>);
    EXPECT_FALSE(provides_array_element_access_v<Foo*>);
    EXPECT_TRUE(provides_array_element_access_v<std::string>);
    EXPECT_TRUE(provides_array_element_access_v<std::vector<Foo>>);
}