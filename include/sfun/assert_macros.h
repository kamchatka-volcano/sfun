#ifndef STUFF_FROM_UNNAMED_NAMESPACE_ASSERT_MACROS_H
#define STUFF_FROM_UNNAMED_NAMESPACE_ASSERT_MACROS_H

///
///Renamed part of Microsoft.GSL assert header
///This header shouldn't contain any functions to support safe bundling with other libraries.
///
#if defined(__clang__) || defined(__GNUC__)
#define sfunLikely(x) __builtin_expect(!!(x), 1)
#define sfunUnlikely(x) __builtin_expect(!!(x), 0)
#else
#define sfunLikely(x) (!!(x))
#define sfunUnlikely(x) (!!(x))
#endif // defined(__clang__) || defined(__GNUC__)

#define sfunPrecondition(cond) \
    (sfunLikely(cond) ? static_cast<void>(0) : std::terminate())
#define sfunPostcondition(cond) \
    (sfunLikely(cond) ? static_cast<void>(0) : std::terminate())
#define sfunInvariant(cond) \
    (sfunLikely(cond) ? static_cast<void>(0) : std::terminate())

#endif //STUFF_FROM_UNNAMED_NAMESPACE_ASSERT_MACROS_H