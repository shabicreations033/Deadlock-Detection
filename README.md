# Deadlock-Detection

**Operating System Project Documentation: DeadlockDetection**

**1. Introduction**

Deadlocks are a critical issue in operating systems, occurring when two or more processes are blocked indefinitely, waiting for each other to release resources. This project addresses the problem of deadlocks by developing a C++ based tool for deadlock detection and prevention.  The core of the project lies in implementing and comparing two prominent deadlock handling methods: Resource Allocation Graph (RAG) and Wait-For Graph (WFG).  Understanding and mitigating deadlocks is crucial for ensuring system stability and responsiveness in multi-process environments. For instance, in a database system, if Transaction A holds a lock on Table 1 and waits for a lock on Table 2, while Transaction B holds a lock on Table 2 and waits for Table 1, a deadlock can halt transaction processing, highlighting the real-world importance of this project.

**2. Project Objectives**

*   Develop a practical tool for deadlock detection in simulated multi-process systems using both the Resource Allocation Graph and Wait-For Graph methodologies. This allows for comparative analysis of these methods.
    Example in Real-World Application: In a cloud service environment, detecting deadlocks is essential to prevent service unavailability caused by virtual machines or containers blocking each other.
*   Implement and analyze core algorithms for resource allocation and process dependency analysis.  The project employs Depth First Search (DFS) for cycle detection in both RAG and WFG, providing a hands-on understanding of these algorithms.
    *   Example in Real-World Application:  In real-time systems like air traffic control, timely deadlock detection is vital to prevent system failures that could have catastrophic consequences.
*   Enhance system reliability by integrating deadlock prevention mechanisms. The project incorporates Resource Ordering for RAG and Process Ordering for WFG, demonstrating proactive approaches to deadlock avoidance.
    *   Example in Real-World Application:  In banking systems or e-commerce platforms, preventing deadlocks ensures smooth and uninterrupted transaction processing, maintaining customer trust and business continuity.
*   Provide a strategy for resolving detected deadlocks through process termination. The tool allows for simulating deadlock resolution by terminating processes involved in a deadlock cycle, freeing up resources.
    *   Example in Real-World Application:  In a web server experiencing deadlock, a resolution strategy like process termination (of non-critical processes) can restore service functionality and prevent server crashes.

**3. Project Scope**

**Features and Functionalities**

*   Resource Allocation Graph (RAG) Method:
    *   Deadlock Detection: Implements RAG using adjacency matrices to model processes, resources, allocations, and requests. Utilizes DFS-based cycle detection to identify deadlocks.
    *   Deadlock Prevention: Incorporates Resource Ordering. Users can define a resource hierarchy, and the system enforces this order to prevent resource requests that could lead to deadlocks.
    *   Example:  Simulates deadlock scenarios in a manufacturing system where machines (processes) require tools and materials (resources), visualized through a RAG to understand dependencies.
*   Wait-For Graph (WFG) Method:
    *   Deadlock Detection: Implements WFG, focusing solely on process dependencies. Uses adjacency matrices and DFS to detect cycles, indicating deadlocks.
    *   Deadlock Prevention: Implements Process Ordering. Processes can only wait for processes with a higher ID, breaking potential deadlock cycles by restricting wait-for relationships.
    *   Example: Models transaction deadlocks in a database system using WFG, simplifying the analysis to process dependencies alone.
*   Deadlock Detection Algorithm:
    *   Depth First Search (DFS):  Employs DFS algorithm for efficient cycle detection in both RAG and WFG representations. The algorithm identifies back edges during graph traversal to detect cycles.
    *   Example:  Finding deadlock cycles in a hospital management system where patient admission, bed allocation, and doctor scheduling processes might become deadlocked.
*   Deadlock Resolution Strategy:
    *   Process Termination: Provides a deadlock resolution mechanism by allowing users to terminate processes involved in a detected deadlock cycle. This releases resources held by these processes, breaking the deadlock.
    *   Example:  Resolving deadlocks in a file sharing system by terminating a process that is part of a deadlock cycle, freeing up file locks.
*   User Interface:
    *   Console-Based Interaction:  Offers a user-friendly, console-based interface for system configuration and operation. Users can input system parameters, matrices, choose detection/prevention methods, and view results through the command line.
    *   Tabular Output: Presents system state information (resource tables, matrices, WFG adjacency matrix) and deadlock detection results in well-formatted tables for easy interpretation and analysis.
    *   Example: Allows students to easily simulate and test various deadlock scenarios by entering system parameters and matrices through a straightforward console interface.

Target Users

*   Operating System Developers and Researchers:
    *   Example:  For testing and validating deadlock handling mechanisms in operating system kernels and resource management modules.
*   Students and Professionals in Operating Systems and Concurrency:
    *   Example:  As an educational tool to learn and experiment with deadlock detection, prevention, and resolution techniques in academic settings and professional training.
*   Developers of Multi-threaded and Distributed Systems:
    *   Example:  For understanding and addressing deadlock risks in complex applications, distributed systems, and cloud environments where resource contention and process synchronization are critical concerns.

4. Technology Stack

*   Programming Language:
    *   C++: Chosen for its efficiency in implementing system-level algorithms and managing data structures. (Example:  C++ allows for efficient implementation of DFS and graph traversal algorithms, crucial for deadlock detection.)
*   Development Tools:
    *   Ubuntu (Linux): Used as the development operating system, providing a robust and open-source environment.
    *   GCC Compiler: The GNU Compiler Collection (GCC) is used to compile the C++ code into an executable.
    *   Visual Studio Code (VS Code): A lightweight but powerful code editor used for writing, editing, and debugging the C++ project. (Example:  VS Code's debugging capabilities are used to step through the deadlock detection logic and verify its correctness.)
*   Libraries:
    *   Standard Template Library (STL):  Extensively utilized for core data structures like `vector` (for matrices and dynamic arrays), `string`, and algorithms. (Example:  `std::vector` is used to represent adjacency matrices for graphs and store resource and process information.)
    *   `<iostream>`: For input and output operations, allowing interaction with the console.
    *   `<iomanip>`: For input/output manipulation, used to format tabular output and align text in the console.
    *   `<numeric>`: Contains numerical operations, like `iota` for initializing sequences, used in resource ordering.
    *   `<string>`: For string manipulation and handling text-based outputs and headers.
    *   `<set>`: For set data structure, potentially used for managing unique sets of processes or resources if needed (though not heavily used in current version).

5. Operating System Concepts Used

*   Process Management:
    *   Process Scheduling and Synchronization: The project directly deals with the challenges of process synchronization that can lead to deadlocks. It explores how uncontrolled resource requests and allocations among concurrent processes can result in system stagnation.
    *   Example:  Simulates scenarios where process scheduling decisions, without proper deadlock management, can lead to system-wide deadlocks, especially in systems with preemptive scheduling.
*   Resource Management:
    *   Resource Allocation Strategies: The project emphasizes the importance of resource allocation policies and how they directly impact the possibility of deadlocks. It demonstrates different strategies, including resource ordering for prevention and process termination for resolution.
    *   Example:  Illustrates how different resource allocation schemes, like those in memory management or device management, can be analyzed for deadlock potential using RAG and WFG.

6. Implementation Details

Core Algorithms and Logic:

*   Resource Allocation Graph (RAG):
    *   Graph Representation: Processes and resources are modeled as nodes in a directed graph. Edges represent current resource allocations (from resource to process) and pending resource requests (from process to resource).
    *   Adjacency Matrix:  The `adjMatrix` (adjacency matrix) in the `ResourceAllocationGraph` class efficiently stores the graph structure, enabling quick checks for edges and traversal during cycle detection.
    *   Cycle Detection using DFS: The `detectCycleUtil` function implements Depth First Search. It recursively explores the graph, marking visited nodes and tracking the recursion stack. A back edge (to a node already in the recursion stack) indicates a cycle, and hence a deadlock.
    *   Resource Ordering for Prevention: The `setResourceOrder` function allows users to define a resource order. The `requestResource` function then enforces this order, denying requests that violate it, thus preventing cyclic dependencies.

*   Wait-For Graph (WFG):
    *   Simplified Graph: WFG simplifies the model by only representing processes as nodes. An edge from process P<sub>i</sub> to P<sub>j</sub> in `waitGraph` indicates that P<sub>i</sub> is waiting for P<sub>j</sub>.
    *   Adjacency Matrix:  The `waitGraph` adjacency matrix in the `WaitForGraph` class stores the wait-for relationships between processes.
    *   Cycle Detection using DFS:  Similar to RAG, the `detectCycleUtil` function in `WaitForGraph` uses DFS to find cycles in the `waitGraph`. Cycles in WFG directly represent deadlocks.
    *   Process Ordering for Prevention: The `setPreventionMode(true)` enables process ordering. In `inputGraph`, the program enforces the rule that a process P<sub>i</sub> can only wait for P<sub>j</sub> if j > i, preventing cycles during graph construction.

Modules:

*   Input Handling: Functions like `inputMatrices()` in `ResourceAllocationGraph` and `inputGraph()` in `WaitForGraph` handle user input for system configuration. They prompt for the number of processes, resources, matrix data, and prevention settings.
*   Graph Construction: The `buildGraph()` function in `ResourceAllocationGraph` constructs the adjacency matrix (`adjMatrix`) based on the allocation and request matrices. In `WaitForGraph`, the `inputGraph()` function directly populates the `waitGraph` adjacency matrix based on user input.
*   Cycle Detection: The `detectCycleUtil()` function (in both `ResourceAllocationGraph` and `WaitForGraph`) implements the core DFS algorithm for cycle detection.
*   Output Module:  Functions like `printTableHeader()`, `printTableRow()`, `printTableFooter()`, `printMatrixTable()`, `printResourceInstancesTable()`, `printGraphRepresentation()` (in `ResourceAllocationGraph`), `printWaitForGraphTable()`, and `printGraph()` (in `WaitForGraph`) are responsible for displaying system information and deadlock detection results in a formatted way on the console.
*   Deadlock Resolution: The `resolveDeadlock()` function in `ResourceAllocationGraph` implements process termination as a resolution strategy.
*   Deadlock Prevention: `setResourceOrder()` and `requestResource()` in `ResourceAllocationGraph` together implement resource ordering.  `setPreventionMode()` and the modified `inputGraph()` in `WaitForGraph` implement process ordering.
*   Resource Request and Release (RAG Prevention Mode): `requestResource()` and `releaseResource()` functions in `ResourceAllocationGraph` provide the operational interface for resource management under resource ordering prevention.

Code Snippet (Deadlock Detection in Wait-For Graph)

```cpp
bool WaitForGraph::detectDeadlock() {
    cout << "\n-------- Wait-For Graph Deadlock Detection Process --------\n";
    printWaitForGraphTable(); // Output Wait-For Graph table
    printGraph();           // Output Wait-For Graph representation
    vector<bool> visited(numProcesses, false);
    vector<bool> recStack(numProcesses, false);
    vector<int> cycleNodes;
    int cycleStart = -1;

    for (int i = 0; i < numProcesses; i++) {
        if (!visited[i] && detectCycleUtil(i, visited, recStack, cycleNodes, cycleStart)) {
            cout << "\n Deadlock Detected in Wait-For Graph! \n";
            cout << "Deadlock cycle: ";
            for (size_t j = 0; j < cycleNodes.size(); ++j) {
                cout << "P" << cycleNodes[j];
                if (j < cycleNodes.size() - 1) {
                    cout << " -> ";
                }
            }
            cout << " -> P" << cycleStart << endl;
            char killChoice;
            cout << "Deadlock detected in Wait-For Graph. Terminate program? (y/n): ";
            cin >> killChoice;
            if (killChoice == 'n' || killChoice == 'N') {
                cout << "-------- Wait-For Graph Deadlock Detection Process Completed --------\n";
                return true;
            }
            else
            {
                cout << "-------- Wait-For Graph Deadlock Detection Process Completed --------\n";
                return false;
            }
        }
    }
    cout << "\nNo deadlock detected in Wait-For Graph.\n";
    cout << "-------- Wait-For Graph Deadlock Detection Process Completed --------\n";
    return false;
}
Use code with caution.
Markdown
7. Conclusion

Project Outcomes

Successful Implementation of Deadlock Detection: The project successfully implements and demonstrates deadlock detection using both Resource Allocation Graph and Wait-For Graph techniques.

Example: Demonstrated successful deadlock detection in a simulated university management system where resource contention in scheduling and database access is modeled.

Development of a Console-Based Analysis Tool: A user-friendly console tool has been developed, enabling users to easily simulate, configure, and analyze process-resource allocation scenarios to understand and detect deadlocks.

Example: Created a tool that allows users to simulate deadlock scenarios in a hospital appointment scheduling system, providing insights into potential scheduling bottlenecks.

Integration of Deadlock Resolution and Prevention Strategies: The project integrates process termination as a deadlock resolution strategy and resource/process ordering as prevention mechanisms, providing a comprehensive approach to deadlock management.

Example: Implemented and tested deadlock prevention strategies in a simulated online gaming server, reducing the likelihood of server stalls due to deadlocks and improving user experience.

Comparative Analysis of RAG and WFG: The project provides a platform for comparing the effectiveness and complexity of Resource Allocation Graph and Wait-For Graph methods in different deadlock scenarios.

Key Learnings

Deep Understanding of Deadlock Detection Algorithms: Gained a practical and in-depth understanding of deadlock detection algorithms, specifically the implementation and application of Depth First Search for cycle detection.

Example: Acquired hands-on experience in applying cycle detection algorithms within the context of operating system resource management, enhancing theoretical knowledge with practical implementation.

Practical Application of Graph Theory in Operating Systems: The project provided a valuable practical application of graph theory concepts in the context of operating systems, demonstrating how graphs can effectively model process and resource dependencies for deadlock analysis.

Example: Learned to map real-world system dependencies into abstract graph representations, a skill applicable to broader software engineering and system design domains.

Enhanced Problem-Solving Skills in Process Synchronization and Resource Allocation: The project significantly improved problem-solving skills related to process synchronization and resource allocation, particularly in addressing the challenges of deadlock management in concurrent systems.

Example: Developed enhanced skills in designing and optimizing multi-threaded applications to minimize deadlock risks, crucial for developing efficient and reliable concurrent software, including AI models and high-performance applications.

This project lays a strong foundation for further exploration of advanced deadlock management techniques, including more sophisticated resolution strategies like resource preemption and rollback, and advanced prevention methods suitable for modern distributed and concurrent systems, such as in blockchain technologies or large-scale distributed databases.
