#include "ArcGraph.hpp"

#include <cassert>
#include <vector>

ArcGraph::ArcGraph(int vertexCount) : verticesCount(vertexCount) {}

ArcGraph::ArcGraph(const IGraph &graph) : verticesCount(graph.VerticesCount()) {
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        for (int j: graph.GetNextVertices(i)) {
            edges.emplace_back(i, j);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < verticesCount);
    assert(to >= 0 && to < verticesCount);

    edges.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    return verticesCount;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < verticesCount);
    std::vector<int> nextVertices;

    for (auto edge: edges) {
        if (edge.from == vertex) {
            nextVertices.push_back(edge.to);
        }
    }
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < verticesCount);
    std::vector<int> prevVertices;

    for (auto edge: edges) {
        if (edge.to == vertex) {
            prevVertices.push_back(edge.from);
        }
    }
    return prevVertices;
}
