/* begin_end.cpp */
#include <iostream>

auto main() -> int
{
   std::cout << "[begin_end.cpp]" << std::endl;

   // Declaring an array
   int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

   // using begin() and end()
   std::cout << "Displaying array using begin() and end()" << std::endl;
   for(auto i = std::begin(arr); i != std::end(arr); ++i)
      std::cout << *i << " ";
   std::cout << std::endl;

   return 0;
}
