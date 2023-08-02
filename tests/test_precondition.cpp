#include <sfun/precondition.h>
#include <gtest/gtest.h>
#include <functional>
#include <optional>
#include <string>
#include <vector>

TEST(Precondition, NotEmpty)
{
    auto dataSize = [](sfun::not_empty<const std::vector<int>&> data)
    {
        return data.get().size();
    };

    auto data = std::vector{1, 2, 3};
    ASSERT_EQ(dataSize(data), 3);
}

TEST(Precondition, NotEmptyViolated)
{
    auto dataSize = [](sfun::not_empty<const std::vector<int>&> data)
    {
        return data.get().size();
    };

    ASSERT_EXIT(
            {
                std::set_terminate(
                        []
                        {
                            exit(1);
                        });
                dataSize(std::vector<int>{});
                exit(0);
            },
            ::testing::ExitedWithCode(1),
            ".*");
}

struct Foo {
    int num = 42;
};

TEST(Precondition, Valid)
{
    auto foo = Foo{};
    auto getNum = [](sfun::valid<const Foo*> pFoo)
    {
        return pFoo.get()->num;
    };

    ASSERT_EQ(getNum(&foo), 42);
}

TEST(Precondition, ValidViolated)
{
    auto getNum = [](sfun::valid<const Foo*> pFoo)
    {
        return pFoo.get()->num;
    };

    ASSERT_EXIT(
            {
                std::set_terminate(
                        []
                        {
                            exit(1);
                        });
                getNum(nullptr);
                exit(0);
            },
            ::testing::ExitedWithCode(1),
            ".*");
}

TEST(Precondition, Interval)
{
    auto getNum = [](sfun::interval<int, 0, 128> num)
    {
        return 1000 + num;
    };

    ASSERT_EQ(getNum(0), 1000);
    ASSERT_EQ(getNum(127), 1127);
}

TEST(Precondition, IntervalViolated)
{
    auto getNum = [](sfun::interval<int, 0, 128> num)
    {
        return 1000 + num;
    };

    ASSERT_EXIT(
            {
                std::set_terminate(
                        []
                        {
                            exit(1);
                        });
                getNum(128);
                exit(0);
            },
            ::testing::ExitedWithCode(1),
            ".*");
}

TEST(Precondition, IntervalOpen)
{
    auto getNum = [](sfun::interval_open<int, 0, 128> num)
    {
        return 1000 + num;
    };
    ASSERT_EQ(getNum(1), 1001);
    ASSERT_EQ(getNum(127), 1127);
}

TEST(Precondition, IntervalOpenViolated)
{
    auto getNum = [](sfun::interval_open<int, 0, 128> num)
    {
        return 1000 + num;
    };

    ASSERT_EXIT(
            {
                std::set_terminate(
                        []
                        {
                            exit(1);
                        });
                getNum(0);
                exit(0);
            },
            ::testing::ExitedWithCode(1),
            ".*");
}

TEST(Precondition, IntervalClosed)
{
    auto getNum = [](sfun::interval_closed<int, 0, 128> num)
    {
        return 1000 + num;
    };

    ASSERT_EQ(getNum(0), 1000);
    ASSERT_EQ(getNum(128), 1128);
}

TEST(Precondition, IntervalClosedViolated)
{
    auto getNum = [](sfun::interval_closed<int, 0, 128> num)
    {
        return 1000 + num;
    };

    ASSERT_EXIT(
            {
                std::set_terminate(
                        []
                        {
                            exit(1);
                        });
                getNum(129);
                exit(0);
            },
            ::testing::ExitedWithCode(1),
            ".*");
}

TEST(Precondition, NotNegative)
{
    auto getNum = [](sfun::not_negative<int> num)
    {
        return 1000 + num;
    };
    ASSERT_EQ(getNum(0), 1000);
    ASSERT_EQ(getNum(127), 1127);
}

TEST(Precondition, NotNegativeViolated)
{
    auto getNum = [](sfun::not_negative<float> num)
    {
        return 1000.f + num;
    };

    ASSERT_EXIT(
            {
                std::set_terminate(
                        []
                        {
                            exit(1);
                        });
                getNum(-1.f);
                exit(0);
            },
            ::testing::ExitedWithCode(1),
            ".*");
}