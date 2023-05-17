#include "ArcGraph.hpp"
#include "IGraph.hpp"
#include "ListGraph.hpp"
#include "MatrixGraph.hpp"
#include "SetGraph.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

using VisitFn = std::function<void(int)>;
using InitFn = std::function<IGraph *()>;

void BFS(const IGraph &graph, int vertex, const VisitFn& visit) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> bfsQ;
    bfsQ.push(vertex);
    visited[vertex] = true;
    while (!bfsQ.empty()) {
        int current = bfsQ.front();
        bfsQ.pop();
        visit(current);
        std::vector<int> adjList = graph.GetNextVertices(current);
        // sort because ArcGraph and ListGraph can return non-ordered vector
        std::sort(adjList.begin(), adjList.end());
        for (int next : adjList) {
            if (!visited[next]) {
                bfsQ.push(next);
                visited[next] = true;
            }
        }
    }
}

void run(const InitFn& initFunc, std::ostream &output) {
    std::vector<IGraph *> graphs;
    graphs.push_back(initFunc());

    // create copy of every type of graphs
    graphs.push_back(new ListGraph(*graphs[0]));
    graphs.push_back(new MatrixGraph(*graphs[0]));
    graphs.push_back(new SetGraph(*graphs[0]));
    graphs.push_back(new ArcGraph(*graphs[0]));

    // for each graph call BFS
    for (IGraph *graph: graphs) {
        BFS(*graph, 4, [&output](int vertex) { output << vertex << " "; });
        output << "end ";
    }

    for (IGraph *graph: graphs) {
        delete graph;
    }
}

// we check that all BFS outputs are equal
void checkOutput(std::stringstream &output) {
    // GraphBFS stores order of every graph's BFS
    std::vector<std::vector<int>> GraphBFS(5);

    int i = 0;
    std::string lexeme;
    while (output >> lexeme) {
        if (lexeme == "end") {
            ++i;
            continue;
        }
        GraphBFS[i].push_back(std::stoi(lexeme));
    }

    assert(GraphBFS[0] == GraphBFS[1]);
    assert(GraphBFS[0] == GraphBFS[2]);
    assert(GraphBFS[0] == GraphBFS[3]);
    assert(GraphBFS[0] == GraphBFS[4]);
}

IGraph *initListGraph() {
    IGraph *listGraph = new ListGraph(5);
    listGraph->AddEdge(1, 2);
    listGraph->AddEdge(2, 3);
    listGraph->AddEdge(2, 4);
    listGraph->AddEdge(4, 3);
    listGraph->AddEdge(4, 1);

    return listGraph;
}

IGraph *initMatrixGraph() {
    IGraph *matrixGraph = new MatrixGraph(5);
    matrixGraph->AddEdge(3, 4);
    matrixGraph->AddEdge(4, 1);
    matrixGraph->AddEdge(2, 3);
    matrixGraph->AddEdge(1, 2);
    matrixGraph->AddEdge(4, 2);

    return matrixGraph;
}

IGraph *initSetGraph() {
    IGraph *setGraph = new SetGraph(5);
    setGraph->AddEdge(2, 1);
    setGraph->AddEdge(3, 4);
    setGraph->AddEdge(4, 3);
    setGraph->AddEdge(1, 4);
    setGraph->AddEdge(4, 2);

    return setGraph;
}

IGraph *initArcGraph() {
    IGraph *arcGraph = new ArcGraph(5);
    arcGraph->AddEdge(1, 2);
    arcGraph->AddEdge(1, 3);
    arcGraph->AddEdge(1, 4);
    arcGraph->AddEdge(2, 3);
    arcGraph->AddEdge(4, 1);

    return arcGraph;
}

int main() {
    std::vector<InitFn> initFuncs;
    initFuncs.push_back(initListGraph);
    initFuncs.push_back(initMatrixGraph);
    initFuncs.push_back(initSetGraph);
    initFuncs.push_back(initArcGraph);

    // for every type of graph we use test
    for (const InitFn& init: initFuncs) {
        std::stringstream output;
        run(init, output);
        std::cout << output.str() << std::endl;
        checkOutput(output);
    }
    return 0;
}
