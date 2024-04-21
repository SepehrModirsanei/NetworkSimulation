#include <iostream>
#include <vector>
#include <climits>

using namespace std;

vector<int> findShortestPath(const vector<vector<int>>& graph, int source, int destination) {
    int numNodes = graph.size();

    vector<int> distance(numNodes, INT_MAX);
    vector<bool> visited(numNodes, false);
    vector<int> previous(numNodes, -1);

    distance[source] = 0;

    for (int i = 0; i < numNodes - 1; ++i) {
        int minDistance = INT_MAX;
        int minIndex = -1;

        // Find the node with the minimum distance
        for (int j = 0; j < numNodes; ++j) {
            if (!visited[j] && distance[j] < minDistance) {
                minDistance = distance[j];
                minIndex = j;
            }
        }

        // Mark the selected node as visited
        visited[minIndex] = true;

        // Update the distances of neighboring nodes
        for (int k = 0; k < numNodes; ++k) {
            if (!visited[k] && graph[minIndex][k] != 0 && distance[minIndex] != INT_MAX && distance[minIndex] + graph[minIndex][k] < distance[k]) {
                distance[k] = distance[minIndex] + graph[minIndex][k];
                previous[k] = minIndex;
            }
        }
    }

    // Construct the shortest path
    vector<int> shortestPath;
    int current = destination;
    while (current != -1) {
        shortestPath.insert(shortestPath.begin(), current);
        current = previous[current];
    }

    return shortestPath;
}

int main() {
    int numNodes;
    cout << "Enter the number of nodes: ";
    cin >> numNodes;

    vector<vector<int>> graph(numNodes, vector<int>(numNodes, 0));

    int numEdges;
    cout << "Enter the number of edges: ";
    cin >> numEdges;

    cout << "Enter the edges in the format 'node1 node2 weight':\n";
    cin.ignore(); // Ignore the newline character after the previous input

    for (int i = 0; i < numEdges; ++i) {
        string line;
        getline(cin, line);

        istringstream iss(line);
        int node1, node2, weight;
        if (iss >> node1 >> node2 >> weight) {
            if (node1 == node2) {
                cout << "Error: Node 1 and Node 2 cannot be the same.\n";
                continue;
            }

            if (node1 >= numNodes || node2 >= numNodes) {
                cout << "Error: Node indices are out of range.\n";
                continue;
            }

            // Subtract 1 from node values
            --node1;
            --node2;

            graph[node1][node2] = weight;
            graph[node2][node1] = weight;
        }
    }

    int source, destination;
    cout << "Enter the source and destination nodes: ";
    cin >> source >> destination;

    --source;
    --destination;

    vector<int> shortestPath = findShortestPath(graph, source, destination);

    cout << "Shortest Path: ";
    for (int node : shortestPath) {
        cout << node + 1 << " ";
    }
    cout << endl;

    return 0;
}
