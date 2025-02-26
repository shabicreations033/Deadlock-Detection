#include <iostream>
#include <vector>

using namespace std;

class ResourceAllocationGraph {
    int processes, resources;
    vector<vector<int>> allocation, request;
    vector<int> totalInstances, available;
    vector<vector<int>> adjacencyMatrix;

public:
    ResourceAllocationGraph(int p, int r) : processes(p), resources(r) {
        allocation.resize(p, vector<int>(r, 0));
        request.resize(p, vector<int>(r, 0));
        totalInstances.resize(r, 0);
        available.resize(r, 0);
        adjacencyMatrix.resize(p + r, vector<int>(p + r, 0));
    }

    void inputMatrices() {
        cout << "\nEnter total instances of each resource:\n";
        for (int j = 0; j < resources; j++) {
            cout << "Resource R" << j << ": ";
            cin >> totalInstances[j];
        }

        cout << "\nEnter available resources:\n";
        for (int j = 0; j < resources; j++) {
            cout << "Available instances of R" << j << ": ";
            cin >> available[j];
        }

        cout << "\nEnter allocation matrix:\n";
        for (int i = 0; i < processes; i++) {
            cout << "P" << i << " -> ";
            for (int j = 0; j < resources; j++) {
                cin >> allocation[i][j];
            }
        }

        cout << "\nEnter request matrix:\n";
        for (int i = 0; i < processes; i++) {
            cout << "P" << i << " -> ";
            for (int j = 0; j < resources; j++) {
                cin >> request[i][j];
            }
        }

        buildGraph();
    }

    void buildGraph() {
        for (auto &row : adjacencyMatrix) fill(row.begin(), row.end(), 0);
        for (int i = 0; i < processes; i++) {
            for (int j = 0; j < resources; j++) {
                if (allocation[i][j] > 0)
                    adjacencyMatrix[processes + j][i] = 1; // Resource to Process
                if (request[i][j] > 0)
                    adjacencyMatrix[i][processes + j] = 1; // Process to Resource
            }
        }
    }

    void printGraphRepresentation();
    bool detectDeadlock();
    bool detectCycleUtil(int, vector<bool>&, vector<bool>&, vector<int>&, int&);
    void printCycleDetails(vector<int>&, int);
};

void ResourceAllocationGraph::printGraphRepresentation() {
    cout << "\nGraph Representation:\n";
    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < resources; j++) {
            if (request[i][j] > 0)
                cout << "P" << i << " -> R" << j << " (requesting " << request[i][j] << ")\n";
            if (allocation[i][j] > 0)
                cout << "R" << j << " -> P" << i << " (holding " << allocation[i][j] << ")\n";
        }
    }
}

bool ResourceAllocationGraph::detectCycleUtil(int node, vector<bool> &visited, vector<bool> &recStack, vector<int> &cycleNodes, int &cycleStart) {
    if (!visited[node]) {
        visited[node] = true;
        recStack[node] = true;
        cycleNodes.push_back(node);

        for (int i = 0; i < processes + resources; i++) {
            if (adjacencyMatrix[node][i]) {
                if (!visited[i] && detectCycleUtil(i, visited, recStack, cycleNodes, cycleStart))
                    return true;
                else if (recStack[i]) {
                    cycleStart = i;
                    return true;
                }
            }
        }
    }
    recStack[node] = false;
    cycleNodes.pop_back();
    return false;
}

bool ResourceAllocationGraph::detectDeadlock() {
    printGraphRepresentation();
    vector<bool> visited(processes + resources, false);
    vector<bool> recStack(processes + resources, false);
    vector<int> cycleNodes;
    int cycleStart = -1;

    for (int i = 0; i < processes + resources; i++) {
        if (!visited[i] && detectCycleUtil(i, visited, recStack, cycleNodes, cycleStart)) {
            cout << "\nDeadlock Detected!\n";
            printCycleDetails(cycleNodes, cycleStart);
            return true;
        }
    }
    cout << "No deadlock detected.\n";
    return false;
}

void ResourceAllocationGraph::printCycleDetails(vector<int> &cycleNodes, int cycleStart) {
    cout << "Deadlock Cycle: ";
    for (int node : cycleNodes) {
        if (node < processes) cout << "P" << node << " -> ";
        else cout << "R" << (node - processes) << " -> ";
    }
    cout << "P" << cycleStart << " (Cycle Complete)\n";
}

class WaitForGraph {
    int processes;
    vector<vector<int>> waitGraph;

public:
    WaitForGraph(int p) : processes(p) {
        waitGraph.resize(p, vector<int>(p, 0));
    }

    void inputGraph() {
        cout << "Enter Wait-For Graph adjacency matrix:\n";
        for (int i = 0; i < processes; i++)
            for (int j = 0; j < processes; j++)
                cin >> waitGraph[i][j];
    }

    void printGraph();
    bool detectDeadlock();
    bool detectCycleUtil(int, vector<bool>&, vector<bool>&);
};

void WaitForGraph::printGraph() {
    cout << "\nWait-For Graph Representation:\n";
    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < processes; j++) {
            if (waitGraph[i][j])
                cout << "P" << i << " -> P" << j << endl;
        }
    }
}

bool WaitForGraph::detectDeadlock() {
    vector<bool> visited(processes, false);
    vector<bool> recStack(processes, false);
    printGraph();
    for (int i = 0; i < processes; i++) {
        if (!visited[i] && detectCycleUtil(i, visited, recStack)) {
            cout << "Deadlock detected in Wait-For Graph!\n";
            return true;
        }
    }
    cout << "No deadlock detected in Wait-For Graph.\n";
    return false;
}

bool WaitForGraph::detectCycleUtil(int node, vector<bool> &visited, vector<bool> &recStack) {
    if (!visited[node]) {
        visited[node] = true;
        recStack[node] = true;

        for (int i = 0; i < processes; i++) {
            if (waitGraph[node][i]) { 
                if (!visited[i] && detectCycleUtil(i, visited, recStack))
                    return true;
                else if (recStack[i])
                    return true;
            }
        }
    }
    recStack[node] = false;
    return false;
}

int main() {
    int choice, p, r;
    cout << "Choose detection method:\n1. Resource Allocation Graph (RAG)\n2. Wait-For Graph (WFG)\nEnter choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "Enter number of processes: ";
            cin >> p;
            cout << "Enter number of resources: ";
            cin >> r;
            { ResourceAllocationGraph rag(p, r); rag.inputMatrices(); rag.detectDeadlock(); }
            break;
        case 2:
            cout << "Enter number of processes: ";
            cin >> p;
            { WaitForGraph wfg(p); wfg.inputGraph(); wfg.detectDeadlock(); }
            break;
        default:
            cout << "Invalid choice.\n";
    }
    return 0;
}
