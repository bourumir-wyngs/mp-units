// The MIT License (MIT)
//
// Copyright (c) 2018 Mateusz Pusz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <version>

#if __clang__
#define MP_UNITS_COMP_CLANG __clang_major__
#elif __GNUC__
#define MP_UNITS_COMP_GCC __GNUC__
#define MP_UNITS_COMP_GCC_MINOR __GNUC_MINOR__
#elif _MSC_VER
#define MP_UNITS_COMP_MSVC _MSC_VER
#endif

// Adapted from https://github.com/ericniebler/range-v3/blob/master/include/range/v3/detail/config.hpp#L185.
#define MP_UNITS_PRAGMA(X) _Pragma(#X)
#if !MP_UNITS_COMP_MSVC
#define MP_UNITS_DIAGNOSTIC_PUSH MP_UNITS_PRAGMA(GCC diagnostic push)
#define MP_UNITS_DIAGNOSTIC_POP MP_UNITS_PRAGMA(GCC diagnostic pop)
#define MP_UNITS_DIAGNOSTIC_IGNORE_PRAGMAS MP_UNITS_PRAGMA(GCC diagnostic ignored "-Wpragmas")
#define MP_UNITS_DIAGNOSTIC_IGNORE(X)                                \
  MP_UNITS_DIAGNOSTIC_IGNORE_PRAGMAS                                 \
  MP_UNITS_PRAGMA(GCC diagnostic ignored "-Wunknown-pragmas")        \
  MP_UNITS_PRAGMA(GCC diagnostic ignored "-Wunknown-warning-option") \
  MP_UNITS_PRAGMA(GCC diagnostic ignored X)
#define MP_UNITS_DIAGNOSTIC_IGNORE_EXPR_ALWAYS_TF
#define MP_UNITS_DIAGNOSTIC_IGNORE_FLOAT_EQUAL MP_UNITS_DIAGNOSTIC_IGNORE("-Wfloat-equal")
#define MP_UNITS_DIAGNOSTIC_IGNORE_LOSS_OF_DATA
#define MP_UNITS_DIAGNOSTIC_IGNORE_MISSING_BRACES MP_UNITS_DIAGNOSTIC_IGNORE("-Wmissing-braces")
#define MP_UNITS_DIAGNOSTIC_IGNORE_NON_TEMPLATE_FRIEND MP_UNITS_DIAGNOSTIC_IGNORE("-Wnon-template-friend")
#define MP_UNITS_DIAGNOSTIC_IGNORE_SHADOW MP_UNITS_DIAGNOSTIC_IGNORE("-Wshadow")
#define MP_UNITS_DIAGNOSTIC_IGNORE_UNREACHABLE
#define MP_UNITS_DIAGNOSTIC_IGNORE_ZERO_AS_NULLPOINTER_CONSTANT \
  MP_UNITS_DIAGNOSTIC_IGNORE("-Wzero-as-nullpointer-constant")
#define MP_UNITS_DIAGNOSTIC_IGNORE_DEPRECATED MP_UNITS_DIAGNOSTIC_IGNORE("-Wdeprecated-declarations")
#else
#define MP_UNITS_DIAGNOSTIC_PUSH MP_UNITS_PRAGMA(warning(push))
#define MP_UNITS_DIAGNOSTIC_POP MP_UNITS_PRAGMA(warning(pop))
#define MP_UNITS_DIAGNOSTIC_IGNORE_PRAGMAS MP_UNITS_PRAGMA(warning(disable : 4068))
#define MP_UNITS_DIAGNOSTIC_IGNORE(X) MP_UNITS_DIAGNOSTIC_IGNORE_PRAGMAS MP_UNITS_PRAGMA(warning(disable : X))
#define MP_UNITS_DIAGNOSTIC_IGNORE_EXPR_ALWAYS_TF MP_UNITS_DIAGNOSTIC_IGNORE(4296)
#define MP_UNITS_DIAGNOSTIC_IGNORE_FLOAT_EQUAL
#define MP_UNITS_DIAGNOSTIC_IGNORE_LOSS_OF_DATA MP_UNITS_DIAGNOSTIC_IGNORE(4244)
#define MP_UNITS_DIAGNOSTIC_IGNORE_MISSING_BRACES
#define MP_UNITS_DIAGNOSTIC_IGNORE_NON_TEMPLATE_FRIEND
#define MP_UNITS_DIAGNOSTIC_IGNORE_SHADOW MP_UNITS_DIAGNOSTIC_IGNORE(4459)
#define MP_UNITS_DIAGNOSTIC_IGNORE_UNREACHABLE MP_UNITS_DIAGNOSTIC_IGNORE(4702)
#define MP_UNITS_DIAGNOSTIC_IGNORE_ZERO_AS_NULLPOINTER_CONSTANT
#define MP_UNITS_DIAGNOSTIC_IGNORE_DEPRECATED
#endif

#if _LIBCPP_VERSION
#define MP_UNITS_LIBCXX _LIBCPP_VERSION
#endif

#if MP_UNITS_LIBCXX

#if MP_UNITS_LIBCXX < 13000

#include <concepts/concepts.hpp>
#include <range/v3/algorithm/lower_bound.hpp>
#include <range/v3/algorithm/transform.hpp>
#include <range/v3/functional/comparisons.hpp>
#include <range/v3/iterator.hpp>
#include <range/v3/iterator/concepts.hpp>
#include <range/v3/range/concepts.hpp>

#elif MP_UNITS_LIBCXX < 14000

#include <range/v3/functional/comparisons.hpp>

#endif

#endif

#include <compare>
#include <concepts>

#if MP_UNITS_COMP_MSVC || (MP_UNITS_COMP_CLANG && MP_UNITS_COMP_CLANG < 16)

#define MP_UNITS_TYPENAME typename

#else

#define MP_UNITS_TYPENAME

#endif

#if MP_UNITS_COMP_CLANG

#define MP_UNITS_CONSTEVAL constexpr

#else

#define MP_UNITS_CONSTEVAL consteval

#endif

#if MP_UNITS_COMP_MSVC

#define MP_UNITS_CONSTRAINED_AUTO_WORKAROUND(X)

#else

#define MP_UNITS_CONSTRAINED_AUTO_WORKAROUND(X) X

#endif

#if MP_UNITS_COMP_MSVC || (MP_UNITS_COMP_GCC && MP_UNITS_COMP_GCC < 11)

#define MP_UNITS_CONSTRAINED_NTTP_WORKAROUND(X)

#else

#define MP_UNITS_CONSTRAINED_NTTP_WORKAROUND(X) X

#endif


namespace std {

#if MP_UNITS_COMP_GCC

template<class T>
concept default_constructible = constructible_from<T>;

#elif MP_UNITS_LIBCXX

#if MP_UNITS_LIBCXX < 13000

// concepts
using concepts::common_with;
using concepts::constructible_from;
using concepts::convertible_to;
using concepts::copy_constructible;
using concepts::derived_from;
using concepts::equality_comparable;
using concepts::equality_comparable_with;
using concepts::integral;
using concepts::move_constructible;
using concepts::regular;
using concepts::three_way_comparable;
using concepts::three_way_comparable_with;
using concepts::totally_ordered;

using ranges::compare_three_way;

using ranges::input_iterator;
using ranges::iter_value_t;
using ranges::sentinel_for;

namespace ranges {

using ::ranges::begin;
using ::ranges::distance;
using ::ranges::end;

using ::ranges::input_range;
using ::ranges::range_value_t;

using ::ranges::lower_bound;
using ::ranges::transform;

}  // namespace ranges

// missing in Range-v3
template<class T>
concept floating_point = std::is_floating_point_v<T>;

template<class T>
concept default_initializable =
  std::constructible_from<T> && requires { T{}; } && requires { ::new (static_cast<void*>(nullptr)) T; };

template<class F, class... Args>
concept invocable = requires(F&& f, Args&&... args) { std::invoke(std::forward<F>(f), std::forward<Args>(args)...); };

template<class F, class... Args>
concept regular_invocable = invocable<F, Args...>;

template<class T, class U>
constexpr bool cmp_equal(T t, U u) noexcept
{
  using UT = std::make_unsigned_t<T>;
  using UU = std::make_unsigned_t<U>;
  if constexpr (std::is_signed_v<T> == std::is_signed_v<U>)
    return t == u;
  else if constexpr (std::is_signed_v<T>)
    return t < 0 ? false : UT(t) == u;
  else
    return u < 0 ? false : t == UU(u);
}

template<class T, class U>
constexpr bool cmp_not_equal(T t, U u) noexcept
{
  return !cmp_equal(t, u);
}

template<class T, class U>
constexpr bool cmp_less(T t, U u) noexcept
{
  using UT = std::make_unsigned_t<T>;
  using UU = std::make_unsigned_t<U>;
  if constexpr (std::is_signed_v<T> == std::is_signed_v<U>)
    return t < u;
  else if constexpr (std::is_signed_v<T>)
    return t < 0 ? true : UT(t) < u;
  else
    return u < 0 ? false : t < UU(u);
}

template<class T, class U>
constexpr bool cmp_greater(T t, U u) noexcept
{
  return cmp_less(u, t);
}

template<class T, class U>
constexpr bool cmp_less_equal(T t, U u) noexcept
{
  return !cmp_greater(t, u);
}

template<class T, class U>
constexpr bool cmp_greater_equal(T t, U u) noexcept
{
  return !cmp_less(t, u);
}

template<class R, class T>
constexpr bool in_range(T t) noexcept
{
  return std::cmp_greater_equal(t, std::numeric_limits<R>::min()) &&
         std::cmp_less_equal(t, std::numeric_limits<R>::max());
}

#elif MP_UNITS_LIBCXX < 14000

using concepts::three_way_comparable;
using concepts::three_way_comparable_with;
using ::ranges::compare_three_way;

#endif

#endif

}  // namespace std
