#include "CountryGraph.h"

using namespace std;
// start abdelrahman ahmed :
UserGraph::UserGraph(string username, string password)
{
    this->username = username;
    this->password = password;
}

void UserGraph::loadGraphFromFiles()
{
    string city = username + "_city.txt";
    string edge = username + "_edge.txt";
    graph.Read_Cities_FromFiles(city);
    graph.Read_Edges_FromFiles(edge);
}

void UserGraph::storeGraphIntoFiles()
{
    string city = username + "_city.txt";
    string edge = username + "_edge.txt";
    graph.Write_Cities_InFiles(city);
    graph.Write_Edges_InFiles(edge);
}

void UserGraph::createFiles()
{
    string city = username + "_city.txt";
    const char* cityptr = city.c_str();
    ofstream outfile_city(cityptr);
    string edge = username + "_edge.txt";
    const char* edgeptr = edge.c_str();
    ofstream outfile_edge(edgeptr);
}

void UserGraph::removeFiles()
{
    string city = username + "_city.txt";
    const char* cityptr = city.c_str();
    remove(cityptr);
    string edge = username + "_edge.txt";
    const char* edgeptr = edge.c_str();
    remove(edgeptr);
}

void UserManager::signUp()
{
    string username;
    string password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    for (auto& user : users)
    {
        if (user.username == username && user.password == password)
        {
            cout << "user already exist, log in with it" << endl;
            return;
        }
    }

    UserGraph newuser(username, password);
    newuser.createFiles();
    users.push_back(newuser);
    cout << "regestrition done now log in" << endl;
}

UserGraph* UserManager::logIn()
{
    string username;
    string password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    for (auto& user : users)
    {
        if (user.username == username && user.password == password)
        {
            return &user;
        }
    }
    cout << "can not find user" << endl;
    return nullptr;
}

void UserManager::display()
{
    cout << "here all usernames\n";
    for (auto& user : users)
    {
        cout << user.username << endl;
    }
}

void UserManager::saveAllGraphs()
{
    for (auto& user : users)
    {
        user.storeGraphIntoFiles();
    }
}

void UserManager::loadAllGraphs()
{
    for (auto& user : users)
    {
        user.loadGraphFromFiles();
    }
}

void UserManager::deleteGraph()
{
    string username;
    string password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].username == username && users[i].password == password)
        {
            users[i].removeFiles();
            users.erase(users.begin() + i);
            return;
        }
    }
    cout << "user you entered no found\n";
    display();
}

void UserManager::loadUsers()
{
    string filename = "USRES.txt";
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(infile, line))
    {
        vector<string> data_tokens;
        string token;
        istringstream tokenStream(line);
        while (getline(tokenStream, token, ',')) {
            data_tokens.push_back(token);
        }
        UserGraph user(data_tokens[0], data_tokens[1]);
        users.push_back(user);
    }
    infile.close();
}

void UserManager::saveUsres()
{
    string filename = "USRES.txt";
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    for (auto& user : users)
    {
        outfile << user.username << "," << user.password << endl;
    }
    outfile.close();
}
// End abdelrahman ahmed 
// start abdelrahman tamer 
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
    AddCity(city_1);
    AddCity(city_2);
    if (!FindEdge(city_1, city_2))
    {

        edge newedge1;
        newedge1.destination_city = city_1;
        newedge1.cost = cost;
        cities[city_2].push_back(newedge1);
        edge newedge2;
        newedge2.destination_city = city_2;
        newedge2.cost = cost;
        cities[city_1].push_back(newedge2);

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

unordered_map<string, list<edge>>& CountryGraph::getCities()
{
    return cities;
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

int CountryGraph::Write_Cities_InFiles(string filename)
{

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
}

int CountryGraph::Write_Edges_InFiles(string filename)
{
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
}

int CountryGraph::Read_Cities_FromFiles(string filename)
{
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    string line;
    while (getline(infile, line))
    {
        if (line != "")
            AddCity(line);
    }
    infile.close();
}

int CountryGraph::Read_Edges_FromFiles(string filename)
{

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

bool CountryGraph::is_graphempty()
{
    return cities.empty();
}
//end abdelrahman tamer
//start abdelrahman azzat  
queue<string> CountryGraph::BFS(string start)
{
    unordered_set<string> visited;
    queue<string>temp;
    queue<string>gui_q;
    visited.insert(start);
    temp.push(start);
    gui_q.push(start);
    int levelsize = 0;
    while (!temp.empty())
    {
        int levelSize = temp.size();
        string currentlevel = "";
        for (int i = 0; i < levelSize; i++)
        {

            string current = temp.front();
            temp.pop();
            for (const auto& nextEdge : cities.at(current))
            {
                string next = nextEdge.destination_city;
                if (visited.count(next) == 0)
                {
                    visited.insert(next);
                    temp.push(next);
                    currentlevel += next + "-";
                }
            }
        }
        gui_q.push(currentlevel);
    }


    return gui_q;
}
//end abdelrahman azzat 
//start shahd Hany 
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
//end  shahd Hany
// start Mai 
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
//end Mai 
//start Safwa 
queue<string> CountryGraph::dijkstra_algorithm(string source)//O((V+E)logV)
{

    unordered_map<string, int> costs; //for sorting each city with it’s updated distance
    unordered_map<string, bool> visited; // for marking the cities the dijkstra’s_algorithm already visit 
    unordered_map<string, string> previous_node;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> next_node; // the priority_queue using for sort the nodes using thier costs , we need the next node that one with min distance 


    for (auto node : cities) {
        //set all the distances with infinity value for using it in comparing
        costs[node.first] = numeric_limits<int>::max();
        visited[node.first] = 0;
    }

    costs[source] = 0;
    previous_node[source] = " The start city";
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
    queue<string>gui;
    for (auto distance : costs) {
        string lineforgui = "";
        lineforgui += distance.first + " : ";
        if (distance.second == numeric_limits<int>::max())
        {
            lineforgui += " it is disconnected city from start city ";
        }
        else {
            lineforgui += to_string(distance.second);
            if (previous_node[distance.first] == " The start city")
            {
                lineforgui += previous_node[distance.first];
            }
            else
            {
                lineforgui += " from " + previous_node[distance.first];
            }
        }
        gui.push(lineforgui);
    }

    return gui;

}
// end Safwa
//start Rana 
pair<int, vector<string>> CountryGraph::FloydWarshall2(string start_city, string dist_city) {
    // Create a distance map to store all shortest paths
    unordered_map<string, unordered_map<string, int>> distance;

    // Create a predecessor map to reconstruct the shortest paths
    unordered_map<string, unordered_map<string, string>> predecessor_city;


    // Initialize the distance and predecessor maps with the graph data
    for (const auto& city : cities) {
        distance[city.first] = unordered_map<string, int>();
        predecessor_city[city.first] = unordered_map<string, string>();
        for (const auto& edge : city.second) {
            distance[city.first][edge.destination_city] = edge.cost;
            predecessor_city[city.first][edge.destination_city] = city.first;
        }
        // Set distance to self to 0 and to unconnected cities to infinity
        distance[city.first][city.first] = 0;
        for (const auto& otherCity : cities) {
            if (distance[city.first].count(otherCity.first) == 0) {
                distance[city.first][otherCity.first] = INT_MAX;
                predecessor_city[city.first][otherCity.first] = ""; // No path initially
            }
        }
    }

    // Relax all edges by considering intermediate vertices
    for (const auto& intermediate_city : cities) {
        for (const auto& source_city : cities) {
            for (const auto& destination_city : cities) {
                // If there's a shorter path through the intermediate vertex, update the distance and predecessor
                if (distance[source_city.first][intermediate_city.first] != INT_MAX &&
                    distance[intermediate_city.first][destination_city.first] != INT_MAX &&
                    distance[source_city.first][intermediate_city.first] + distance[intermediate_city.first][destination_city.first] < distance[source_city.first][destination_city.first]) {
                    distance[source_city.first][destination_city.first] = distance[source_city.first][intermediate_city.first] + distance[intermediate_city.first][destination_city.first];
                    predecessor_city[source_city.first][destination_city.first] = predecessor_city[intermediate_city.first][destination_city.first];
                }
            }
        }
    }

    // Get the distance between the start and destination city
    int dist = distance[start_city][dist_city];
    vector<string> path;
    // Print the shortest path
    cout << "Shortest path from " << start_city << " to " << dist_city << ":" << endl;
    if (dist == INT_MAX) {
        cout << "No path exists." << endl;
    }
    else {
        
        string next_city = dist_city;
        while (next_city != "") {
            path.push_back(next_city);
            next_city = predecessor_city[start_city][next_city];
        }
        reverse(path.begin(), path.end());
        for (const auto& city : path) {
            cout << city;
            if (city != dist_city) {
                cout << " -> ";
            }
        }

    }

    return { dist, path};
}

string CountryGraph::findParent(unordered_map<string, string>& parent, const string& city) {
    if (parent[city] != city) {
        parent[city] = findParent(parent, parent[city]);
    }
    return parent[city];
}

void CountryGraph::unionCities(unordered_map<string, string>& parent, unordered_map<string, int>& rank, const string& city1, const string& city2) {
    string parent1 = findParent(parent, city1);
    string parent2 = findParent(parent, city2);

    if (parent1 != parent2) {
        if (rank[parent1] < rank[parent2]) {
            parent[parent1] = parent2;
        }
        else if (rank[parent1] > rank[parent2]) {
            parent[parent2] = parent1;
        }
        else {
            parent[parent2] = parent1;
            rank[parent1]++;
        }
    }
}

queue<pair<string, edge>> CountryGraph::kruskalMST() {
    // Define a structure to represent an edge
    struct Edge {
        string src;
        string dest;
        int cost;

        Edge(const string& source, const string& destination, int cst) : src(source), dest(destination), cost(cst) {}
    };

    // Comparator function for sorting edges by cost
    auto compareEdges = [](const Edge& e1, const Edge& e2) {
        return e1.cost > e2.cost; // Note: Changed to '>' for minimum spanning tree
    };

    // Priority queue to store edges sorted by cost
    priority_queue<Edge, vector<Edge>, decltype(compareEdges)> pq(compareEdges);

    // Add all edges to the priority queue
    for (const auto& city : cities) {
        for (const auto& edge : city.second) {
            pq.emplace(city.first, edge.destination_city, edge.cost);
        }
    }

    // Set to store parent of each city
    unordered_map<string, string> parent;

    // Initialize rank for each city
    unordered_map<string, int> rank;
    for (const auto& city : cities) {
        parent[city.first] = city.first; // Each city is its own parent initially
        rank[city.first] = 0; // Initialize rank to 0
    }

    // Minimum spanning tree edges
    queue<pair<string, edge>> path; // Queue to store edges for GUI visualization

    int minCost = 0; // Variable to store the total cost of the minimum spanning tree

    // Iterate until all edges are processed or MST is formed
    while (!pq.empty()) {
        Edge currentEdge = pq.top();
        pq.pop();

        string parent1 = findParent(parent, currentEdge.src);
        string parent2 = findParent(parent, currentEdge.dest);

        // Check if adding this edge forms a cycle
        if (parent1 != parent2) {
            // Add edge to MST
            edge e = { currentEdge.dest, currentEdge.cost };
            path.push({ currentEdge.src, e });

            // Add cost to the minimum spanning tree
            minCost += currentEdge.cost;

            // Merge the sets of source and destination cities
            unionCities(parent, rank, parent1, parent2);
        }
    }

    return path;
}

//end Rana
bool CountryGraph::is_connected()
{
    auto it = cities.begin()->first;
    DFS(it);
    if (connected)
    {
        return true;
    }
    else
    {
        return false;
    }
}








