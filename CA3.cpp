#include <iostream>
#include <vector>
#include <sstream>

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

void modifyEdge(vector< vector<int> >& graph, int node1, int node2, int newWeight) {
    if (node1 == node2) {
        cout << "Error: Node 1 and Node 2 cannot be the same.\n";
        return;
    }

    if (node1 >= graph.size() || node2 >= graph.size()) {
        cout << "Error: Node indices are out of range.\n";
        return;
    }

    graph[node1][node2] = newWeight;
    graph[node2][node1] = newWeight;

    displayAdjacencyMatrix(graph);
}

void deleteEdge(vector< vector<int> >& graph, int deleteNode1, int deleteNode2) {
    if (deleteNode1 == deleteNode2) {
        cout << "Error: Node 1 and Node 2 cannot be the same.\n";
        return;
    }

    if (deleteNode1 >= graph.size() || deleteNode2 >= graph.size()) {
        cout << "Error: Node indices are out of range.\n";
        return;
    }

    graph[deleteNode1][deleteNode2] = 0;
    graph[deleteNode2][deleteNode1] = 0;

    displayAdjacencyMatrix(graph);
}
void findShortestPath(const vector< vector<int> >& x, int source) {
    int numNodes = x.size();

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
            if (!visited[k] && x[minIndex][k] != 0 && distance[minIndex] != INT_MAX && distance[minIndex] + x[minIndex][k] < distance[k]) {
                distance[k] = distance[minIndex] + x[minIndex][k];
                previous[k] = minIndex;
            }
        }

        // Print the minimum cost table at each iteration
        cout << "Iteration " << i + 1 << ": Minimum Cost Table\n";
        for (int node = 0; node < numNodes; ++node) {
            cout << "Node " << node << ": " << distance[node] << "\n";
        }
        cout << "\n";
    }

    // Print the shortest paths for all nodes
    for (int node = 0; node < numNodes; ++node) {
        if (node != source) {
            vector<int> shortestPath;
            int current = node;
            while (current != -1) {
                shortestPath.insert(shortestPath.begin(), current);
                current = previous[current];
            }

            cout << "Shortest Path from Node " << source << " to Node " << node << ": ";
            for (int n : shortestPath) {
                cout << n << " ";
            }
            cout << "\n";
        }
    }
}

void simulateBGP(const vector< vector<int> >& adjacencyMatrix) {
    int numNodes = adjacencyMatrix.size();

    // Initialize routing table
    vector< vector<int> > routingTable(numNodes, vector<int>(numNodes, -1));

    // Update routing table based on adjacency matrix
    for (int i = 0; i < numNodes; ++i) {
        for (int j = 0; j < numNodes; ++j) {
            if (i == j) {
                routingTable[i][j] = 0;  // Distance to self is 0
            } else if (adjacencyMatrix[i][j] != 0) {
                routingTable[i][j] = adjacencyMatrix[i][j];  // Directly connected nodes
            }
        }
    }

    // Perform BGP iterations
    for (int k = 0; k < numNodes; ++k) {
        for (int i = 0; i < numNodes; ++i) {
            for (int j = 0; j < numNodes; ++j) {
                if (routingTable[i][j] == -1 || routingTable[i][j] > routingTable[i][k] + routingTable[k][j]) {
                    if (routingTable[i][k] != -1 && routingTable[k][j] != -1) {
                        routingTable[i][j] = routingTable[i][k] + routingTable[k][j];
                    }
                }
            }
        }

        // Print the routing table for each iteration
        cout << "Iteration " << k + 1 << ": Routing Table\n";
        for (int i = 0; i < numNodes; ++i) {
            for (int j = 0; j < numNodes; ++j) {
                cout << routingTable[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

void simulateDVPR(const vector< vector<int> >& adjacencyMatrix, int sourceNode) {
    int numNodes = adjacencyMatrix.size();

    // Initialize distance vector table
    vector< vector<int> > distanceVectorTable(numNodes, vector<int>(numNodes, -1));

    // Initialize next hop table
    vector<int> nextHopTable(numNodes, -1);

    // Update distance vector table based on adjacency matrix
    for (int i = 0; i < numNodes; ++i) {
        for (int j = 0; j < numNodes; ++j) {
            if (i == j) {
                distanceVectorTable[i][j] = 0;  // Distance to self is 0
            } else if (adjacencyMatrix[i][j] != 0) {
                distanceVectorTable[i][j] = adjacencyMatrix[i][j];  // Directly connected nodes
            }
        }
    }

    // Perform DVPR iterations
    for (int k = 0; k < numNodes; ++k) {
        for (int i = 0; i < numNodes; ++i) {
            if (i != sourceNode) {
                int minDistance = INT_MAX;
                int minNode = -1;

                for (int j = 0; j < numNodes; ++j) {
                    if (distanceVectorTable[i][j] != -1 && adjacencyMatrix[j][i] != 0) {
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

    // Print the next hop table
    cout << "Next Hop Table:\n";
    for (int i = 0; i < numNodes; ++i) {
        if (i != sourceNode) {
            cout << "Node " << i << ": Next Hop -> " << nextHopTable[i] << endl;
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

    // Test modifyEdge function
    int modifyNode1, modifyNode2, newWeight;
    cout << "\nEnter the nodes to modify and the new weight: ";
    //cin >> modifyNode1 >> modifyNode2 >> newWeight;

    //modifyEdge(graph, modifyNode1, modifyNode2, newWeight);

    // Test deleteEdge function
    int deleteNode1, deleteNode2;
    cout << "\nEnter the nodes to delete the edge between: ";
    //cin >> deleteNode1 >> deleteNode2;

    //deleteEdge(graph, deleteNode1, deleteNode2);

   // return 0;
    // findShortestPath(graph,1);
    // cout << "Now BGP"<<endl;
    // simulateBGP(graph);
    simulateDVPR(graph,1);


}
