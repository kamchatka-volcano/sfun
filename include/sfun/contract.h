#ifndef STUFF_FROM_UNNAMED_NAMESPACE_CONTRACT_H
#define STUFF_FROM_UNNAMED_NAMESPACE_CONTRACT_H

///
///Renamed part of Microsoft.GSL assert header
///This header shouldn't contain any functions to support safe bundling with other libraries.
///
#if defined(__clang__) || defined(__GNUC__)
#define _sfun_likely(x) __builtin_expect(!!(x), 1)
#else
#define _sfunLikely(x) (!!(x))
#endif //defined(__clang__) || defined(__GNUC__)

#define sfun_contract_check(cond) (_sfun_likely(cond) ? static_cast<void>(0) : std::terminate())

#define sfun_precondition sfun_contract_check
#define sfun_postcondition sfun_contract_check
#define sfun_invariant sfun_contract_check

#endif //STUFF_FROM_UNNAMED_NAMESPACE_CONTRACT_H