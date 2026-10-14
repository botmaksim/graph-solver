# GraphSolver: Interactive Graph Theory & Network Flow IDE

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Qt](https://img.shields.io/badge/Qt-GUI%20%7C%20GraphicsScene-green.svg)](https://www.qt.io/)
[![GoogleTest](https://img.shields.io/badge/GoogleTest-Unit%20Tests-brightgreen.svg)](https://github.com/google/googletest)
[![CMake](https://img.shields.io/badge/CMake-3.14%2B-blue.svg)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

GraphSolver is an algorithmic workbench and visual development environment for graph theory and network flow analysis. Built with C++17 and Qt, the application isolates core solver logic into verifiable modules backed by a GoogleTest suite, paired with an interactive 2D canvas for topology editing and execution visualization.

---

## Table of Contents
1. [Algorithmic Solvers](#algorithmic-solvers)
2. [Interactive Capabilities](#interactive-capabilities)
3. [System Architecture](#system-architecture)
4. [Prerequisites](#prerequisites)
5. [Build & Installation](#build--installation)
6. [Testing](#testing)
7. [Directory Structure](#directory-structure)
8. [License](#license)

---

## Algorithmic Solvers

| Problem Domain | Algorithm | Time Complexity | Implementation Notes |
| :--- | :--- | :--- | :--- |
| **Maximum Flow** | Dinic Algorithm | $\mathcal{O}(V^2 E)$ | Layered network construction with blocking flow augmentation |
| **Minimum-Cost Flow** | Successive Shortest Path | $\mathcal{O}(F \cdot E \log V)$ | Cycle-canceling dual updates via Dijkstra with potentials |
| **Subgraph Isomorphism** | VF2 Algorithm | Sub-exponential | State-space search with depth-first branch and bound pruning |
| **Graph Coloring** | DSatur Algorithm | Heuristic $\mathcal{O}(V^2)$ | Vertex selection by dynamic degree of saturation |
| **Bipartite Matching** | Augmenting Paths | $\mathcal{O}(V \cdot E)$ | Maximum cardinality matching using alternating path searches |
| **Shortest Paths** | Dijkstra Algorithm | $\mathcal{O}((V + E) \log V)$ | Non-negative edge weight evaluation with binary heap priority queues |
| **Minimum Spanning Forest** | Kruskal Algorithm | $\mathcal{O}(E \log E)$ | Disjoint Set Union with path compression and union by rank |
| **Eulerian Traversal** | Hierholzer Algorithm | $\mathcal{O}(V + E)$ | Linear circuit and path reconstruction |
| **Connectivity** | Kosaraju & BFS | $\mathcal{O}(V + E)$ | Strongly and weakly connected component decomposition |

---

## Interactive Capabilities

* **Topological Canvas**: Direct manipulation of weighted directed and undirected vertices and edges within a `QGraphicsScene` viewport.
* **Force-Directed Layout**: Spring-embedder physical layout simulation for automated node untangling and cluster stabilization.
* **Transactional State**: Command pattern implementation supporting complete undo and redo histories across all structural edits.
* **Vector Diagram Export**: Render canvas state directly to high-resolution PNG raster formats for technical publications and reports.
* **Structured Diagnostics**: Runtime logging layer wrapped around `spdlog`, supporting dynamic console redirection and disk persistence.

---

## System Architecture

The project enforces strict separation between algorithmic computation and graphical presentation:

1. **Core Domain**: Solver implementations including maxflowsolver, mincostflowsolver, isomorphismsolver, and coloringsolver operate on primitive adjacency matrices and incidence structures without UI dependencies.
2. **Command Stack**: The `commands` subsystem encapsulates all canvas actions into discrete executable operations, maintaining state integrity across modifications.
3. **View Layer**: The `MyView` and `MyGraphicsScene` components handle viewport transformations, zoom, pan, and real-time vertex displacement.

---

## Prerequisites

Provision the following dependencies on your build host:

* **Compiler**: GCC 9+ or Clang 10+ with C++17 support
* **CMake**: Version 3.14 or higher
* **Qt**: Qt 5.12+ or Qt 6.x via qtbase5-dev or qt6-base-dev packages
* **Logging**: `spdlog` development headers
* **Testing**: GoogleTest library

On Debian or Ubuntu derivatives:
```bash
sudo apt update
sudo apt install -y build-essential cmake qtbase5-dev libspdlog-dev libgtest-dev
```

---

## Build & Installation

1. **Clone the Repository**
   ```bash
   git clone https://github.com/botmaksim/graph-solver.git
   cd graph-solver/graph-solver
   ```

2. **Configure and Compile**
   ```bash
   cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
   cmake --build build -j$(nproc)
   ```

3. **Launch the Application**
   ```bash
   ./build/graph-solver
   ```

---

## Testing

The algorithmic engine is covered by an automated unit testing suite utilizing GoogleTest:

```bash
cd build
ctest --output-on-failure
```

To run individual solver test binaries directly:
```bash
./tests/solver_tests
```

---

## Directory Structure

```text
graph-solver/
├── CMakeLists.txt                 # Project build configuration
├── main.cpp                       # Application entry point and CLI parser
├── mainwindow.cpp/.h/.ui          # MainWindow interface and menu actions
├── mygraphicsscene.cpp/.h         # Interactive canvas logic
├── myview.cpp/.h/.ui              # Viewport transformations and input handling
├── commands.cpp/.h                # Command pattern undo/redo stack
├── TemplateStack.h                # Generic stack container implementation
├── LoggerWrapper.cpp/.h           # spdlog logging abstraction
├── algorithmeditor.cpp/.h/.ui     # Parameter configuration modal
├── vertexeditor.cpp/.h/.ui        # Vertex attribute modification dialog
├── edgeeditor.cpp/.h/.ui          # Edge weight and capacity dialog
├── graph.cpp/.h                   # Core graph adjacency data structures
├── vertex.cpp/.h                  # Vertex entity definition
├── edge.cpp/.h                    # Directed/undirected edge definition
├── maxflowsolver.cpp/.h           # Dinic maximum flow solver
├── mincostflowsolver.cpp/.h       # Successive shortest path min-cost flow
├── isomorphismsolver.cpp/.h       # VF2 subgraph isomorphism engine
├── coloringsolver.cpp/.h          # DSatur graph coloring engine
├── matchingsolver.cpp/.h          # Bipartite matching solver
├── shortestpathsolver.cpp/.h      # Dijkstra shortest path solver
├── minspanningforestsolver.cpp/.h # Kruskal minimum spanning tree solver
├── euleriansolver.cpp/.h          # Hierholzer Eulerian traversal
├── connectivitysolver.cpp/.h      # Kosaraju connectivity analyzer
├── layoutsolver.cpp/.h            # Force-directed layout physics simulation
└── tests/                         # GoogleTest test cases
    └── ...
```

---

## License

This project is open-source software licensed under the [MIT License](LICENSE).
