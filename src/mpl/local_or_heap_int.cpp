#include <cstdio>
#include <iostream>

#include "small_T_opt.hpp"

int main()
{
  SOO<int, 1> heapInt(0);
  std::cout << *heapInt.get() << '\n'; // 0
  *heapInt.get() = 2;
  std::cout << *heapInt.get() << '\n'; // 2

  SOO<int, sizeof(int)> localInt(42);
  std::cout << *localInt.get() << '\n'; // 42
}
