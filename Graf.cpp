#include "Graf.hpp"

TrafficNetwork::~TrafficNetwork()
{
    for (Node *node : nodes)
    {
        delete node;
    }
}

void TrafficNetwork::addNode(Node *node)
{
    nodes.pb(node);
}

const vn &TrafficNetwork::getNodes() const
{
    return nodes;
}

Node *TrafficNetwork::getNodeById(const str &id)
{
    return findNodeById(id);
}

void TrafficNetwork::addEdge(const str &fromId, const str &toId, ll distance)
{
    Node *fromNode = findNodeById(fromId);
    Node *toNode = findNodeById(toId);
    if (fromNode && toNode)
    {
        fromNode->addEdge(toNode, distance);
        cout << "Kreiran edge od " << fromNode->getId() << " do " << toNode->getId() << endl;
    }
}

Node *TrafficNetwork::findNodeById(const str &id)
{
    for (Node *node : nodes)
    {
        if (node->getId() == id)
        {
            return node;
        }
    }
    return nullptr;
}

void TrafficNetwork::serializeGraph(const str &filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Greska pri otvaranju fajla za serijalizaciju" << endl;
        return;
    }

    for (const auto &node : nodes)
    {
        if (auto loc = dynamic_cast<Location *>(node))
        {
            file << "Node: Lokacija, ID: " << loc->getId() << "\n";
        }
        else if (auto road = dynamic_cast<Road *>(node))
        {
            file << "Node: Put, ID: " << road->getId() << ", Duzina: " << road->getLength()
                 << ", MaksBrzina: " << road->getMaxSpeed() << ", MaksVozila: " << road->getMaxVehicles() << "\n";
        }
        else if (auto inter = dynamic_cast<Intersection *>(node))
        {
            file << "Node: Raskrsnica, ID: " << inter->getId() << ", MaksVozila: " << inter->getMaxVehicles() << "\n";
        }
    }

    for (const auto &node : nodes)
    {
        for (const auto &edge : node->getEdges())
        {
            file << "Edge: " << node->getId() << ", " << edge.to->getId() << ", Dist: " << edge.distance << "\n";
        }
    }

    for (auto const &node : nodes)
    {
        if (auto inter = dynamic_cast<Intersection *>(node))
        {
            const auto &connections = inter->getConnections();
            for (const auto &connection : connections)
            {
                file << "Connection: " << inter->getId() << ", " << connection.incoming->getId() << ", " << connection.outgoing->getId()
                     << ", " << (connection.canMove ? "true" : "false") << ", Dist: " << connection.distance
                     << ", AvgSpeed: " << connection.averageSpeed << "\n";
            }
        }
    }

    file.close();
    cout << "Serijalizacija uspjesna" << endl;
}

void TrafficNetwork::deserializeGraph(const str &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Greska pri otvaranju fajla pri deserijalizaciji" << endl;
        return;
    }

    str line;
    Intersection *currentIntersection = nullptr; // Dodajte pokazivač na trenutnu raskrsnicu

    while (getline(file, line))
    {
        istringstream iss(line);
        str type, nodeType, id, dummy, dummy2, dummy3;
        iss >> type;

        if (type == "Node:")
        {
            iss >> nodeType;
            getline(iss, id, ',');
            id = id.substr(id.find("ID: ") + 4);

            if (nodeType.find("Lokacija") != str::npos)
            {
                addNode(new Location(id));
            }
            else if (nodeType.find("Put") != str::npos)
            {
                ll length, maxSpeed, maxVehicles;
                iss >> dummy >> length;
                iss >> dummy2 >> dummy3 >> maxSpeed;
                iss >> dummy3 >> dummy >> maxVehicles;
                addNode(new Road(id, length, maxSpeed, maxVehicles));
            }
            else if (nodeType.find("Raskrsnica") != str::npos)
            {
                ll maxVehicles;
                iss >> dummy >> maxVehicles;
                addNode(new Intersection(id, maxVehicles));
            }
        }
        else if (type == "Edge:")
        {
            str fromId, toId;
            ll distance;
            getline(iss, fromId, ',');
            fromId = fromId.substr(fromId.find(" ") + 1);
            getline(iss, toId, ',');
            toId = toId.substr(toId.find(" ") + 1);
            iss >> dummy >> distance;
            addEdge(fromId, toId, distance);
        }
        else if (type == "Connection:")
        {
            str interId, incomingId, outgoingId, canMoveStr, dummy, dummy1;
            ll distance, avgSpeed;
            bool canMove;
            char temp;
            getline(iss, interId, ',');
            getline(iss, incomingId, ',');
            getline(iss, outgoingId, ',');
            getline(iss, canMoveStr, ',');
            // cout << interId << incomingId << outgoingId;
            iss >> dummy >> distance;
            iss >> dummy >> dummy1 >> avgSpeed;
            // cout << distance << " " << avgSpeed << endl;
            interId.erase(0, interId.find(": ") + 2);
            incomingId.erase(0, incomingId.find(" ") + 1);
            outgoingId.erase(0, outgoingId.find(" ") + 1);
            canMoveStr.erase(0, canMoveStr.find(" ") + 1);
            // cout << interId << " " << incomingId << " " << outgoingId << " " << distance << " " << canMoveStr << " " << avgSpeed << endl;
            canMove = (canMoveStr == "true");

            Intersection *intersection = dynamic_cast<Intersection *>(getNodeById(interId));
            if (intersection)
            {
                // cout << "usao1" << endl;
                Road *incomingRoad = dynamic_cast<Road *>(getNodeById(incomingId));
                Road *outgoingRoad = dynamic_cast<Road *>(getNodeById(outgoingId));
                if (incomingRoad && outgoingRoad)
                {
                    // cout << "usao2" << endl;
                    intersection->addConnection(incomingRoad, outgoingRoad, canMove, distance, avgSpeed);
                }
            }
        }
    }

    file.close();
}
