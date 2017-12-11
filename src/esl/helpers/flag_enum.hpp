//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <type_traits>
#include <bitset>
#include <initializer_list>
#include <esl/helpers/unsafe_flag.hpp>
#include <esl/helpers/utils.hpp>

namespace esl
{
//
// A class to view an enum as a set of flags, stored as single bits.
// Used to replace "flags |= some_flag" or "flags &= ~some_flag" type of code
// with a type safe alternative.
//
template <typename Enum>
class flag_enum
{
private:
  // Bitset with the same size as the enum
  unsafe_flag< sizeof(Enum) * 8 > flags_;

public:
  static_assert(std::is_enum< Enum >::value,
                "The specified type is not an enum.");

  // Constructors
  constexpr flag_enum() noexcept : flags_()
  {
  }

  template < typename... Flags >
  constexpr flag_enum(Flags&&... flags) noexcept
  {
    set(flags...);
  }

  // Set specified flags
  template < typename... Flags >
  constexpr void set(Flags&&... flags) noexcept
  {
    // Error checking
    static_assert(
        details::all_true<
            std::is_same< std::decay_t< Flags >, Enum >::value... >::value,
        "Not all parameters are of the specified enum type.");
    static_assert(sizeof...(Flags) > 0, "At least one flag is required.");

    // A way to unroll the input pack of flags
    (void)std::initializer_list< int >{
        ((void)flags_.set(static_cast< std::size_t >(flags)), 0)...
    };
  }

  // Clear all flags
  constexpr void clear() noexcept
  {
    flags_.reset();
  }

  constexpr bool operator[](Enum flag) const noexcept
  {
    return flags_[static_cast< std::size_t >(flag)];
  }

  // Clear specified flags
  template < typename... Flags >
  constexpr void clear(Flags&&... flags) noexcept
  {
    static_assert(
        details::all_true<
            std::is_same< std::decay_t< Flags >, Enum >::value... >::value,
        "Not all parameters are of the specified enum type.");
    static_assert(sizeof...(Flags) > 0, "At least one flag is required.");

    (void)std::initializer_list< int >{
        ((void)flags_.reset(static_cast< std::size_t >(flags)), 0)...
    };
  }

  // Checks if all the specified flags are set
  template < typename... Flags >
  constexpr bool all(Flags&&... flags) const noexcept
  {
    static_assert(
        details::all_true<
            std::is_same< std::decay_t< Flags >, Enum >::value... >::value,
        "Not all parameters are of the specified enum type.");
    static_assert(sizeof...(Flags) > 0, "At least one flag is required.");

    bool ret = true;

    (void)std::initializer_list< int >{
        (ret &= flags_[static_cast< std::size_t >(flags)], 0)...
    };

    return ret;
  }

  // Checks if any of the specified flags are set
  template < typename... Flags >
  constexpr bool any(Flags&&... flags) const noexcept
  {
    static_assert(
        details::all_true<
            std::is_same< std::decay_t< Flags >, Enum >::value... >::value,
        "Not all parameters are of the specified enum type.");
    static_assert(sizeof...(Flags) > 0, "At least one flag is required.");

    bool ret = false;

    (void)std::initializer_list< int >{
        (ret |= flags_[static_cast< std::size_t >(flags)], 0)...
    };

    return ret;
  }

  // Checks if all flags are cleared
  constexpr bool none() const noexcept
  {
    return flags_.none();
  }
};

} // namespace esl
