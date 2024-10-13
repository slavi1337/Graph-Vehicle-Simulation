#include "Put.hpp"

Road::Road(const str &id, ll length, ll maxSpeed, ll maxVehicles)
    : Node(id), length(length), maxSpeed(maxSpeed), maxVehicles(maxVehicles)
{
    printDetails();
}

void Road::printDetails() const
{
    cout << "Kreiran put ID: " << id << endl;
}

ll Road::getLength() const
{
    return length;
}

ll Road::getMaxSpeed() const
{
    return maxSpeed;
}

void Road::incrementVehicleCount() { currentVehicleCount++; }
void Road::decrementVehicleCount()
{
    if (currentVehicleCount > 0)
        currentVehicleCount--;
}
ll Road::getCurrentVehicleCount() const { return currentVehicleCount; }
bool Road::isFull() const { return currentVehicleCount >= maxVehicles; }
ll Road::getMaxVehicles() const
{
    return maxVehicles;
}