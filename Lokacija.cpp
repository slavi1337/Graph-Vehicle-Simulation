#include "Lokacija.hpp"

Location::Location(const str &id) : Node(id)
{
    printDetails();
}

void Location::printDetails() const
{
    cout << "Kreirana lokacija ID: " << id << endl;
}
