#include "Vozilo.hpp"

Vehicle::Vehicle(const str &id, db avgSpeed, Node *start, Node *end, TrafficNetwork &graph)
    : id(id), averageSpeed(avgSpeed), start(start), destination(end), currentPosition(start)
{
}

str Vehicle::getNextNode()
{
    if (pathIndex + 1 < currentPath.size())
    {
        return currentPath[pathIndex + 1];
    }
    return "";
}

void Vehicle::updatePosition(long long timeStep, TrafficNetwork &graph)
{
    if (hasReachedDestination())
        return; // Vec je na destinaciji

    Node *currentNode = graph.getNodeById(currentPath[pathIndex]);
    // cout << "currnode"<< " " << currentNode->getId() << endl;
    double edgeDistance = 0;          // Distanca do sl cvora
    double speedLimit = averageSpeed; // Pocetna brz je prosjecna brzina

    if (auto *currRoad = dynamic_cast<Road *>(currentNode))
    {
        edgeDistance = currRoad->getLength();
        speedLimit = min(speedLimit, static_cast<double>(currRoad->getMaxSpeed()));
    }

    if (auto *intersectionNode = dynamic_cast<Intersection *>(currentNode))
    {
        Road *previousRoad = dynamic_cast<Road *>(graph.getNodeById(currentPath[pathIndex - 1]));
        Road *nextRoad = dynamic_cast<Road *>(graph.getNodeById(currentPath[pathIndex + 1]));

        NodeConnection *connection = intersectionNode->getConnection(previousRoad, nextRoad);
        if (connection && connection->canMove)
        {
            edgeDistance = connection->distance;

            // increment br vozila na tr cvoru
            intersectionNode->incrementVehicleCount();
            if (intersectionNode->getVehicleCount() <= intersectionNode->getMaxVehicles() + 1)
            {

                // smanjuje brzinu za 10% za svako sledece vozilo
                double speedAdjustmentFactor = 1.0 - (intersectionNode->getVehicleCount() * 0.1);
                speedLimit = min(speedLimit, static_cast<double>(connection->averageSpeed) * speedAdjustmentFactor);
            }
            else
            {
                // nista
            }
        }
        else
        {
            // Ako nema konekcija ili ne moze da se krece, ostaje na trenutnom nodu
            return;
        }
    }

    double distanceToTravel = min(timeStep * speedLimit, edgeDistance - currentPositionInMeters);
    currentPositionInMeters += distanceToTravel;

    if (currentPositionInMeters >= edgeDistance)
    {
        // Dekrementiranje broja vozila na prethodnoj raskrsnici
        if (auto *intersectionNode = dynamic_cast<Intersection *>(currentNode))
        {
            if (intersectionNode->getVehicleCount() > 1)
            {
                cout << "[BRZINA VOZILA] na raskrsnici se smanjila na" << speedLimit << " m/s zbog ostalih vozila" << endl;
                this_thread::sleep_for(chrono::seconds(2));
            }
            intersectionNode->decrementVehicleCount();
        }

        pathIndex++;
        currentPositionInMeters = 0;
        if (pathIndex < currentPath.size())
        {
            Node *nextNode = graph.getNodeById(currentPath[pathIndex]);
            currentPosition = nextNode;

            if (auto *nextIntersection = dynamic_cast<Intersection *>(nextNode))
            {
                nextIntersection->incrementVehicleCount();
            }
        }
        else
        {
            currentPosition = destination;
        }
    }
}

db Vehicle::calculateRemainingTime(db remainingDistance, db speed)
{
    return remainingDistance / speed;
}

str Vehicle::getCurrentPosition() const
{
    return currentPosition->getId();
}

void Vehicle::setCurrentPosition(Node *tren)
{
    currentPosition = tren;
}

bool Vehicle::hasReachedDestination() const
{
    return currentPosition == destination;
}

const str &Vehicle::getId() const { return id; }
Node *Vehicle::getStartNode() const { return start; }
Node *Vehicle::getEndNode() const { return destination; }
db Vehicle::getAverageSpeed() const { return averageSpeed; }

vn Vehicle::calculateBestRoute(TrafficNetwork &graph, Node *start, Node *end, Opcija metric, db &totalDistance, db &totalTime)
{
    // Dijkstra algoritam sa geeksforgeeks ali modifikovan za trenutne cvorove
    map<Node *, db> shortestDistances;
    map<Node *, Node *> previous;
    set<Node *> visited;
    auto compare = [&](Node *a, Node *b)
    { return shortestDistances[a] > shortestDistances[b]; };
    priority_queue<Node *, vn, decltype(compare)> pq(compare);

    totalDistance = 0.0;
    totalTime = 0.0;

    // distance i prethodnici
    for (auto *node : graph.getNodes())
    {
        shortestDistances[node] = numeric_limits<db>::infinity();
        previous[node] = nullptr;
    }
    shortestDistances[start] = 0;
    pq.push(start);

    while (!pq.empty())
    {
        Node *currentNode = pq.top();
        pq.pop();

        if (visited.find(currentNode) != visited.end())
            continue;
        visited.insert(currentNode);

        if (currentNode == end)
            break;

        for (const Edge &edge : currentNode->getEdges())
        {
            Node *neighbor = edge.to;
            db roadSpeedLimit = (metric == Opcija::Time && dynamic_cast<Road *>(neighbor)) ? dynamic_cast<Road *>(neighbor)->getMaxSpeed() : numeric_limits<db>::infinity();
            db effectiveSpeed = min(averageSpeed, roadSpeedLimit);
            db weight = (metric == Opcija::Time) ? (edge.distance / effectiveSpeed) : edge.distance;

            db distanceThroughU = shortestDistances[currentNode] + weight;
            if (distanceThroughU < shortestDistances[neighbor])
            {
                shortestDistances[neighbor] = distanceThroughU;
                previous[neighbor] = currentNode;
                pq.push(neighbor);
            }
        }
    }

    if (shortestDistances[end] == numeric_limits<db>::infinity())
    {
        // nema puta
        return {};
    }

    // Rekonstrukcija najkrace putanje i racunanje vremena i distance
    vn path;
    for (Node *at = end; at != nullptr; at = previous[at])
    {
        if (previous[at])
        {
            db segmentDistance = 0.0;
            db effectiveSpeed = averageSpeed;

            if (auto *currRoad = dynamic_cast<Road *>(at))
            {
                segmentDistance = currRoad->getLength();
                effectiveSpeed = min(averageSpeed, static_cast<db>(currRoad->getMaxSpeed()));
            }

            if (auto *intersection = dynamic_cast<Intersection *>(previous[at]))
            {
                NodeConnection *connection = intersection->getConnection(dynamic_cast<Road *>(previous[previous[at]]), dynamic_cast<Road *>(at));
                if (connection)
                {
                    segmentDistance += connection->distance;
                    effectiveSpeed = min(effectiveSpeed, static_cast<db>(connection->averageSpeed));
                }
            }

            totalDistance += segmentDistance;
            totalTime += segmentDistance / effectiveSpeed;

            path.insert(path.begin(), at);
            if (at == start)
            {
                break;
            }
        }
    }
    vector<str> stringPath;
    for (auto *node : path)
    {
        stringPath.push_back(node->getId());
    }

    // postavljanje default path-a vozila na pronadjeni put
    currentPath = stringPath;
    travelTime = totalTime;
    return path;
}

db Vehicle::getTravelTime() const
{
    return travelTime;
}
