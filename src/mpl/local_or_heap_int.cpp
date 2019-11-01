#include <cstdio>
#include <iostream>

#include "local_or_heap_T.hpp"

int main()
{
  LOH<int, 1> heapInt(0);
  std::cout << *heapInt.get() << '\n'; // 0
  *heapInt.get() = 2;
  std::cout << *heapInt.get() << '\n'; // 2

  LOH<int, sizeof(int)> localInt(42);
  std::cout << *localInt.get() << '\n'; // 42
}
