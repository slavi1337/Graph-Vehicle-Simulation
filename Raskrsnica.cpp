#include "Raskrsnica.hpp"

Intersection::Intersection(const str &id, ll maxVehicles)
    : Node(id), maxVehicles(maxVehicles)
{
    printDetails();
}

void Intersection::addConnection(Road *incoming, Road *outgoing, bool canMove, ll distance, ll avgSpeed)
{
    connections.eb(incoming, outgoing, canMove, distance, avgSpeed);
    // treba dodati da se smanjuje brzina za mzd 10% za svako vozilo na raskrsnici
    cout << "Kreiran prelaz na raskrsnici ID: " << id << ", sa puta ID: " << incoming->getId() << " do puta ID: " << outgoing->getId() << endl;
    cout << "Trenutno raksrsnica ID: " << id << " ima " << connections.size() << " prelaza" << endl;
}

void Intersection::printDetails() const
{
    cout << "Kreairana raskrsnica ID: " << id << endl;
}

NodeConnection *Intersection::getConnection(Road *from, Road *to)
{
    for (auto &connection : connections)
    {
        if (connection.incoming == from && connection.outgoing == to)
        {
            return &connection;
        }
    }
    return nullptr;
}

void Intersection::incrementVehicleCount() { vehicleCount++; }
void Intersection::decrementVehicleCount()
{
    if (vehicleCount > 0)
        vehicleCount--;
}
ll Intersection::getVehicleCount() const { return vehicleCount; }
bool Intersection::isFull() const { return vehicleCount >= maxVehicles; }
ll Intersection::getMaxVehicles() const
{
    return maxVehicles;
}

vector<NodeConnection> Intersection::getConnections() const
{
    return connections;
}
