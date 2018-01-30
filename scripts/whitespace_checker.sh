#!/bin/bash
##          Copyright Emil Fresk 2017-2018.
## Distributed under the Boost Software License, Version 1.0.
##    (See accompanying file LICENSE.md or copy at
##          http://www.boost.org/LICENSE_1_0.txt)

if [[ -n $(git diff-tree --check $(git hash-object -t tree /dev/null) HEAD) ]]; then
    echo "Remove whitespaces before submitting code."
    echo ""
    echo "Git check results:"
    echo ""
    git diff-tree --check $(git hash-object -t tree /dev/null) HEAD
    exit -1
fi
