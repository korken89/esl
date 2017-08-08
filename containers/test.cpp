
#include <iostream>
#include <algorithm>
#include "static_vector.hpp"
#include "ring_buffer.hpp"

esl::static_vector_debug< int, 5 > vec;
int i = 11;

esl::ring_buffer_debug< int, 5 > buf;

void test()
{
  using std::cout;

  buf.push_back(1);
  buf.push_back(2);
  buf.push_back(3);
  buf.push_back(i);

  cout << "front: " << buf.front() << '\n';
  cout << "back: " << buf.back() << '\n';
  cout << "size: " << buf.size() << '\n';
  buf.pop();

  cout << "front: " << buf.front() << '\n';
  cout << "back: " << buf.back() << '\n';
  cout << "size: " << buf.size() << '\n';
  buf.pop();

  cout << "front: " << buf.front() << '\n';
  cout << "back: " << buf.back() << '\n';
  cout << "size: " << buf.size() << '\n';
  buf.pop();

  cout << "front: " << buf.front() << '\n';
  cout << "back: " << buf.back() << '\n';
  cout << "size: " << buf.size() << '\n';
  buf.pop();

  constexpr const int a[3] = {5, 6, 7};
  buf.push_back(a);

  cout << "front: " << buf.front() << '\n';
  cout << "back: " << buf.back() << '\n';
  cout << "size: " << buf.size() << '\n';
}

int main()
{
  // vec.front();
  // vec[5] = 10;
  vec.push_back(19);
  vec.push_back(13);
  vec.emplace_back(18);
  vec.push_back(i + 1);
  vec.push_back(i);

  std::sort(vec.begin(), vec.end());

  std::cout << "static_vector:" << '\n';
  // int sum = 0;
  for (auto v : vec)
    std::cout << v << '\n';

  std::cout << "\nring_buffer:\n";
  test();

  return 0;
}
