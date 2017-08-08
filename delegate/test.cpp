
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

using Callback = esl::delegate< int(int) >;
volatile int asd = 9;

int main()
{
  test a;

  auto d1 = Callback::from_method<test, &test::meth>(a);
  auto d2 = Callback::from_function<&wee>();
  auto d3 = Callback::from_method<test, &test::meth2>(a);

  std::cout << d1(4) + d2(7) + d3(asd) << '\n';

  return 0;
}
