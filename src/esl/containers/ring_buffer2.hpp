#pragma once

#include <atomic>
#include <cstdint>
#include <cstring>

template < typename T, unsigned N >
class ring_buffer
{
  std::atomic< unsigned > head_idx_{0};
  std::atomic< unsigned > tail_idx_{0};

  T buffer_[N];

  template < typename V >
  auto increment(V val, V inc = V(1))
  {
    return (val + inc) % N;
  }

  constexpr auto size(const unsigned current_head,
                      const unsigned current_tail) const noexcept
  {
    return (current_head - current_tail + N) % N;
  }

public:
  constexpr auto size() const noexcept
  {
    const auto current_head = head_idx_.load(std::memory_order_relaxed);
    const auto current_tail = tail_idx_.load(std::memory_order_relaxed);

    return (current_head - current_tail + N) % N;
  }

  constexpr auto capacity() const noexcept
  {
    return N - 1;
  }

  bool push(T item)
  {
    const auto current_head = head_idx_.load(std::memory_order_relaxed);
    const auto next_head = increment(current_head);

    if (next_head != tail_idx_.load(std::memory_order_acquire))
    {
      buffer_[current_head] = item;
      head_idx_.store(next_head, std::memory_order_release);

      return true;
    }

    return false;
  }

  bool push(const T *items, unsigned num)
  {
    const auto current_head = head_idx_.load(std::memory_order_acquire);
    const auto current_tail = tail_idx_.load(std::memory_order_relaxed);

    if (num == 0 || capacity() - size(current_head, current_tail) < num)
      return false;

    const auto space_left_head = N - head_idx_;

    if (space_left_head >= num)
    {
      // All will fit without the head_idx_ overflowing
      // std::memcpy(&buffer_[current_head], items, num * sizeof(T));

      for (unsigned i = 0; i < num; i++)
        buffer_[current_head + i] = items[i];

      // head_idx_ += num;
      head_idx_.store(current_head + num, std::memory_order_release);
    }
    else
    {
      // The head_idx_ will overflow, write in 2 steps
      // std::memcpy(&buffer_[current_head], items, space_left_head *
      // sizeof(T));

      for (unsigned i = 0; i < space_left_head; i++)
        buffer_[current_head + i] = items[i];

      num -= space_left_head;

      // std::memcpy(&buffer_[0], (items + space_left_head), num * sizeof(T));
      for (unsigned i = 0; i < num; i++)
        buffer_[i] = items[space_left_head + i];

      head_idx_.store(num, std::memory_order_release);
    }

    return true;
  }

  bool pop(T &item)
  {
    const auto current_tail = tail_idx_.load(std::memory_order_relaxed);

    if (current_tail == head_idx_.load(std::memory_order_acquire))
      return false;

    item = buffer_[current_tail];
    tail_idx_.store(increment(current_tail), std::memory_order_release);

    return true;
  }

  unsigned pop_chunk(T *destination, unsigned num)
  {
    const auto current_head = head_idx_.load(std::memory_order_relaxed);
    const auto current_tail = tail_idx_.load(std::memory_order_acquire);

    if (num == 0)
      return 0;  // No data read

    if (current_tail == current_head)
      return 0;  // No data read

    const auto num_elems = capacity() - size(current_head, current_tail);
    if (num > num_elems)
      num = num_elems;  // Limit the number of elements to be read

    const auto space_left_tail = N - tail_idx_;

    if (space_left_tail >= num)
    {
      // All will read without the tail_idx_ overflowing
      for (unsigned i = 0; i < num; i++)
        destination[i] = buffer_[current_tail + i];

      tail_idx_.store(current_tail + num, std::memory_order_release);
    }
    else
    {
      // The tail_idx_ will overflow, read in 2 steps
      for (unsigned i = 0; i < space_left_tail; i++)
        destination[i] = buffer_[current_tail + i];

      const auto num_left = num - space_left_tail;

      for (unsigned i = 0; i < num_left; i++)
        destination[space_left_tail + i] = buffer_[i];

      head_idx_.store(num_left, std::memory_order_release);
    }

    return num;
  }
};
