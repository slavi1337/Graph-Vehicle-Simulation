#include "Node.hpp"
#include "helper.hpp"
#include "Lokacija.hpp"
#include "Raskrsnica.hpp"
#include "Graf.hpp"
#include "Vozilo.hpp"
#include "Simulacija.hpp"

// 4. tacku nisam radio (nisam znao smisliti primjer za ilustraciju xD)

int main()
{
    TrafficNetwork graph;

    auto *loc1 = new Location("Banja Luka");
    auto *loc2 = new Location("Krupa na Vrbasu");
    auto *loc3 = new Location("Mrkonjic Grad");
    auto *road1 = new Road("Vidovdanska", 10, 3, 100);
    auto *end1 = new Road("Vidovdanska Kraj", 0, 3, 100);
    auto *road2 = new Road("Jevrejska", 15, 2, 100);
    auto *end2 = new Road("Jevrejska Kraj", 0, 2, 100);
    auto *road3 = new Road("Ostroska", 3, 2, 100);
    auto *end3 = new Road("Ostroska Kraj", 0, 2, 100);
    auto *road4 = new Road("Trebinjska", 25, 4, 100);
    auto *end4 = new Road("Trebinjska Kraj", 0, 4, 100);
    auto *road5 = new Road("Randomska", 50, 10, 100);
    auto *end5 = new Road("Randomska Kraj", 0, 10, 100);
    auto *inter1 = new Intersection("Kanjon Raskrsnica", 2);

    graph.addNode(loc1);
    graph.addNode(loc2);
    graph.addNode(loc3);
    graph.addNode(road1);
    graph.addNode(road2);
    graph.addNode(road3);
    graph.addNode(inter1);
    graph.addNode(road4);
    graph.addNode(end1);
    graph.addNode(end2);
    graph.addNode(end3);
    graph.addNode(end4);
    graph.addNode(end5);
    graph.addNode(road5);

    graph.addEdge("Banja Luka", "Vidovdanska", 0);
    graph.addEdge("Vidovdanska", "Vidovdanska Kraj", 10);
    graph.addEdge("Vidovdanska Kraj", "Kanjon Raskrsnica", 0);
    graph.addEdge("Kanjon Raskrsnica", "Jevrejska", 0);
    graph.addEdge("Jevrejska", "Jevrejska Kraj", 15);
    graph.addEdge("Jevrejska Kraj", "Krupa na Vrbasu", 0);
    graph.addEdge("Krupa na Vrbasu", "Ostroska", 0);
    graph.addEdge("Ostroska", "Ostroska Kraj", 3);
    graph.addEdge("Ostroska Kraj", "Mrkonjic Grad", 0);
    graph.addEdge("Kanjon Raskrsnica", "Trebinjska", 0);
    graph.addEdge("Trebinjska", "Trebinjska Kraj", 25);
    graph.addEdge("Trebinjska Kraj", "Mrkonjic Grad", 0);
    graph.addEdge("Randomska", "Randomska Kraj", 50);
    graph.addEdge("Randomska Kraj", "Banja Luka", 0);
    graph.addEdge("Mrkonjic Grad", "Randomska", 0);
    inter1->addConnection(end1, road2, true, 5, 50);
    inter1->addConnection(end1, road4, true, 4, 2500);
    graph.serializeGraph("Serialize.txt");
    cout << "-----------------------------------------------------------------------------------" << endl;
    this_thread::sleep_for(chrono::seconds(3));
    TrafficNetwork graphnew;
    graphnew.deserializeGraph("Serialize.txt");
    graphnew.serializeGraph("graphnew.txt");
    cout << "-----------------------------------------------------------------------------------" << endl;
    this_thread::sleep_for(chrono::seconds(3));
    // golf ide od Banja Luke do Mrkonjic Grada i brzina mu je 4m/s, za njega se minimizuje vrijeme
    Vehicle vehicle1("Golf 4", 4, loc1, loc3, graph);
    db totalDistance = 0, totalTime = 0;
    auto path1 = vehicle1.calculateBestRoute(graph, loc1, loc3, Opcija::Time, totalDistance, totalTime);
    Node *temp1, *temp2, *temp3;
    cout << "Put za " << vehicle1.getId() << " tako da se minimizuje vrijeme" << endl;
    if (path1.empty())
    {
        cout << "Nema puta" << endl;
    }
    else
    {
        cout << "Banja Luka";
        int i = 0;
        for (const auto &node : path1)
        {
            if (i == 0)
                temp1 = node;
            cout << " -> " << node->getId();
            i++;
        }
        cout << "\nUkupno rastojanje: " << totalDistance << " metara" << endl;
        cout << "Ukupno vrijeme potrebno da se dodje: " << totalTime << " sekundi" << endl;
    }
    cout << "-----------------------------------------------------------------------------------" << endl;
    this_thread::sleep_for(chrono::seconds(3));

    // audi ide isto od BL do MG i isto ide 4m/s i za njega se minimizuje distanca
    Vehicle vehicle2("Audi A8", 4, loc1, loc3, graph);
    totalDistance = 0;
    totalTime = 0;
    auto path2 = vehicle2.calculateBestRoute(graph, loc1, loc3, Opcija::Distance, totalDistance, totalTime);

    cout << "Put za " << vehicle2.getId() << " tako da se minimizuje ukupan predjen put" << endl;
    if (path2.empty())
    {
        cout << "Nema puta" << endl;
    }
    else
    {
        cout << "Banja Luka";
        int i = 0;
        for (const auto &node : path2)
        {
            if (i == 0)
                temp2 = node;
            cout << " -> " << node->getId();
            i++;
        }
        cout << "\nUkupno rastojanje: " << totalDistance << " metara" << endl;
        cout << "Ukupno vrijeme potrebno da se dodje: " << totalTime << " sekundi" << endl;
    }
    cout << "-----------------------------------------------------------------------------------" << endl;
    this_thread::sleep_for(chrono::seconds(3));

    // supra ide od MG do BL brzina 5m/s jedini put preko Randomske ulice
    Vehicle vehicle3("Toyota Supra", 5, loc3, loc1, graph);
    totalDistance = 0;
    totalTime = 0;
    auto path3 = vehicle3.calculateBestRoute(graph, loc3, loc1, Opcija::Distance, totalDistance, totalTime);

    cout << "Put za " << vehicle3.getId() << " tako da se minimizuje ukupan predjen put" << endl;
    if (path3.empty())
    {
        cout << "Nema puta" << endl;
    }
    else
    {
        cout << "Mrkonjic Grad";
        int i = 0;
        for (const auto &node : path3)
        {
            if (i == 0)
                temp3 = node;
            cout << " -> " << node->getId();
            i++;
        }
        cout << "\nUkupno rastojanje: " << totalDistance << " metara" << endl;
        cout << "Ukupno vrijeme potrebno da se dodje: " << totalTime << " sekundi" << endl;
    }
    cout << "-----------------------------------------------------------------------------------" << endl;
    this_thread::sleep_for(chrono::seconds(3));

    vector<Vehicle *> vehicles;
    vehicles.pb(&vehicle1);
    vehicles.pb(&vehicle2);
    vehicles.pb(&vehicle3);

    TrafficSimulation simulation(graph, vehicles);

    simulation.printVehicleState(vehicle1);
    simulation.printVehicleState(vehicle2);
    simulation.printVehicleState(vehicle3);
    cout << "-----------------------------------------------------------------------------------" << endl;
    vehicle1.setCurrentPosition(temp1);
    vehicle2.setCurrentPosition(temp2);
    vehicle3.setCurrentPosition(temp3);
    // 10 koraka simulacije, nakon cega se ispisu pozicije
    for (int i = 0; i < 10; i++)
    {
        cout << "-----------------------------------------------------------------------------------" << endl;
        simulation.performSimulationStep();
        this_thread::sleep_for(chrono::seconds(1));
    }

    simulation.printVehicleState(vehicle1);
    simulation.printVehicleState(vehicle2);
    simulation.printVehicleState(vehicle3);

    cout << "-----------------------------------------------------------------------------------" << endl;

    // ostatak simulacije- dok svi ne dodju na destinaciju
    while (!simulation.isSimulationEnded())
    {

        cout << "-----------------------------------------------------------------------------------" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        simulation.performSimulationStep();
    }

    cout << "-----------------------------------------------------------------------------------" << endl;
    cout << "Kraj simulacije" << endl;
    // prosjecno vrijeme, golf i audi su isli od BL do MG -> njihovoUkupnoVr/2
    cout << "Prosjecno vrijeme trajanja putovanja od Banja Luka do Mrkonjic Grad je " << simulation.calculateAverageTravelTime(loc1, loc3) << " sekundi";

    return 0;
}