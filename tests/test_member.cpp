#include <sfun/member.h>
#include <gtest/gtest.h>
#include <functional>
#include <optional>
#include <string>

using namespace sfun;

namespace {

struct Bar {
    int val = 777;
};

Bar* getBar()
{
    static auto bar = Bar{};
    return &bar;
}

constexpr int testFunc()
{
    return 777;
}

int testFunc2()
{
    return 999;
}

struct Foo {
    Foo(int& refVal)
        : ref{refVal}
        , cref{refVal}
        , ptr{&refVal}
        , cptr{&refVal}
        , ptr_const{&refVal}
        , cptr_const{&refVal}
    {
    }

    member<int&> ref;
    member<const int&> cref;
    member<int*> ptr;
    member<const int*> cptr;
    member<int* const> ptr_const;
    member<const int* const> cptr_const;
    member<Bar*> ptrbar = getBar();
    member<Bar*> ptrbar_const = getBar();
    member<int> num{42};
    member<const int> num_const{42};
    member<std::unique_ptr<Bar>> smart_ptr = std::make_unique<Bar>();
    member<const std::unique_ptr<Bar>> smart_ptr_const = std::make_unique<Bar>();
    member<std::unique_ptr<const Bar>> csmart_ptr = std::make_unique<Bar>();
    member<const std::unique_ptr<const Bar>> csmart_ptr_const = std::make_unique<Bar>();
    member<std::string> str;
    member<const std::string> cstr = "Hello moon";
    member<std::optional<std::string>> opt_str;
    member<bool> flag = true;
    member<const bool> flag_const = true;
    member<std::function<void(int)>> func;
    member<const std::function<int()>> cfunc = []()
    {
        return 42;
    };
    member<int (*)()> func_ptr = testFunc;
    member<int (*const)()> cfunc_ptr = testFunc;
};

struct FooCopyable {
    FooCopyable(int& refVal)
        : ref{refVal}
        , cref{refVal}
        , ptr{&refVal}
        , cptr{&refVal}
        , ptr_const{&refVal}
        , cptr_const{&refVal}
    {
    }

    member<int&> ref;
    member<const int&> cref;
    member<int*> ptr;
    member<const int*> cptr;
    member<int* const> ptr_const;
    member<const int* const> cptr_const;
    member<int> num{42};
    member<const int> num_const{42};
    member<std::string> str;
    member<const std::string> cstr = "Hello moon";
    member<std::optional<std::string>> opt_str;
    member<const bool> flag = true;
    member<std::function<void(int)>> func;
    member<const std::function<int()>> cfunc = []()
    {
        return 42;
    };
    member<int (*)()> func_ptr = testFunc;
    member<int (*const)()> cfunc_ptr = testFunc;
};

TEST(Member, CopyStruct)
{
    int refVal = 42;
    auto foo = FooCopyable{refVal};
    foo.num = 0;
    foo.str = "foobar";
    auto foo2 = foo;
    ASSERT_EQ(foo2.num, 0);
    ASSERT_EQ(foo2.str, "foobar");
}

TEST(Member, AssignStruct)
{
    int refVal = 42;
    auto foo = FooCopyable{refVal};
    auto foo2 = FooCopyable{refVal};
    foo2.num = 0;
    foo2.str = "foobar";
    foo = foo2;
    ASSERT_EQ(foo.num, 0);
    ASSERT_EQ(foo.str, "foobar");
}

TEST(Member, MoveStruct)
{
    int refVal = 42;
    auto foo = Foo{refVal};
    auto foo2 = Foo{refVal};
    foo2.num = 0;
    foo2.str = "foobar";
    foo = std::move(foo2);
    ASSERT_EQ(foo.num, 0);
    ASSERT_EQ(foo.str, "foobar");
}

TEST(Member, Assignment)
{
    int refVal = 42;
    int testVal = 100;
    auto foo = Foo{refVal};

    foo.ref = 100;
    ASSERT_EQ(foo.ref, 100);
    foo.ptr = &testVal;
    ASSERT_EQ(*foo.ptr, 100);
    foo.cptr = &testVal;
    ASSERT_EQ(*foo.cptr, 100);
    //foo.ptr_const = &testVal;
    //foo.cptr_const = &testVal;

    foo.num = 100;
    ASSERT_EQ(foo.num, 100);
    //foo.num_const = 100;

    auto pBar = std::make_unique<Bar>();
    pBar->val = 999;

    foo.smart_ptr = std::move(pBar);
    ASSERT_EQ(foo.smart_ptr->val, 999);
    //foo.smart_ptr_const = std::make_unique<Bar>();

    foo.csmart_ptr = std::make_unique<Bar>();
    ASSERT_EQ(foo.csmart_ptr->val, 777);
    //foo.csmart_ptr_const = std::make_unique<Bar>();

    foo.str = "Hello world";
    ASSERT_EQ(foo.str, "Hello world");
    //foo.cstr = "Hello world";

    foo.opt_str = "Hello world";
    ASSERT_EQ(foo.opt_str.get().value(), "Hello world");

    // Other member assignment
    auto foo2 = Foo{testVal};

    foo.ref = foo2.ref;
    ASSERT_EQ(foo.ref, 100);
    foo.ptr = foo2.ptr;
    ASSERT_EQ(*foo.ptr, 100);
    foo.cptr = foo2.cptr;
    ASSERT_EQ(*foo.cptr, 100);
    //foo.ptr_const = foo2.ptr_const;
    //foo.cptr_const = foo2.cptr_const;

    foo.num = foo2.num;
    ASSERT_EQ(foo.num, 42);
    //foo.num_const = foo2.num_const;

    foo.smart_ptr = std::move(foo2.smart_ptr);
    ASSERT_EQ(foo.smart_ptr->val, 777);
    foo.smart_ptr_const = std::move(foo2.smart_ptr_const);

    foo.csmart_ptr = std::move(foo2.csmart_ptr);
    ASSERT_EQ(foo.csmart_ptr->val, 777);
    //foo.csmart_ptr_const = std::move(foo.csmart_ptr_const);

    foo.str = foo2.str;
    ASSERT_EQ(foo.str, "");
    //foo.cstr = foo2.cstr;

    foo.opt_str = foo2.opt_str;
    ASSERT_FALSE(foo.opt_str.get().has_value());
}

TEST(Member, Dereference)
{
    int refVal = 42;
    int testVal = 100;
    {
        auto foo = Foo{refVal};
        foo.ptr = &testVal;
        ASSERT_EQ(*foo.ptr, 100);
        foo.opt_str = "Hello world";
        ASSERT_EQ(*foo.opt_str, "Hello world");
        foo.smart_ptr = std::make_unique<Bar>();
        ASSERT_EQ((*foo.smart_ptr).val, 777);
    }
    {
        const auto foo = Foo{refVal};
        ASSERT_EQ(*foo.ptr, 42);
        ASSERT_EQ((*foo.csmart_ptr).val, 777);
    }
}

TEST(Member, MemberAccess)
{
    int refVal = 42;
    {
        auto foo = Foo{refVal};
        foo.opt_str = "Hello world";
        ASSERT_TRUE(foo.opt_str);
        ASSERT_EQ(foo.opt_str->size(), 11);

        foo.smart_ptr = std::make_unique<Bar>();
        ASSERT_EQ(foo.smart_ptr->val, 777);

        ASSERT_EQ(foo.csmart_ptr->val, 777);

        ASSERT_EQ(foo.ptrbar->val, 777);
        ASSERT_EQ(foo.ptrbar_const->val, 777);

        foo.str = "Hello world";
        ASSERT_EQ(foo.str[0], 'H');
        ASSERT_EQ(foo.cstr[0], 'H');

        int test = 10;
        foo.func = [&](int num)
        {
            test += num;
        };
        foo.func(5);
        ASSERT_EQ(test, 15);
        ASSERT_EQ(foo.cfunc(), 42);

        ASSERT_EQ(foo.cfunc_ptr(), 777);
        foo.func_ptr = testFunc2;
        ASSERT_EQ(foo.func_ptr(), 999);
    }
    {
        const auto foo = Foo{refVal};
        ASSERT_EQ(foo.cstr[0], 'H');
        ASSERT_EQ(foo.csmart_ptr->val, 777);
        ASSERT_EQ(foo.cfunc(), 42);
        ASSERT_EQ(foo.cfunc_ptr(), 777);
    }
}

TEST(Member, ArithmeticsAssignment)
{
    int refVal = 42;
    auto foo = Foo{refVal};

    foo.num += 10;
    ASSERT_EQ(foo.num, 52);
    foo.num -= 10;
    ASSERT_EQ(foo.num, 42);
    foo.num *= 2;
    ASSERT_EQ(foo.num, 84);
    foo.num /= 2;
    ASSERT_EQ(foo.num, 42);

    foo.num %= 5;
    ASSERT_EQ(foo.num, 2);

    foo.num &= 7;
    ASSERT_EQ(foo.num, 2);

    foo.num |= 1;
    ASSERT_EQ(foo.num, 3);

    foo.num ^= 1;
    ASSERT_EQ(foo.num, 2);

    foo.num <<= 2;
    ASSERT_EQ(foo.num, 8);

    foo.num >>= 1;
    ASSERT_EQ(foo.num, 4);
}

TEST(Member, Arithmetics)
{
    int refVal = 42;
    auto foo = Foo{refVal};

    auto i = 0;
    i = foo.num + 10;
    ASSERT_EQ(i, 52);
    i = 10 + foo.num;
    ASSERT_EQ(i, 52);

    i = foo.num - 10;
    ASSERT_EQ(i, 32);
    i = 84 - foo.num;
    ASSERT_EQ(i, 42);

    i = foo.num * 2;
    ASSERT_EQ(i, 84);

    i = 2 * foo.num;
    ASSERT_EQ(i, 84);

    i = foo.num / 2;
    ASSERT_EQ(i, 21);

    i = 84 / foo.num;
    ASSERT_EQ(i, 2);

    i = -foo.num;
    ASSERT_EQ(i, -42);

    i = foo.num % 5;
    ASSERT_EQ(i, 2);

    i = 85 % foo.num;
    ASSERT_EQ(i, 1);

    foo.num = 2;
    i = foo.num & 1;
    ASSERT_EQ(i, 0);

    i = 7 & foo.num;
    ASSERT_EQ(i, 2);

    i = foo.num | 1;
    ASSERT_EQ(i, 3);
    i = 1 | foo.num;
    ASSERT_EQ(i, 3);

    foo.num = 3;
    i = foo.num ^ 1;
    ASSERT_EQ(i, 2);

    i = 1 ^ foo.num;
    ASSERT_EQ(i, 2);

    foo.num = 2;
    i = foo.num << 2;
    ASSERT_EQ(i, 8);

    i = 8 >> foo.num;
    ASSERT_EQ(i, 2);
}

TEST(Member, IncrementDecrement)
{
    int refVal = 42;
    auto foo = Foo{refVal};
    {
        auto i = foo.num++;
        ASSERT_EQ(i, 42);
        ASSERT_EQ(foo.num, 43);
    }
    {
        auto i = ++foo.num;
        ASSERT_EQ(i, 44);
        ASSERT_EQ(foo.num, 44);
    }
    {
        auto i = foo.num--;
        ASSERT_EQ(i, 44);
        ASSERT_EQ(foo.num, 43);
    }
    {
        auto i = --foo.num;
        ASSERT_EQ(i, 42);
        ASSERT_EQ(foo.num, 42);
    }
}

TEST(Member, Logical)
{
    int refVal = 42;
    auto foo = Foo{refVal};

    ASSERT_FALSE(!foo.flag);
    ASSERT_TRUE(foo.flag || false);
    ASSERT_TRUE(foo.flag && true);
}

TEST(Member, Comparison)
{
    int refVal = 42;
    auto foo = Foo{refVal};

    ASSERT_TRUE(foo.flag == true);
    ASSERT_TRUE(foo.flag != false);
    ASSERT_TRUE(foo.num < 100);
    ASSERT_TRUE(foo.num > 10);
    ASSERT_TRUE(foo.num >= 42);
    ASSERT_TRUE(foo.num <= 42);
    ASSERT_TRUE(foo.cstr == "Hello moon");
}

TEST(Member, NoConstPropagation)
{
    int refVal = 42;
    const auto foo = Foo{refVal};

    foo.ref = 100;
    ASSERT_EQ(refVal, 100);
    *foo.ptr = 101;
    ASSERT_EQ(refVal, 101);
    *foo.ptr_const = 102;
    ASSERT_EQ(refVal, 102);
    //foo.ptr = &testVal;
    //foo.cptr = &testVal;
    //foo.ptr_const = &testVal;
    //foo.cptr_const = &testVal;

    //foo.num = 100;
    //foo.num_const = 100;

    auto pBar = std::make_unique<Bar>();
    pBar->val = 999;

    *foo.smart_ptr = *pBar;
    ASSERT_EQ(foo.smart_ptr->val, 999);

    foo.smart_ptr->val = 100;
    ASSERT_EQ(foo.smart_ptr->val, 100);

    *foo.smart_ptr_const = *pBar;
    ASSERT_EQ(foo.smart_ptr_const->val, 999);

    foo.smart_ptr_const->val = 100;
    ASSERT_EQ(foo.smart_ptr_const->val, 100);

    //foo.smart_ptr = std::move(pBar);
    //foo.smart_ptr_const = std::make_unique<Bar>();
    //foo.csmart_ptr = std::make_unique<Bar>();
    //foo.csmart_ptr_const = std::make_unique<Bar>();
    //foo.str = "Hello world";
    //foo.cstr = "Hello world";
    //foo.opt_str = "Hello world";
}

TEST(Member, Constexpr)
{
    struct T {
        member<int (*)()> func_ptr = testFunc;
    };
    constexpr auto t = T{};
    static_assert(t.func_ptr() == 777);
}

} //namespace