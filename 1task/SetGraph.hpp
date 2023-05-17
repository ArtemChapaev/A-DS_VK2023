#pragma once

#include "IGraph.hpp"

#include <vector>
#include <set>

// хранит граф в виде массива сбалансированных деревьев поиска

// выбрал деревья, тк решил гарантированно всегда возвращать упорядоченные вектора
// в GetNextVertices(), GetPrevVertices()
class SetGraph : public IGraph {
public:
    SetGraph(int vertexCount);
    SetGraph(const IGraph &graph);

    // IGraph
    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::set<int>> adjLists;
    std::vector<std::set<int>> prevAdjLists;
};
