#!/bin/bash
##          Copyright Emil Fresk 2017-2018.
## Distributed under the Boost Software License, Version 1.0.
##    (See accompanying file LICENSE.md or copy at
##          http://www.boost.org/LICENSE_1_0.txt)

bash ./scripts/run_clangformat.sh
if [[ -n $(git diff --name-status) ]]; then
  echo "Run clang-format before submitting code."
  echo "The script is available under ./scripts/run_clangformat.sh"
  echo ""
  echo "Files not conforming to style:"
  echo ""
  git --no-pager diff --name-status
  echo ""
  echo "Git diff:"
  git --no-pager diff
  exit -1
fi
echo "Style checks passed!"
