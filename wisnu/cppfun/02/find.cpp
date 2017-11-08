/* find.cpp */
#include <vector>
#include <algorithm>
#include <iostream>
#include "vehicle.h"

using namespace std;

bool isTwoWheeled(const Vehicle &vehicle)
{
   return (vehicle.getNumOfWheel() == 2);
}

auto main() -> int
{
   cout << "[find.cpp]" << endl;
   
   // Initializing several Vehicle instances
   Vehicle car("car", 4);
   Vehicle moto("motorcycle", 2);
   Vehicle bike("bicycle", 2);
   Vehicle bus("bus", 6);
   
   vector<Vehicle> vehicles = {car,moto,bike,bus};
   
   // Displaying the vehicles
   cout << "All vehicles: ";
   for(auto v:vehicles)
      cout << v.getType() << " ";
   cout << endl;
   
   // Displaying two-wheeled vehicles
   cout << "Two-wheeled vehicle(s): ";
   auto twv = find_if(
      begin(vehicles),
      end(vehicles),
      isTwoWheeled);
   while(twv != end(vehicles)) {
      cout << twv->getType() << " ";
      twv = find_if(++twv, end(vehicles), isTwoWheeled);
   }
   cout << endl;
   
   return 0;
}