#ifndef STUFF_FROM_UNNAMED_NAMESPACE_CONTRACT_H
#define STUFF_FROM_UNNAMED_NAMESPACE_CONTRACT_H

///
///Renamed part of Microsoft.GSL assert header
///This header shouldn't contain any functions to support safe bundling with other libraries.
///
#if defined(__clang__) || defined(__GNUC__)
#define _sfunLikely(x) __builtin_expect(!!(x), 1)
#else
#define _sfunLikely(x) (!!(x))
#endif // defined(__clang__) || defined(__GNUC__)

#define sfunContractCheck(cond) \
    (_sfunLikely(cond) ? static_cast<void>(0) : std::terminate())

#define sfunPrecondition sfunContractCheck
#define sfunPostcondition sfunContractCheck
#define sfunInvariant sfunContractCheck

#endif //STUFF_FROM_UNNAMED_NAMESPACE_CONTRACT_H