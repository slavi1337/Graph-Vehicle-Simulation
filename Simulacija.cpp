#include "Simulacija.hpp"

TrafficSimulation::TrafficSimulation(TrafficNetwork &g, const vector<Vehicle *> &v)
    : graph(g), vehicles(v) {}

void TrafficSimulation::performSimulationStep()
{
    currentStep++;
    cout << "Korak simulacije: " << currentStep << " sekundi" << endl;

    for (auto &vehicle : vehicles)
    {
        vehicle->updatePosition(1, graph); // jedan korak simulacije je 1 sekunda, izmijeniti ovdje ako treba drugi
        printVehicleState(*vehicle);
    }
}

void TrafficSimulation::printVehicleState(Vehicle &vehicle)
{
    cout << vehicle.getId() << " je trenutno na : " << vehicle.getCurrentPosition();
    if (vehicle.hasReachedDestination())
    {
        cout << ", i stigao je na destinaciju.";
    }
    else
    {
        cout << ", krece se prema: " << vehicle.getNextNode();
    }
    cout << endl;
}

bool TrafficSimulation::isSimulationEnded()
{
    for (const auto &vehicle : vehicles)
    {
        if (!vehicle->hasReachedDestination())
            return false;
    }
    return true;
}

db TrafficSimulation::calculateAverageTravelTime(Location *start, Location *end) const
{
    db totalTravelTime = 0;
    ll count = 0;
    for (const auto &vehicle : vehicles)
    {
        if (vehicle->getStartNode() == start && vehicle->getEndNode() == end)
        {
            totalTravelTime += vehicle->getTravelTime();
            count++;
        }
    }
    return (count > 0) ? totalTravelTime / count : 0;
}
