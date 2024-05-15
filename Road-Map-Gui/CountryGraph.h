#pragma once
#include<unordered_map>
#include<list>
#include<string>
#include <stack>
#include <queue>
#include<vector>
using namespace std;

struct edge
{
	string destination_city;
	int cost;
};
class CountryGraph
{

	unordered_map<string, list<edge>>cities;
	stack<pair<int, pair<string, list<edge>>>> undoStack;
	stack<pair<int, pair<string, list<edge>>>> redoStack;

public:
	void AddCity(string city);
	void AddEdge(string city_1, string city_2, int cost);
	bool FindCity(string city);
	bool FindEdge(string city_1, string city_2);
	void DisplayGraph();
	void DisplayCities();
	void DisplayEdges();
	void DeleteCity(string city);
	void DeleteEdge(string city_1, string city_2);
	void Delete_AllGraph();
	int Write_Cities_InFiles();
	int Write_Edges_InFiles();
	int Read_Cities_FromFiles();
	int Read_Edges_FromFiles();
	queue<string> DFS(string start_city);
	queue<string> BFS(string start);
	queue<pair<string, edge>> Prims();
	queue<string> dijkstra_algorithm(string start_city);
	void Undo();
	void Redo();
	void ApplyUChanges(pair<int, pair<string, list<edge>>>&);
	void ApplyRChanges(pair<int, pair<string, list<edge>>>&);
	void ReAddCity(pair<string, list<edge>>&);
	edge GetEdge(string city_1, string city_2);
	bool applychanges;
	unordered_map<string, list<edge>>& getCities();
	vector<pair<string, edge>> getEdges();
	string findParent(unordered_map<string, string>& parent, const string& city);
	void unionCities(unordered_map<string, string>& parent, unordered_map<string, int>& rank, const string& city1, const string& city2);
	queue<pair<string, edge>> kruskalMST();
	int FloydWarshall2(string start_city, string dist_city);
};