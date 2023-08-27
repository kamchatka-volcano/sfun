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

namespace {

Bar* getBar()
{
    static auto bar = Bar{};
    return &bar;
}

constexpr int testFunc()
{
    return 777;
}

} //namespace

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

    indirect_member<int&> ref;
    indirect_member<const int&> cref;
    indirect_member<int*> ptr;
    indirect_member<const int*> cptr;
    indirect_member<int* const> ptr_const;
    indirect_member<const int* const> cptr_const;
    indirect_member<Bar*> ptrbar = getBar();
    indirect_member<const Bar*> ptrbar_const = getBar();
    indirect_member<std::unique_ptr<Bar>> smart_ptr = std::make_unique<Bar>();
    indirect_member<const std::unique_ptr<Bar>> smart_ptr_const = std::make_unique<Bar>();
    indirect_member<std::unique_ptr<const Bar>> csmart_ptr = std::make_unique<Bar>();
    indirect_member<const std::unique_ptr<const Bar>> csmart_ptr_const = std::make_unique<Bar>();
    indirect_member<int (*)()> func_ptr = testFunc;
    indirect_member<int (*const)()> cfunc_ptr = testFunc;
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

    indirect_member<int&> ref;
    indirect_member<const int&> cref;
    indirect_member<int*> ptr;
    indirect_member<const int*> cptr;
    indirect_member<int* const> ptr_const;
    indirect_member<const int* const> cptr_const;
    indirect_member<int (*)()> func_ptr = testFunc;
    indirect_member<int (*const)()> cfunc_ptr = testFunc;
};

TEST(IndirectMember, CopyStruct)
{
    int refVal = 42;
    auto foo = FooCopyable{refVal};
    auto foo2 = foo;
    ASSERT_EQ(foo2.ref.get(), 42);
}

TEST(IndirectMember, AssignStruct)
{
    int refVal = 42;
    int refVal2 = 99;
    auto foo = FooCopyable{refVal};
    auto foo2 = FooCopyable{refVal2};
    foo = foo2;
    ASSERT_EQ(foo.ref.get(), 99);
}

TEST(IndirectMember, MoveStruct)
{
    int refVal = 42;
    int refVal2 = 99;
    auto foo = Foo{refVal};
    auto foo2 = Foo{refVal2};
    foo2.ref.get() = 100;
    foo = std::move(foo2);
    ASSERT_EQ(foo.ref.get(), 100);
}

TEST(IndirectMember, Assignment)
{
    int refVal = 42;
    auto foo = Foo{refVal};

    foo.ref.get() = 100;
    ASSERT_EQ(refVal, 100);

    *foo.ptr = 99;
    ASSERT_EQ(refVal, 99);
    ASSERT_EQ(*foo.ptr, 99);

    //foo.cptr = 7;
    //foo.cptr_const = 7;
    *foo.ptr_const = 7;
    ASSERT_EQ(refVal, 7);
    ASSERT_EQ(*foo.ptr_const, 7);

    foo.ptrbar->val = 42;
    ASSERT_EQ(getBar()->val, 42);
    //foo.ptrbar_const->val = 42;

    (*foo.smart_ptr).val = 99;
    ASSERT_EQ((*foo.smart_ptr).val, 99);

    foo.smart_ptr->val = 100;
    ASSERT_EQ(foo.smart_ptr->val, 100);

    (*foo.smart_ptr_const).val = 99;
    ASSERT_EQ((*foo.smart_ptr_const).val, 99);

    foo.smart_ptr_const->val = 100;
    ASSERT_EQ(foo.smart_ptr_const->val, 100);

    //    //(*foo.csmart_ptr).val = 99;
    //    //foo.csmart_ptr->val = 100;
    //    (*foo.csmart_ptr_const).val = 99;
    //    foo.csmart_ptr_const->val = 100;
}

TEST(IndirectMember, InvokeFuncPtr)
{
    int refVal = 42;
    auto foo = Foo{refVal};

    ASSERT_EQ(foo.func_ptr(), 777);
    ASSERT_EQ(foo.cfunc_ptr(), 777);
}

TEST(IndirectMember, ConstPropagation)
{
    int refVal = 42;
    const auto foo = Foo{refVal};

    //    foo.ref.get() = 100;
    //    *foo.ptr = 99;
    //    foo.cptr = 7;
    //    foo.cptr_const = 7;
    //    *foo.ptr_const = 7;

    //foo.ptrbar->val = 42;
    //foo.ptrbar_const->val = 42;

    //(*foo.smart_ptr).val = 99;
    //foo.smart_ptr->val = 100;
    //(*foo.smart_ptr_const).val = 99;
    //foo.smart_ptr_const->val = 100;

    //    (*foo.csmart_ptr).val = 99;
    //    foo.csmart_ptr->val = 100;
    //    (*foo.csmart_ptr_const).val = 99;
    //    foo.csmart_ptr_const->val = 100;

    ASSERT_EQ(foo.func_ptr(), 777);
    ASSERT_EQ(foo.cfunc_ptr(), 777);
}

TEST(IndirectMember, Constexpr)
{
    struct T {
        indirect_member<int (*)()> func_ptr = testFunc;
    };
    constexpr auto t = T{};
    static_assert(t.func_ptr() == 777);
}

} //namespace