/* vehicle.h */
#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include <string>

class Vehicle {
private:
   std::string vehicleType;
   int totalOfWheel;
public:
   Vehicle(const std::string &type, int wheel);
   Vehicle();
   ~Vehicle();
   std::string getType() const {return vehicleType;}
   int getNumOfWheel() const {return totalOfWheel;}
};

#endif // End of __VEHICLE_H__