#include "CountryGraph.h"
#include<iostream>
#include <sstream>
#include <fstream>
#include<vector>
#include<queue>
#include<unordered_map>
using namespace std;


#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>




void CountryGraph::AddCity(string newcity) {

    cities[newcity];//o(1)
}

void CountryGraph::AddEdge(string city_1, string city_2, int cost) {
    if (!FindEdge(city_1, city_2))
    {

        edge newedge1;
        newedge1.destination_city = city_1;
        newedge1.cost = cost;
        cities[city_2].push_back(newedge1);//o(1)

        edge newedge2;
        newedge2.destination_city = city_2;
        newedge2.cost = cost;
        cities[city_1].push_back(newedge2);//o(1)
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

void CountryGraph::DeleteCity(string deletedcity)
{
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
    for (auto& city : cities)
    {
        DeleteCity(city.first);
    }

}

void CountryGraph::DisplayGraph()
{
    for (auto& city : cities)//o(n^2)
    {
        cout << "(" << city.first << ")" << "------>";
        for (auto& Edges : city.second)
            cout << "(" << Edges.destination_city << "," << Edges.cost << ")" << "------>";
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

        if (data_tokens.size() >= 3)
        {
            AddEdge(data_tokens[0], data_tokens[1], stoi(data_tokens[2]));
        }
    }
    infile.close();
}

CountryGraph CountryGraph::prims(int& total_cost) {
    priority_queue <pair<int, pair<string, string>>, vector<pair<int, pair<string, string>>>, greater<pair<int, pair<string, string>>>> pq;
    CountryGraph msp;
    unordered_map<string, bool> vis;

    // start with all cities unvisited
    for (auto& city : cities)
        vis[city.first] = false;

    string start = cities.begin()->first;
    pq.push({ 0, {start, start} });

    while (!pq.empty()) {

        int cost = pq.top().first;
        string parent = pq.top().second.first;
        string city = pq.top().second.second;
        pq.pop();

        if (!vis[city]) {
            total_cost += cost;
            vis[city] = true;

            if (city != start)
                msp.AddEdge(parent, city, cost);

            for (auto& edge : cities[city]) {
                if (!vis[edge.destination_city])
                    pq.push({ edge.cost, {city, edge.destination_city} });
            }
        }
    }
    return msp;
}

