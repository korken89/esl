//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace esl
{
namespace details
{
// Meta function to check if all bools in a pack is true
template< bool... > struct bool_pack;

template< bool... Bools >
using all_true = std::is_same<
                    bool_pack< Bools..., true >,
                    bool_pack< true, Bools... >
                 >;

} // namespace details
} // namespace esl
