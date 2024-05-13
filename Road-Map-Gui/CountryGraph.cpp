#include "CountryGraph.h"
#include<iostream>
#include <sstream>
#include <fstream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include<stack>
using namespace std;
const float INF = INFINITY;

unordered_map<string, list<edge>>& CountryGraph::getCities() {

    return cities;
}

void CountryGraph::AddCity(string newcity) {

    cities[newcity];//o(1)

    if (applychanges)
        undoStack.push({ 0, make_pair(newcity, list<edge>()) });


}

void CountryGraph::AddEdge(string city_1, string city_2, int cost) {
    if (city_1 == city_2)
    {
        cout << "you canot add edge between same city" << endl;
        return;
    }
    if (!FindEdge(city_1, city_2))
    {
        //cairo alex 10
        edge newedge1;
        newedge1.destination_city = city_1;//cairo
        newedge1.cost = cost;//10
        cities[city_2].push_back(newedge1);//o(1)
        //alex.list(cairo,10)
        edge newedge2;
        newedge2.destination_city = city_2;//alex
        newedge2.cost = cost;//10
        cities[city_1].push_back(newedge2);//o(1)
        //cairo.list(alex,10)

        if (applychanges)
            undoStack.push({ 1, make_pair(city_1, list<edge>{newedge2}) });
    }

}

bool CountryGraph::FindCity(string city)
{
    return cities.find(city) != cities.end();//o(1) 

}

bool CountryGraph::FindEdge(string city_1, string city_2)
{
    for (auto& edges : cities[city_1])
    {
        if (edges.destination_city == city_2)
        {
            return true;
        }
    }
    return false;

}

edge CountryGraph::GetEdge(string city_1, string city_2)
{
    for (auto& edges : cities[city_1])
    {
        if (edges.destination_city == city_2)
        {
            return edges;
        }
    }
}

void CountryGraph::DeleteCity(string deletedcity)
{
    if (applychanges)
        undoStack.push({ 2, make_pair(deletedcity, cities[deletedcity]) });

    cities.erase(deletedcity);
    for (auto& city : cities)//O(V*E)
    {
        auto& edges = city.second;
        edges.erase(remove_if(edges.begin(), edges.end(), [deletedcity](const edge& e) {
            return e.destination_city == deletedcity;
            }), edges.end());
    }
}

void CountryGraph::DeleteEdge(string city_1, string city_2) //O(E)
{

    if (FindCity(city_1) and FindCity(city_2))
    {
        if (applychanges)
            undoStack.push({ 3, make_pair(city_1, list<edge>{GetEdge(city_1, city_2)}) });

        cities[city_1].erase(std::remove_if(cities[city_1].begin(), cities[city_1].end(), [city_2](const edge& e) {
            return e.destination_city == city_2;
            }), cities[city_1].end());

        cities[city_2].erase(std::remove_if(cities[city_2].begin(), cities[city_2].end(), [city_1](const edge& e) {
            return e.destination_city == city_1;
            }), cities[city_2].end());



    }
}

void CountryGraph::Delete_AllGraph()
{
    cities.clear();
}

void CountryGraph::DisplayGraph()
{
    for (auto& city : cities)//o(n^2)
    {
        cout << "(" << city.first << ")";
        for (auto& Edges : city.second)
            cout << "------>" << "(" << Edges.destination_city << "," << Edges.cost << ")";
        cout << endl;
    }
}

void CountryGraph::DisplayCities()
{
    for (auto& city : cities)
    {
        cout << city.first << endl;
    }
}

void CountryGraph::DisplayEdges()
{
    for (auto& city : cities)
        for (auto& edge : city.second)
        {
            cout << "(" << city.first << "," << edge.destination_city << "," << edge.cost << ")" << endl;

        }
}
vector<pair<string, edge>> CountryGraph::getEdges()
{
    vector<pair<string, edge>> edges;
    for (auto& city : cities)
        for (auto& edge : city.second)
        {
            edges.push_back({ city.first , edge });
        }
    return edges;
}

int CountryGraph::Write_Cities_InFiles()
{
    string filename = "cities.txt";
    ofstream clearFile(filename);
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }
    for (auto& city : cities)
    {
        outfile << city.first << endl;
    }
    outfile.close();
    cout << " City saved to file: " << filename << endl;
}

int CountryGraph::Write_Edges_InFiles()
{
    string filename = "edges.txt";
    ofstream clearFile(filename);
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }
    for (auto& city : cities)
        for (auto& edge : city.second)
        {
            outfile << city.first << "," << edge.destination_city << "," << edge.cost << endl;

        }
    outfile.close();
    cout << " Edges saved to file: " << filename << endl;
}

int CountryGraph::Read_Cities_FromFiles()
{
    string filename = "cities.txt";
    vector<string> cities;
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    string line;
    while (getline(infile, line))
    {
        AddCity(line);
    }
    infile.close();
}

int CountryGraph::Read_Edges_FromFiles()
{
    string filename = "edges.txt";


    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    string line;
    while (getline(infile, line)) {
        vector<string> data_tokens;
        string token;
        istringstream tokenStream(line);
        while (getline(tokenStream, token, ',')) {
            data_tokens.push_back(token);
        }

        if (data_tokens.size() == 3)
        {
            AddEdge(data_tokens[0], data_tokens[1], stoi(data_tokens[2]));
        }
    }
    infile.close();
}

queue<string> CountryGraph::BFS(string start)
{
    unordered_set<string> visited;
    queue<string>temp;
    queue<string>path;

    visited.insert(start);
    temp.push(start);
    path.push(start);
    int levelnum = 0;

    while (!temp.empty())
    {
        string current = temp.front();
        temp.pop();

        for (const auto& nextEdge : cities.at(current))
        {
            string next = nextEdge.destination_city;
            if (temp.empty())
                levelnum++;
            if (visited.count(next) == 0)
            {
                visited.insert(next);
                temp.push(next);
                cout << next << " ";
                path.push(next);
            }
        }
    }
    return path;
}

queue<string> CountryGraph::DFS(string start_city) {
    unordered_map<string, bool> visited;
    stack<string> vertex_stack;
    queue<string>path;
    vertex_stack.push(start_city);
    while (!vertex_stack.empty()) {
        string current_city = vertex_stack.top();
        vertex_stack.pop();
        path.push(current_city);
        // Process the current city 
        cout << "city: " << current_city << " ";
        visited[current_city] = true;
        // Explore unvisited neighbors
        for (auto& edge : cities[current_city]) {
            string neighbor_city = edge.destination_city;
            if (!visited[neighbor_city]) {
                //  visited[neighbor_city] = true;
                vertex_stack.push(neighbor_city);
            }
        }

    }
    return path;
}

queue<pair<string, edge>> CountryGraph::Prims() {
    // pq sort edges acendingly 
    priority_queue <pair<int, pair<string, string>>, vector<pair<int, pair<string, string>>>, greater<pair<int, pair<string, string>>>> pq;
    queue <pair<string, edge>> path; //gui
    CountryGraph msp;
    unordered_map<string, bool> vis;
    int total_cost = 0;

    // start with all cities unvisited
    for (auto& city : cities)
        vis[city.first] = false;

    // visit the first city and push the edges 
    string start = cities.begin()->first;
    vis[start] = true;
    for (auto& edge : cities[start]) {
        if (!vis[edge.destination_city])
            pq.push({ edge.cost, {start, edge.destination_city} });
    }

    while (!pq.empty()) {

        int cost = pq.top().first;
        string parent = pq.top().second.first;
        string destination = pq.top().second.second;
        pq.pop();

        // continue if destination was visited 

        if (!vis[destination]) {
            // add the cost and visit the city
            total_cost += cost;
            vis[destination] = true;
            msp.AddEdge(parent, destination, cost);
            cout << parent << " -> " << destination << " " << cost << endl;
            edge e;
            e.destination_city = destination;
            e.cost = cost;
            path.push({ parent, e });
            // push all the edges 
            for (auto& edge : cities[destination]) {
                if (!vis[edge.destination_city])
                    pq.push({ edge.cost, {destination, edge.destination_city} });
            }
        }
    }
    return path;
}
void CountryGraph::dijkstra_algorithm(string source)//O((V+E)logV)
{
    unordered_map<string, int> costs; //for sorting each city with it’s updated distance
    unordered_map<string, bool> visited; // for marking the cities the dijkstra’s_algorithm already visit 
    unordered_map<string, string> previous_node;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> next_node; // the priority_queue using for sort the nodes using thier costs , we need the next node that one with min distance 


    for (auto node : cities) {
        //set all the distances with infinity value for using it in comparing
        costs[node.first] = INF;
        visited[node.first] = 0;
    }

    costs[source] = 0;
    next_node.push({ costs[source], source });

    while (!next_node.empty()) {
        auto current_city = next_node.top();
        string current = current_city.second;
        next_node.pop(); //remove this city cause it already selected (visited)
        if (!visited[current]) //second---> string in the priority_queue
        {
            visited[current] = 1;
            for (auto neighbours : cities[current]) {
                if (costs[current] + neighbours.cost < costs[neighbours.destination_city] && !visited[neighbours.destination_city]) {

                    costs[neighbours.destination_city] = costs[current] + neighbours.cost;
                    previous_node[neighbours.destination_city] = current;
                    next_node.push({ costs[neighbours.destination_city] ,neighbours.destination_city });

                }
            }
        }
    }

    cout << "Shortest distances from " << source << ":\n";
    for (auto distance : costs) {
        cout << distance.first << " : " << distance.second << " from " << previous_node[distance.first] << endl;
    }
}

void CountryGraph::ReAddCity(pair<string, list<edge>>& city) {
    cities[city.first];
    for (auto& e : city.second) {
        AddEdge(city.first, e.destination_city, e.cost);
    }
}

void CountryGraph::Undo() {
    if (!undoStack.empty()) {
        redoStack.push(undoStack.top());
        ApplyUChanges(undoStack.top());
        undoStack.pop();
    }
    else {
        cout << "Nothing to undo." << endl;
    }
}

// Method to redo the last undone operation
void CountryGraph::Redo() {
    if (!redoStack.empty()) {
        undoStack.push(redoStack.top());
        ApplyRChanges(redoStack.top());
        redoStack.pop();
    }
    else {
        cout << "Nothing to redo." << endl;
    }
}

void CountryGraph::ApplyUChanges(pair<int, pair<string, list<edge>>>& change) {
    applychanges = false;
    switch (change.first) {
    case 0: //delete city
        DeleteCity(change.second.first);
        break;

    case 1: //delete edge
        DeleteEdge(change.second.first, change.second.second.front().destination_city);
        break;

    case 2: // add city
        ReAddCity(change.second);
        break;

    case 3: // add edge
        AddEdge(change.second.first, change.second.second.front().destination_city, change.second.second.front().cost);
        break;
    }
    applychanges = true;
}

void CountryGraph::ApplyRChanges(pair<int, pair<string, list<edge>>>& change) {
    applychanges = false;
    switch (change.first) {
    case 0: //add city
        AddCity(change.second.first);
        break;

    case 1: //add edge
        AddEdge(change.second.first, change.second.second.front().destination_city, change.second.second.front().cost);
        break;

    case 2: // delete city
        DeleteCity(change.second.first);
        break;

    case 3: // delete edge
        DeleteEdge(change.second.first, change.second.second.front().destination_city);
        break;
    }
    applychanges = true;
}
