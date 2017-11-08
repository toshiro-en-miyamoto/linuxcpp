/* vector.cpp */
#include <vector>
#include <iostream>

auto main() -> int
{
   std::cout << "[vector.cpp]" << std::endl;
   
   // Initializing a vector containing three integer
   std::vector<int> vect = {0,1,2};
   
   // Displaying the original elements
   std::cout << "Original data: ";
   for(auto v:vect)
      std::cout << v << " ";
   std::cout << std::endl;
   
   // Adding two new data
   vect.push_back(3);
   vect.push_back(4);
   
   // Displaying the altered array
   std::cout << "New data added: ";
   for(auto v:vect)
      std::cout << v << " ";
   std::cout << std::endl;
   
   // Modifying the 2nd and 4th element
   vect.at(2) = 5;
   vect.at(4) = 6;
   
   // Displaying the altered array
   std::cout << "Manipuldated data: ";
   for(auto v:vect)
      std::cout << v << " ";
   std::cout << std::endl;

   return 0;
}