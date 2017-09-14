//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include "delegate.hpp"

struct test
{
  int meth(int i)
  {
    return i*i;
  }

  int meth2(int i) const
  {
    return i*i;
  }
};

int wee(int i)
{
  return i*i;
}

using callback = esl::delegate< int(int) >;
volatile int asd = 9;

int main()
{
  test a;

  auto d1 = callback::from<test, &test::meth>(a);
  auto d2 = callback::from<wee>();
  auto d3 = callback::from<test, &test::meth2>(a);

  callback c;

  // c = d1;
  if (c.valid())
    std::cout << d1(4) + d2(7) + d3(asd) + c(2999) << '\n';
  else
    std::cout << d1(4) + d2(7) + d3(asd) << '\n';


  return 0;
}
