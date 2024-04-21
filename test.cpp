#include <iostream>
#include <vector>
#include <limits>
#include <stack>
#include <sstream>
#include <iomanip>

using namespace std;
void displayAdjacencyMatrix(const vector< vector<int> >& graph) {
    cout << "Adjacency Matrix:\n";
    for (const auto& row : graph) {
        for (int weight : row) {
            cout << weight << " ";
        }
        cout << "\n";
    }
}
void getShortestPath(int sourceNode, int destinationNode, const vector<int>& nextHopTable, stack<int>& shortestPath) {
    int currentNode = destinationNode;
    shortestPath.push(currentNode);

    while (currentNode != sourceNode) {
        currentNode = nextHopTable[currentNode];
        shortestPath.push(currentNode);
    }
}
void simulateDVRP(const vector<vector<int> >& adjacencyMatrix, int sourceNode) {
    int numNodes = adjacencyMatrix.size();

    // Initialize distance vector table
    vector<vector<int> > distanceVectorTable(numNodes, vector<int>(numNodes, -1));

    // Initialize next hop table
    vector<int> nextHopTable(numNodes, -1);

    // Update distance vector table based on adjacency matrix
    cout << "here"<< endl;
    for (int i = 0; i < numNodes; ++i) {
        for (int j = 0; j < numNodes; ++j) {
            if (i == j) {
                distanceVectorTable[i][j] = 0;  // Distance to self is 0
            } else if (adjacencyMatrix[i][j] != 0) {
                distanceVectorTable[i][j] = adjacencyMatrix[i][j];  // Directly connected nodes
            }
        }
    }

    // Perform DVRP iterations
    for (int k = 0; k < numNodes; ++k) {
        for (int i = 0; i < numNodes; ++i) {
            if (i != sourceNode) {
                int minDistance = numeric_limits<int>::max();
                int minNode = -1;

                for (int j = 0; j < numNodes; ++j) {
                    if (distanceVectorTable[i][j] != -1 && j != sourceNode) {
                        int distance = distanceVectorTable[i][j] + distanceVectorTable[sourceNode][j];
                        if (distance < minDistance) {
                            minDistance = distance;
                            minNode = j;
                        }
                    }
                }

                if (minNode != -1) {
                    distanceVectorTable[i][sourceNode] = minDistance;
                    nextHopTable[i] = minNode;
                }
            }
        }
    }

    // Store the shortest paths in a vector of strings
    vector<string> shortestPaths(numNodes);
    for (int i = 0; i < numNodes; ++i) {
        if (i != sourceNode) {
            stack<int> shortestPath;
            getShortestPath(sourceNode, i, nextHopTable, shortestPath);

            stringstream ss;
            while (!shortestPath.empty()) {
                ss << shortestPath.top();
                shortestPath.pop();
                if (!shortestPath.empty()) {
                    ss << "->";
                }
            }
            shortestPaths[i] = ss.str();
        }
    }

    // Print the next hop table
    cout << "Next Hop Table for Source Node " << sourceNode << ":\n";
    cout << "Dest | Next Hop\n";
    for (int i = 0; i < numNodes; ++i) {
        if (i != sourceNode) {
            cout << setw(4) << i << " | " << setw(8) << nextHopTable[i] << "\n";
        }
    }

    // Print the shortest path for each node
    cout << "\nShortest Paths from Source Node " << sourceNode << ":\n";
    cout << "Dest | Dist | Shortest Path\n";
    for (int i = 0; i < numNodes; ++i) {
        if (i != sourceNode) {
            cout << setw(4) << i << " | " << setw(4) << distanceVectorTable[i][sourceNode] << " | " << shortestPaths[i] << "\n";
        }
    }
}

int main() {
    int numNodes;
    cout << "Enter the number of nodes: ";
    cin >> numNodes;

    vector< vector <int> >graph(numNodes, vector<int>(numNodes, 0));

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

            graph[node1][node2] = weight;
            graph[node2][node1] = weight;
        }
    }
    displayAdjacencyMatrix(graph);

    int sourceNode = 0;

    simulateDVRP(graph, sourceNode);

    return 0;
}
