#include <sfun/utility.h>
#include <gtest/gtest.h>

using namespace sfun;

TEST(Utility, IndexRange)
{
    {
        using range = make_index_range<1, 4>;
        ASSERT_EQ(range::size(), 3);
        EXPECT_TRUE((std::is_same_v<range, std::index_sequence<1, 2, 3>>));
    }
    {
        using range = make_index_range<0, 3>;
        ASSERT_EQ(range::size(), 3);
        EXPECT_TRUE((std::is_same_v<range, std::index_sequence<0, 1, 2>>));
    }
    {
        using range = make_index_range<1, 1>;
        ASSERT_EQ(range::size(), 0);
        EXPECT_TRUE((std::is_same_v<range, std::index_sequence<>>));
    }
}
