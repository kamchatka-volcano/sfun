#include <sfun/optional_ref.h>
#include <sfun/utility.h>
#include <gtest/gtest.h>
#include <functional>
#include <optional>
#include <string>
#include <vector>

std::string toString(sfun::optional_ref<int> num)
{
    if (!num.has_value())
        return "empty";
    return std::to_string(num.value());
}

template<int Num>
constexpr int testConstexprOptRef()
{
    auto num = Num;
    auto oNum = sfun::optional_ref<const int>{num};
    return oNum.value();
}

template<int Num>
std::string toString()
{
    return std::to_string(Num);
}

struct Foo {
    int num = 42;
};

struct TestObject {
    sfun::member<sfun::optional_ref<int>> oRef;
};

struct FooObject {
    sfun::member<sfun::optional_ref<Foo>> oRef;
};

TEST(OptionalRef, Constructor)
{
    auto foo = 42;
    auto oRef = sfun::optional_ref<int>{foo};
    ASSERT_TRUE(oRef.has_value());
    ASSERT_EQ(oRef.value(), 42);

    foo = 99;
    ASSERT_EQ(oRef.value(), 99);

    oRef.value() = 7;
    ASSERT_EQ(foo, 7);
}

TEST(OptionalRef, PointerConstructor)
{
    auto foo = 42;
    auto oRef = sfun::optional_ref<int>{&foo};
    ASSERT_TRUE(oRef.has_value());
    ASSERT_EQ(oRef.value(), 42);

    foo = 99;
    ASSERT_EQ(oRef.value(), 99);

    oRef.value() = 7;
    ASSERT_EQ(foo, 7);
}

TEST(OptionalRef, NulloptConstructor)
{
    auto oRef = sfun::optional_ref<int>{std::nullopt};
    ASSERT_FALSE(oRef.has_value());
}

TEST(OptionalRef, CopyConstructor)
{
    auto foo = 42;
    auto oRef = sfun::optional_ref<int>{foo};
    auto oRef2 = oRef;
    ASSERT_TRUE(oRef2.has_value());
    ASSERT_EQ(oRef2.value(), 42);

    foo = 99;
    ASSERT_EQ(oRef2.value(), 99);

    oRef2.value() = 7;
    ASSERT_EQ(foo, 7);
}

TEST(OptionalRef, MoveConstructor)
{
    auto foo = 42;
    auto oRef = sfun::optional_ref<int>{foo};
    auto oRef2 = std::move(oRef);
    ASSERT_TRUE(oRef2.has_value());
    ASSERT_EQ(oRef2.value(), 42);

    foo = 99;
    ASSERT_EQ(oRef2.value(), 99);

    oRef2.value() = 7;
    ASSERT_EQ(foo, 7);
}

TEST(OptionalRef, Emplace)
{
    auto foo = 42;
    auto bar = 99;
    auto oRef = sfun::optional_ref<int>{};
    ASSERT_FALSE(oRef.has_value());

    oRef.emplace(foo);
    ASSERT_TRUE(oRef.has_value());
    ASSERT_EQ(oRef.value(), 42);

    foo = 99;
    ASSERT_EQ(oRef.value(), 99);

    oRef.value() = 7;
    ASSERT_EQ(foo, 7);

    oRef.emplace(bar);
    ASSERT_EQ(oRef.value(), 99);
}

TEST(OptionalRef, EmplacePointer)
{
    auto foo = 42;
    auto bar = 99;
    auto oRef = sfun::optional_ref<int>{};
    ASSERT_FALSE(oRef.has_value());

    oRef.emplace(&foo);
    ASSERT_TRUE(oRef.has_value());
    ASSERT_EQ(oRef.value(), 42);

    foo = 99;
    ASSERT_EQ(oRef.value(), 99);

    oRef.value() = 7;
    ASSERT_EQ(foo, 7);

    oRef.emplace(&bar);
    ASSERT_EQ(oRef.value(), 99);
}

TEST(OptionalRef, MemberAccess)
{
    auto foo = Foo{};
    auto oRef = sfun::optional_ref<Foo>{foo};
    ASSERT_TRUE(oRef.has_value());
    ASSERT_EQ(oRef->num, 42);
}

TEST(OptionalRef, Const)
{
    auto foo = 42;
    auto oRef = sfun::optional_ref<const int>{foo};
    ASSERT_TRUE(oRef.has_value());
    ASSERT_EQ(oRef.value(), 42);

    auto oRef2 = oRef;
    ASSERT_EQ(oRef2.value(), 42);
}

TEST(OptionalRef, AsFunctionArg)
{
    ASSERT_EQ("empty", toString({}));
    ASSERT_EQ("empty", toString(std::nullopt));
    auto num = 42;
    ASSERT_EQ("42", toString(num));
}

TEST(OptionalRefWrapper, InVector)
{

    auto vec = std::vector<sfun::optional_ref_wrapper<int>>{};
    auto foo = 42;
    auto bar = 99;
    auto oRefFoo = sfun::optional_ref<int>{foo};
    auto oRefBar = sfun::optional_ref<int>{bar};

    vec.insert(vec.begin(), oRefFoo);
    vec.insert(vec.end(), oRefBar);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec.at(0).get().value(), 42);
    ASSERT_EQ(vec.at(1).get().value(), 99);
}

TEST(OptionalRefWrapper, InVectorWithConstRef)
{
    auto vec = std::vector<sfun::optional_ref_wrapper<const int>>{};
    auto foo = 42;
    auto bar = 99;
    const auto oRefFoo = sfun::optional_ref<const int>{foo};
    const auto oRefBar = sfun::optional_ref<const int>{bar};

    vec.insert(vec.begin(), oRefFoo);
    vec.insert(vec.end(), oRefBar);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec.at(0).get().value(), 42);
    ASSERT_EQ(vec.at(1).get().value(), 99);
}

TEST(OptionalRefMember, Modify)
{
    auto foo = 42;
    auto obj = TestObject{foo};
    auto obj2 = TestObject{};
    ASSERT_TRUE(obj.oRef.get().has_value());
    ASSERT_TRUE(obj.oRef.get());
    ASSERT_TRUE(obj.oRef);

    obj.oRef.get().emplace(foo);
    ASSERT_TRUE(obj.oRef);

    ASSERT_TRUE(obj.oRef.get().has_value());
    ASSERT_EQ(obj.oRef.get().value(), 42);

    foo = 99;
    ASSERT_EQ(obj.oRef.get().value(), 99);

    obj.oRef.get().value() = 7;
    ASSERT_EQ(foo, 7);

    obj2 = obj;
    ASSERT_EQ(obj2.oRef.get().value(), 7);
}

TEST(OptionalRefMember, Dereference)
{
    auto foo = 42;
    auto obj = TestObject{foo};
    ASSERT_TRUE(obj.oRef);
    ASSERT_EQ(*obj.oRef, 42);

    foo = 99;
    ASSERT_EQ(*obj.oRef, 99);

    *obj.oRef = 7;
    ASSERT_EQ(foo, 7);
}

TEST(OptionalRefMember, MemberAccess)
{
    auto foo = Foo{};
    auto obj = FooObject{foo};
    ASSERT_TRUE(obj.oRef);

    ASSERT_EQ(obj.oRef->num, 42);

    foo.num = 99;
    ASSERT_EQ(obj.oRef->num, 99);

    obj.oRef->num = 7;
    ASSERT_EQ(foo.num, 7);
}

TEST(OptionalRef, Constexpr)
{
    ASSERT_EQ("42", toString<testConstexprOptRef<42>()>());
}
