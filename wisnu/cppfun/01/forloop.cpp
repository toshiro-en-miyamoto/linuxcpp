/* forloop.cpp */
#include <iostream>

auto main() -> int
{
   std::cout << "[forllop.cpp]" << std::endl;

   // Declaring an array
   int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

   // using range-based for loops
   std::cout << "Displaying array using range-based for loop" << std::endl;
   for(auto a:arr)
      std::cout << a << " ";
   std::cout << std::endl;

   return 0;
}
