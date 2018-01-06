//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>
#include <cstdint>
#include <memory>
#include <utility>
#include <limits>

namespace esl
{
namespace details
{
// Meta function to check if all bools in a pack is true
template < bool... >
struct bool_pack;

template < bool... Bools >
using all_true =
    std::is_same< bool_pack< Bools..., true >, bool_pack< true, Bools... > >;

//
// Simple compile-time repeat implementation with index
//
template < std::size_t... Is, typename F >
constexpr void repeat_impl(std::index_sequence< Is... >, F&& fun) noexcept(
    noexcept((void)std::initializer_list< std::size_t >{(fun(Is), Is)...}))
{
  (void)std::initializer_list< std::size_t >{(fun(Is), Is)...};
}

}  // namespace details

//
// Simple compile-time repeat implementation with index
//
template < std::size_t N, typename F >
constexpr void repeat(F&& fun) noexcept(noexcept(details::repeat_impl(
    std::make_index_sequence< N >{}, std::forward< F >(fun))))
{
  details::repeat_impl(std::make_index_sequence< N >{}, std::forward< F >(fun));
}

//
// Checkers to get the smallest type that can hold the number
//

// Unsigned integers
template < std::uint64_t N >
using uint_least_t = std::conditional_t<
    (N > std::numeric_limits< std::uint32_t >::max()), std::uint64_t,
    std::conditional_t<
        (N > std::numeric_limits< std::uint16_t >::max()), std::uint32_t,
        std::conditional_t< (N > std::numeric_limits< std::uint8_t >::max()),
                            std::uint16_t, std::uint8_t > > >;
// Signed integers
template < std::uint64_t N >
using int_least_t = std::conditional_t<
    (N > std::numeric_limits< std::int32_t >::max()), std::int64_t,
    std::conditional_t<
        (N > std::numeric_limits< std::int16_t >::max()), std::int32_t,
        std::conditional_t< (N > std::numeric_limits< std::int8_t >::max()),
                            std::int16_t, std::int8_t > > >;

}  // namespace esl
