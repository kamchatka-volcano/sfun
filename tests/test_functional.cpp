#include <sfun/functional.h>
#include <gtest/gtest.h>

using namespace sfun;

struct Foo {
    bool operator()(int, const double&)
    {
        return false;
    }
};

TEST(Functional, Basic)
{
    auto args = callable_args<Foo>{};
    ASSERT_EQ(args.size(), 2);
    EXPECT_TRUE((std::is_same_v<decltype(args.template at<0>())::type, int>));
    EXPECT_TRUE((std::is_same_v<decltype(args.template at<1>())::type, const double&>));

    EXPECT_TRUE((std::is_same_v<callable_return_type<Foo>, bool>));
}
