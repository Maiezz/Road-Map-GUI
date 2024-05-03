#pragma once
#include<unordered_map>
#include<list>
#include<string>
#include<vector>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
using namespace std;
struct edge
{
	string destination_city;
	int cost;
};
class CountryGraph
{
	unordered_map<string, list<edge>>cities;
	unordered_map<string, QPointF> cityPositions;
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
	CountryGraph prims(int&);

	

};
