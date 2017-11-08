/* sort.cpp */
#include <vector>
#include <algorithm>
#include <iostream>

bool compare(int a, int b)
{
   return (a > b);
}

auto main() -> int
{
   std::cout << "[sort.cpp]" << std::endl;
    
   // Initializing a vector
   std::vector<int> vect = {20,43,11,78,5,96};
    
   // Displaying the original vector
   std::cout << "Original data: ";
   for(auto v:vect)
      std::cout << v << " ";
   std::cout << std::endl;
   
   // Sorting the vector element ascending
   std::sort(std::begin(vect), std::end(vect));
   
   // Displaying the ascending sorted elements
   std::cout << "Ascending sorted: ";
   for(auto v:vect)
      std::cout << v << " ";
   std::cout << std::endl;

   // Sorting the vector element descending
   std::sort(std::begin(vect), std::end(vect), compare);
   
   // Displaying the descending sorted elements
   std::cout << "Descending sorted: ";
   for(auto v:vect)
      std::cout << v << " ";
   std::cout << std::endl;
   
   return 0;
}