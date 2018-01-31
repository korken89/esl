#!/bin/bash
##          Copyright Emil Fresk 2017-2018.
## Distributed under the Boost Software License, Version 1.0.
##    (See accompanying file LICENSE.md or copy at
##          http://www.boost.org/LICENSE_1_0.txt)

cd /usr/src/gtest
cmake . -DBUILD_SHARED_LIBS=ON
make
mv libgtest* /usr/lib/
