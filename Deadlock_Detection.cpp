#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <string>
#include <set>

using namespace std;

void printTableHeader(const vector<string>& headers) {
    cout << "  +";
    for (size_t i = 0; i < headers.size(); ++i) {
        cout << string(9, '-') << "+";
    }
    cout << endl;
    cout << "  |";
    for (const string& header : headers) {
        cout << setw(9) << left << header << "|";
    }
    cout << endl;
    cout << "  +";
    for (size_t i = 0; i < headers.size(); ++i) {
        cout << string(9, '-') << "+";
    }
    cout << endl;
}

void printTableRow(const vector<int>& rowData) {
    cout << "  |";
    for (int data : rowData) {
        cout << setw(9) << right << data << "|";
    }
    cout << endl;
}

void printTableFooter(const vector<string>& headers) {
    cout << "  +";
    for (size_t i = 0; i < headers.size(); ++i) {
        cout << string(9, '-') << "+";
    }
    cout << endl;
}

class ResourceAllocationGraph {
private:
    int numProcesses, numResources;
    vector<vector<int>> allocationMatrix, requestMatrix;
    vector<int> totalResourceInstances, availableResources;
    vector<vector<int>> adjMatrix;
    vector<int> resOrder;

    void printResourceInstancesTable_internal(const string& title, const vector<int>& instances) {
        cout << "\n" << title << ":\n";
        vector<string> headers;
        for (int j = 0; j < numResources; ++j) {
            headers.push_back("R" + to_string(j));
        }
        printTableHeader(headers);
        printTableRow(instances);
        printTableFooter(headers);
    }

    void printMatrixTable_internal(const string& title, const vector<vector<int>>& matrix, const string& rowHeaderPrefix) {
        cout << "\n" << title << ":\n";
        vector<string> headers;
        headers.push_back("Process");
        for (int j = 0; j < numResources; ++j) {
            headers.push_back("R" + to_string(j));
        }
        printTableHeader(headers);
        for (int i = 0; i < numProcesses; ++i) {
            vector<int> rowData = matrix[i];
            cout << "  | " << setw(9) << left << rowHeaderPrefix + to_string(i) << "|";
            printTableRow(rowData);
        }
        printTableFooter(headers);
    }


public:
    ResourceAllocationGraph(int p, int r) : numProcesses(p), numResources(r) {
        allocationMatrix.resize(p, vector<int>(r, 0));
        requestMatrix.resize(p, vector<int>(r, 0));
        totalResourceInstances.resize(r, 0);
        availableResources.resize(r, 0);
        adjMatrix.resize(p + r, vector<int>(p + r, 0));
        resOrder.resize(r);
        iota(resOrder.begin(), resOrder.end(), 0);
    }

    void inputMatrices() {
        cout << "\nEnter system configuration:\n";

        cout << "\nTotal instances for each resource type:\n";
        for (int j = 0; j < numResources; j++) {
            cout << "Instances of Resource R" << j << ": ";
            cin >> totalResourceInstances[j];
        }
        printResourceInstancesTable("Total Resource Instances");


        cout << "\nCurrently available instances for each resource type:\n";
        for (int j = 0; j < numResources; j++) {
            cout << "Available instances of R" << j << ": ";
            cin >> availableResources[j];
        }
        printResourceInstancesTable("Available Resource Instances");


        cout << "\nAllocation Matrix (resources allocated to each process):\n";
        for (int i = 0; i < numProcesses; i++) {
            cout << "Process P" << i << " -> ";
            for (int j = 0; j < numResources; j++) {
                cin >> allocationMatrix[i][j];
            }
        }
        printMatrixTable("Allocation Matrix");


        cout << "\nRequest Matrix (resources requested by each process):\n";
        for (int i = 0; i < numProcesses; i++) {
            cout << "Process P" << i << " -> ";
            for (int j = 0; j < numResources; j++) {
                cin >> requestMatrix[i][j];
            }
        }
        printMatrixTable("Request Matrix");

        buildGraph();
    }

    void setResourceOrder() {
        cout << "\nEnter resource order for deadlock prevention (resource indices, e.g., '0 2 1' for R0 < R2 < R1):\n";
        cout << "Current resources are R0 to R" << numResources - 1 << endl;
        vector<int> tempOrder(numResources);
        for (int i = 0; i < numResources; ++i) {
            cin >> tempOrder[i];
            if (tempOrder[i] < 0 || tempOrder[i] >= numResources) {
                cout << "Invalid resource index. Using default order.\n";
                iota(resOrder.begin(), resOrder.end(), 0);
                return;
            }
            for (int j = 0; i < j; ++j) {
                if (tempOrder[i] == tempOrder[j]) {
                    cout << "Duplicate resource index. Using default order.\n";
                    iota(resOrder.begin(), resOrder.end(), 0);
                    return;
                }
            }
        }
        resOrder = tempOrder;
        cout << "Resource order set successfully: ";
        for (int i = 0; i < numResources; ++i) {
            cout << "R" << resOrder[i] << (i == numResources - 1 ? "" : " < ");
        }
        cout << endl;
    }


    void buildGraph() {
        for (auto &row : adjMatrix) fill(row.begin(), row.end(), 0);
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                if (allocationMatrix[i][j] > 0)
                    adjMatrix[numResources + j][i] = 1;
                if (requestMatrix[i][j] > 0)
                    adjMatrix[i][numResources + j] = 1;
            }
        }
    }

    void printGraphRepresentation() {
        cout << "\nResource Allocation Graph:\n";
        bool hasEdges = false;
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                if (requestMatrix[i][j] > 0) {
                    cout << "  Process P" << i << " is requesting " << requestMatrix[i][j] << " units of Resource R" << j << "\n";
                    hasEdges = true;
                }
                if (allocationMatrix[i][j] > 0) {
                    cout << "  Resource R" << j << " is held by Process P" << i << " (" << allocationMatrix[i][j] << " units)\n";
                    hasEdges = true;
                }
            }
        }
        if (!hasEdges) {
            cout << "  Graph is empty (no requests or allocations).\n";
        }
    }

    bool detectCycleUtil(int node, vector<bool> &visited, vector<bool> &recStack, vector<int> &cycleNodes, int &cycleStart) {
        if (!visited[node]) {
            visited[node] = true;
            recStack[node] = true;
            cycleNodes.push_back(node);

            for (int i = 0; i < numResources + numProcesses; i++) {
                if (adjMatrix[node][i]) {
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

    bool detectDeadlock() {
        cout << "\n-------- Deadlock Detection Process --------\n";
        printResourceInstancesTable("Current Available Resource Instances");
        printMatrixTable("Current Allocation Matrix");
        printMatrixTable("Current Request Matrix");

        printGraphRepresentation();
        vector<bool> visited(numResources + numProcesses, false);
        vector<bool> recStack(numResources + numProcesses, false);
        vector<int> cycleNodes;
        int cycleStart = -1;

        for (int i = 0; i < numResources + numProcesses; i++) {
            if (!visited[i] && detectCycleUtil(i, visited, recStack, cycleNodes, cycleStart)) {
                cout << "\n******************** Deadlock Detected! ********************\n";
                printCycleDetails(cycleNodes, cycleStart);

                char killChoice;
                cout << "\nDo you want to resolve deadlock by killing processes? (y/n): ";
                cin >> killChoice;
                if (killChoice == 'y' || killChoice == 'Y') {
                    resolveDeadlock(cycleNodes, cycleStart);
                    cout << "\nDeadlock resolution completed by process termination.\n";
                    cout << "-------- Deadlock Detection Process Completed --------\n";
                    return false;
                } else {
                    cout << "\nDeadlock resolution skipped. Deadlock persists.\n";
                    cout << "-------- Deadlock Detection Process Completed --------\n";
                    return true;
                }
            }
        }
        cout << "\nNo deadlock detected.\n";
        cout << "-------- Deadlock Detection Process Completed --------\n";
        return false;
    }

    void printCycleDetails(vector<int> &cycleNodes, int cycleStart) {
        cout << "Deadlock Cycle Details:\n";
        for (size_t i = 0; i < cycleNodes.size(); ++i) {
            int currentNode = cycleNodes[i];
            int nextNode = (i == cycleNodes.size() - 1) ? cycleStart : cycleNodes[i + 1];

            if (currentNode < numProcesses) {
                cout << "  Process P" << currentNode << " is waiting for ";
            } else {
                cout << "  Resource R" << (currentNode - numResources) << " is held by ";
            }

            if (nextNode < numProcesses) {
                cout << "Process P" << nextNode << endl;
            } else {
                cout << "Resource R" << (nextNode - numResources) << endl;
            }
        }
        cout << "  (Cycle: ";
        for (int node : cycleNodes) {
            if (node < numProcesses) cout << "P" << node << " -> ";
            else cout << "R" << (node - numResources) << " -> ";
        }
        cout << (cycleStart < numProcesses ? "P" : "R") << (cycleStart < numProcesses ? cycleStart : cycleStart - numResources) << ")\n";
    }


    bool requestResource(int processID, int resourceID, int units) {
        cout << "\n-------- Resource Request Process --------\n";
        if (resourceID < 0 || resourceID >= numResources) {
            cout << "Invalid resource ID.\n";
            cout << "-------- Resource Request Process Completed --------\n";
            return false;
        }
        if (processID < 0 || processID >= numProcesses) {
            cout << "Invalid process ID.\n";
            cout << "-------- Resource Request Process Completed --------\n";
            return false;
        }
        if (units <= 0) {
            cout << "Invalid request units.\n";
            cout << "-------- Resource Request Process Completed --------\n";
            return false;
        }

        for (int j = 0; j < numResources; ++j) {
            if (allocationMatrix[processID][j] > 0) {
                int heldResourceIndexInOrder = -1;
                int requestedResourceIndexInOrder = -1;

                for(int orderIndex = 0; orderIndex < numResources; ++orderIndex) {
                    if (resOrder[orderIndex] == j) heldResourceIndexInOrder = orderIndex;
                    if (resOrder[orderIndex] == resourceID) requestedResourceIndexInOrder = orderIndex;
                }

                if (requestedResourceIndexInOrder < heldResourceIndexInOrder) {
                    cout << "Resource Order Violation: Process P" << processID
                         << " already holds R" << j << " (order index " << heldResourceIndexInOrder << "), cannot request R"
                         << resourceID << " (order index " << requestedResourceIndexInOrder << " which is lower).\n";
                    cout << "-------- Resource Request Process Completed --------\n";
                    return false;
                }
            }
        }


        if (availableResources[resourceID] >= units) {
            availableResources[resourceID] -= units;
            allocationMatrix[processID][resourceID] += units;
            requestMatrix[processID][resourceID] = 0;
            buildGraph();
            cout << "Successfully allocated " << units << " units of R" << resourceID << " to Process P" << processID << ".\n";
            printResourceInstancesTable("Updated Available Resource Instances");
            printMatrixTable("Updated Allocation Matrix");
            cout << "-------- Resource Request Process Completed --------\n";
            return true;
        } else {
            cout << "Not enough resources available for R" << resourceID << ". Request by P" << processID << " for " << units << " units denied.\n";
            cout << "-------- Resource Request Process Completed --------\n";
            return false;
        }
    }

    void releaseResource(int processID, int resourceID, int units) {
        cout << "\n-------- Resource Release Process --------\n";
        if (resourceID < 0 || resourceID >= numResources) {
            cout << "Invalid resource ID.\n";
            cout << "-------- Resource Release Process Completed --------\n";
            return;
        }
        if (processID < 0 || processID >= numProcesses) {
            cout << "Invalid process ID.\n";
            cout << "-------- Resource Release Process Completed --------\n";
            return;
        }
        if (units <= 0) {
            cout << "Invalid release units.\n";
            cout << "-------- Resource Release Process Completed --------\n";
            return;
        }
        if (allocationMatrix[processID][resourceID] < units) {
            cout << "Process P" << processID << " is not holding " << units << " units of R" << resourceID << " to release.\n";
             cout << "-------- Resource Release Process Completed --------\n";
            return;
        }

        allocationMatrix[processID][resourceID] -= units;
        availableResources[resourceID] += units;
        buildGraph();
        cout << "Successfully released " << units << " units of R" << resourceID << " from Process P" << processID << ".\n";
        printResourceInstancesTable("Updated Available Resource Instances");
        printMatrixTable("Updated Allocation Matrix");
        cout << "-------- Resource Release Process Completed --------\n";
    }

    void resolveDeadlock(vector<int> &cycleNodes, int cycleStart) {
        cout << "\n-------- Deadlock Resolution Process --------\n";
        cout << "Resolving deadlock by killing processes...\n";
        set<int> processesToKill;
        for (int node : cycleNodes) {
            if (node < numProcesses) {
                processesToKill.insert(node);
            }
        }
        processesToKill.insert(cycleStart);
        set<int> killedProcesses;

        for (int processID : processesToKill) {
            if (processID < numProcesses && killedProcesses.find(processID) == killedProcesses.end()) {
                cout << "  Killing Process P" << processID << ".\n";
                cout << "  Resources released from Process P" << processID << ":\n";
                for (int resourceID = 0; resourceID < numResources; ++resourceID) {
                    int unitsToRelease = allocationMatrix[processID][resourceID];
                    if (unitsToRelease > 0) {
                        availableResources[resourceID] += unitsToRelease;
                        allocationMatrix[processID][resourceID] = 0;
                        cout << "    - " << unitsToRelease << " units of Resource R" << resourceID << "\n";
                    }
                }
                killedProcesses.insert(processID);
            }
        }
        buildGraph();
        printResourceInstancesTable("Updated Available Resource Instances");
        printMatrixTable("Updated Allocation Matrix");
        cout << "-------- Deadlock Resolution Process Completed --------\n";
    }

    void printResourceInstancesTable(const string& title) {
        printResourceInstancesTable_internal(title, availableResources);
    }

    void printMatrixTable(const string& title) {
        printMatrixTable_internal(title, allocationMatrix, "P");
    }


};

class WaitForGraph {
    int numProcesses;
    vector<vector<int>> waitGraph;

    void printWaitForGraphTable() {
        cout << "\nWait-For Graph Adjacency Matrix:\n";
        vector<string> headers;
        headers.push_back("Process");
        for (int j = 0; j < numProcesses; ++j) {
            headers.push_back("P" + to_string(j));
        }
        printTableHeader(headers);
        for (int i = 0; i < numProcesses; ++i) {
            vector<int> rowData = waitGraph[i];
            cout << "  | " << setw(9) << left << "P" + to_string(i) << "|";
            printTableRow(rowData);
        }
        printTableFooter(headers);
    }


public:
    WaitForGraph(int p) : numProcesses(p) {
        waitGraph.resize(p, vector<int>(p, 0));
    }

    void inputGraph() {
        cout << "Enter Wait-For Graph adjacency matrix (0 or 1):\n";
        for (int i = 0; i < numProcesses; i++) {
            cout << "Process P" << i << " -> ";
            for (int j = 0; j < numProcesses; j++) {
                cin >> waitGraph[i][j];
            }
        }
        printWaitForGraphTable();
    }

    void printGraph();
    bool detectDeadlock();
    bool detectCycleUtil(int, vector<bool>&, vector<bool>&);
};


void WaitForGraph::printGraph() {
    cout << "\nWait-For Graph Representation:\n";
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numProcesses; j++) {
            if (waitGraph[i][j])
                cout << "Process P" << i << " -> Process P" << j << endl;
        }
    }
}

bool WaitForGraph::detectDeadlock() {
    cout << "\n-------- Wait-For Graph Deadlock Detection Process --------\n";
    printWaitForGraphTable();
    printGraph();
    vector<bool> visited(numProcesses, false);
    vector<bool> recStack(numProcesses, false);
    for (int i = 0; i < numProcesses; i++) {
        if (!visited[i] && detectCycleUtil(i, visited, recStack)) {
            cout << "\n******************** Deadlock Detected in Wait-For Graph! ********************\n";
            cout << "-------- Wait-For Graph Deadlock Detection Process Completed --------\n";
            return true;
        }
    }
    cout << "\nNo deadlock detected in Wait-For Graph.\n";
    cout << "-------- Wait-For Graph Deadlock Detection Process Completed --------\n";
    return false;
}

bool WaitForGraph::detectCycleUtil(int node, vector<bool> &visited, vector<bool> &recStack) {
    if (!visited[node]) {
        visited[node] = true;
        recStack[node] = true;

        for (int i = 0; i < numProcesses; i++) {
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
    int choice, p, r, methodChoice;
    int ragOperationChoice;

    cout << "Choose deadlock method:\n1. Resource Allocation Graph (RAG)\n2. Wait-For Graph (WFG)\nEnter choice: ";
    cin >> choice;

    switch (choice) {
        case 1: {
            cout << "Enter number of processes: ";
            cin >> p;
            cout << "Enter number of resources: ";
            cin >> r;
            ResourceAllocationGraph rag(p, r);
            rag.inputMatrices();

            do {
                cout << "\nChoose RAG operation:\n";
                cout << "1. Detect Deadlock\n";
                cout << "2. Deadlock Prevention (Resource Ordering)\n";
                cout << "3. Detect and Resolve Deadlock\n";
                cout << "4. Show Current System Tables\n";
                cout << "0. Exit RAG Menu\nEnter choice: ";
                cin >> methodChoice;

                switch (methodChoice) {
                    case 1:
                        rag.detectDeadlock();
                        break;
                    case 2: {
                        rag.setResourceOrder();
                        int preventionOperationChoice;
                        do {
                            cout << "\nRAG Deadlock Prevention Menu:\n";
                            cout << "1. Request Resource\n2. Release Resource\n3. Print Graph\n4. Detect Deadlock (for monitoring)\n5. Show Current System Tables\n0. Exit Prevention Mode\nEnter operation: ";
                            cin >> preventionOperationChoice;
                            switch (preventionOperationChoice) {
                                case 1: {
                                    int processID, resourceID, units;
                                    cout << "Enter Process ID, Resource ID, Units to request: ";
                                    cin >> processID >> resourceID >> units;
                                    rag.requestResource(processID, resourceID, units);
                                    break;
                                }
                                case 2: {
                                    int processID, resourceID, units;
                                    cout << "Enter Process ID, Resource ID, Units to release: ";
                                    cin >> processID >> resourceID >> units;
                                    rag.releaseResource(processID, resourceID, units);
                                    break;
                                }
                                case 3:
                                    rag.printGraphRepresentation();
                                    break;
                                case 4:
                                    rag.detectDeadlock();
                                    break;
                                case 5:
                                    rag.printResourceInstancesTable("Current Available Resource Instances");
                                    rag.printMatrixTable("Current Allocation Matrix");
                                    rag.printMatrixTable("Current Request Matrix");
                                    break;
                                case 0:
                                    cout << "Exiting Prevention Mode.\n";
                                    break;
                                default:
                                    cout << "Invalid operation in Prevention Menu.\n";
                                    break;
                            }
                        } while (preventionOperationChoice != 0);
                        break;
                    }
                    case 3:
                        rag.detectDeadlock();
                        break;
                    case 4:
                        rag.printResourceInstancesTable("Current Available Resource Instances");
                        rag.printMatrixTable("Current Allocation Matrix");
                        rag.printMatrixTable("Current Request Matrix");
                        break;
                    case 0:
                        cout << "Exiting RAG Menu.\n";
                        break;
                    default:
                        cout << "Invalid method choice for RAG.\n";
                        break;
                }
            } while (methodChoice != 0);
            break;
        }
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
