#include <sfun/optional_ref.h>
#include <sfun/utility.h>
#include <gtest/gtest.h>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace {

std::string toString(sfun::optional_cpref<int> num)
{
    if (!num.has_value())
        return "empty";
    return std::to_string(num.value());
}

template<int Num>
constexpr int testConstexprOptRef()
{
    auto num = Num;
    auto oNum = sfun::optional_cpref<const int>{num};
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
    sfun::member<sfun::optional_cpref<int>> oRef;
};

struct FooObject {
    sfun::member<sfun::optional_cpref<Foo>> oRef;
};

TEST(OptionalConstPropagatedRef, Constructor)
{
    auto foo = 42;
    auto oRef = sfun::optional_cpref<int>{foo};
    ASSERT_TRUE(oRef.has_value());
    ASSERT_EQ(oRef.value(), 42);

    foo = 99;
    ASSERT_EQ(oRef.value(), 99);

    oRef.value() = 7;
    ASSERT_EQ(foo, 7);
}

TEST(OptionalConstPropagatedRef, PointerConstructor)
{
    auto foo = 42;
    auto oRef = sfun::optional_cpref<int>{&foo};
    ASSERT_TRUE(oRef.has_value());
    ASSERT_EQ(oRef.value(), 42);

    foo = 99;
    ASSERT_EQ(oRef.value(), 99);

    oRef.value() = 7;
    ASSERT_EQ(foo, 7);
}

TEST(OptionalConstPropagatedRef, NulloptConstructor)
{
    auto oRef = sfun::optional_cpref<int>{std::nullopt};
    ASSERT_FALSE(oRef.has_value());
}

TEST(OptionalConstPropagatedRef, CopyConstructor)
{
    auto foo = 42;
    auto oRef = sfun::optional_cpref<int>{foo};
    auto oRef2 = oRef;
    ASSERT_TRUE(oRef2.has_value());
    ASSERT_EQ(oRef2.value(), 42);

    foo = 99;
    ASSERT_EQ(oRef2.value(), 99);

    oRef2.value() = 7;
    ASSERT_EQ(foo, 7);
}

TEST(OptionalConstPropagatedRef, MoveConstructor)
{
    auto foo = 42;
    auto oRef = sfun::optional_cpref<int>{foo};
    auto oRef2 = std::move(oRef);
    ASSERT_TRUE(oRef2.has_value());
    ASSERT_EQ(oRef2.value(), 42);

    foo = 99;
    ASSERT_EQ(oRef2.value(), 99);

    oRef2.value() = 7;
    ASSERT_EQ(foo, 7);
}

TEST(OptionalConstPropagatedRef, Emplace)
{
    auto foo = 42;
    auto bar = 99;
    auto oRef = sfun::optional_cpref<int>{};
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

TEST(OptionalConstPropagatedRef, EmplacePointer)
{
    auto foo = 42;
    auto bar = 99;
    auto oRef = sfun::optional_cpref<int>{};
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

TEST(OptionalConstPropagatedRef, MemberAccess)
{
    auto foo = Foo{};
    auto oRef = sfun::optional_cpref<Foo>{foo};
    ASSERT_TRUE(oRef.has_value());
    ASSERT_EQ(oRef->num, 42);
}

TEST(OptionalConstPropagatedRef, Const)
{
    auto foo = 42;
    auto oRef = sfun::optional_cpref<const int>{foo};
    ASSERT_TRUE(oRef.has_value());
    ASSERT_EQ(oRef.value(), 42);

    auto oRef2 = oRef;
    ASSERT_EQ(oRef2.value(), 42);
}

TEST(OptionalConstPropagatedRef, AsFunctionArg)
{
    ASSERT_EQ("empty", toString({}));
    ASSERT_EQ("empty", toString(std::nullopt));
    auto num = 42;
    ASSERT_EQ("42", toString(num));
}

TEST(OptionalConstPropagatedRefWrapper, InVector)
{

    auto vec = std::vector<sfun::optional_cpref_wrapper<int>>{};
    auto foo = 42;
    auto bar = 99;
    auto oRefFoo = sfun::optional_cpref<int>{foo};
    auto oRefBar = sfun::optional_cpref<int>{bar};

    vec.insert(vec.begin(), oRefFoo);
    vec.insert(vec.end(), oRefBar);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec.at(0).get().value(), 42);
    ASSERT_EQ(vec.at(1).get().value(), 99);
}

TEST(OptionalConstPropagatedRefWrapper, InVectorWithConstRef)
{
    auto vec = std::vector<sfun::optional_cpref_wrapper<const int>>{};
    auto foo = 42;
    auto bar = 99;
    const auto oRefFoo = sfun::optional_cpref<const int>{foo};
    const auto oRefBar = sfun::optional_cpref<const int>{bar};

    vec.insert(vec.begin(), oRefFoo);
    vec.insert(vec.end(), oRefBar);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec.at(0).get().value(), 42);
    ASSERT_EQ(vec.at(1).get().value(), 99);
}

TEST(OptionalConstPropagatedRefMember, Modify)
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

TEST(OptionalConstPropagatedRefMember, Dereference)
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

TEST(OptionalConstPropagatedRefMember, ConstDereference)
{
    auto foo = 42;
    const auto obj = TestObject{foo};
    ASSERT_TRUE(obj.oRef);
    ASSERT_EQ(*obj.oRef, 42);

    foo = 99;
    ASSERT_EQ(*obj.oRef, 99);

    //*obj.oRef = 7;
    //ASSERT_EQ(foo, 7);
}

TEST(OptionalConstPropagatedRefMember, MemberAccess)
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

TEST(OptionalConstPropagatedRefMember, ConstMemberAccess)
{
    auto foo = Foo{};
    const auto obj = FooObject{foo};
    ASSERT_TRUE(obj.oRef);

    ASSERT_EQ(obj.oRef->num, 42);

    foo.num = 99;
    ASSERT_EQ(obj.oRef->num, 99);

    //obj.oRef->num = 7;
    //ASSERT_EQ(foo.num, 7);
}

TEST(OptionalConstPropagatedRef, Constexpr)
{
    ASSERT_EQ("42", toString<testConstexprOptRef<42>()>());
}

} //namespace