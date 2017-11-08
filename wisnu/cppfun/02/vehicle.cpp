/* vehicle.cpp */
#include "vehicle.h"

using namespace std;

// Constructor without arguments
Vehicle::Vehicle() :
   totalOfWheel(0)
{}

// Constructor with arguments
Vehicle::Vehicle(const string &type, int wheel) :
   vehicleType(type),
   totalOfWheel(wheel)
{}

// Destructor
Vehicle::~Vehicle()
{}