#include <sfun/type_list.h>
#include <gtest/gtest.h>

using namespace sfun;

struct Foo {};

TEST(TypeList, Basic)
{
    auto list = type_list<int, double, Foo>{};
    ASSERT_EQ(list.size(), 3);
    EXPECT_TRUE((std::is_same_v<to_type<decltype(list.template at<0>())>, int>));
    EXPECT_TRUE((std::is_same_v<to_type<decltype(list.template at<1>())>, double>));
    EXPECT_TRUE((std::is_same_v<to_type<decltype(list.template at<2>())>, Foo>));
    EXPECT_TRUE((std::is_same_v<decltype(list)::tuple, std::tuple<int, double, Foo>>));
}
