#pragma once

#include "IGraph.hpp"
#include <vector>

// хранит граф в виде одного массива пар {from, to}.
class ArcGraph : public IGraph {
public:
    ArcGraph(int vertexCount);
    ArcGraph(const IGraph &graph);

    // IGraph
    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    struct Edge {
        Edge(int from, int to) : from(from), to(to) {}

        int from;
        int to;
    };

    std::vector<Edge> edges;
    int verticesCount;
};
