/* lambda_tiny_func.cpp */
#include <vector>
#include <algorithm>
#include <iostream>
#include "vehicle.h"

using namespace std;

auto main() -> int
{
   cout << "[lambda_tiny.cpp]" << endl;
   
   // Initializing several Vehicle instances
   Vehicle car("car", 4);
   Vehicle moto("motorcycle", 2);
   Vehicle bike("bicycle", 2);
   Vehicle bus("bus", 6);
   
   // Assigning the preceding instances
   vector<Vehicle> vehicles = {car,moto,bike,bus};
   
   // Displaying elements using Lambda expression
   cout << "All vehicles: ";
   for_each(
      begin(vehicles),
      end(vehicles),
      [](const Vehicle& v) {
         cout << v.getType() << " ";
   });
   cout << endl;

   return 0;
}