#include "MatrixGraph.hpp"

#include <cassert>
#include <vector>

MatrixGraph::MatrixGraph(int vertexCount) {
    adjMatrix.resize(vertexCount);
    for (std::vector<int> &row: adjMatrix) {
        row.resize(vertexCount);
    }
}

MatrixGraph::MatrixGraph(const IGraph &graph) {
    adjMatrix.resize(graph.VerticesCount());
    for (std::vector<int> &row: adjMatrix) {
        row.resize(graph.VerticesCount());
    }

    for (int i = 0; i < graph.VerticesCount(); ++i) {
        for (auto j: graph.GetNextVertices(i)) {
            adjMatrix[i][j] = 1;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjMatrix.size());
    assert(to >= 0 && to < adjMatrix.size());

    adjMatrix[from][to] = 1;
}

int MatrixGraph::VerticesCount() const {
    return adjMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjMatrix.size());
    std::vector<int> nextVertices;

    for (int i = 0; i < VerticesCount(); ++i) {
        if (adjMatrix[vertex][i]) {
            nextVertices.push_back(i);
        }
    }
    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjMatrix.size());
    std::vector<int> prevVertices;

    for (int i = 0; i < VerticesCount(); ++i) {
        if (adjMatrix[i][vertex]) {
            prevVertices.push_back(i);
        }
    }
    return prevVertices;
}
