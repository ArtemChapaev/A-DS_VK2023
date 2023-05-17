#include "SetGraph.hpp"

#include <cassert>
#include <vector>
#include <set>

SetGraph::SetGraph(int vertexCount) {
    adjLists.resize(vertexCount);
    prevAdjLists.resize(vertexCount);
}

SetGraph::SetGraph(const IGraph &graph) {
    adjLists.resize(graph.VerticesCount());
    prevAdjLists.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        for (auto j: graph.GetNextVertices(i)) {
            adjLists[i].insert(j);
        }

        for (auto j: graph.GetPrevVertices(i)) {
            prevAdjLists[i].insert(j);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjLists.size());
    assert(to >= 0 && to < adjLists.size());

    adjLists[from].insert(to);
    prevAdjLists[to].insert(from);
}

int SetGraph::VerticesCount() const {
    return adjLists.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    std::vector<int> nextVertices;

    for (int next : adjLists[vertex]) {
        nextVertices.push_back(next);
    }
    return nextVertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    std::vector<int> prevVertices;

    for (int prev : prevAdjLists[vertex]) {
        prevVertices.push_back(prev);
    }
    return prevVertices;
}
