#pragma once

namespace esl
{
namespace error_functions
{

struct noop
{
  constexpr void operator()(const char* msg) const noexcept
  {
    (void)msg;
  }
};

struct halt
{
  void operator()(const char* msg) const noexcept
  {
    (void)msg;
    while (1);
  }
};

} // namespace error_functions
} // namespace esl

