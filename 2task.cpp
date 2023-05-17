#include <iostream>

#include <cassert>
#include <queue>
#include <vector>

// хранит граф в виде списков смежности
class ListGraph {
public:
    explicit ListGraph(int vertexCount);

    void AddEdge(int from, int to);
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;

    int CountDifferentShortestPaths(int from, int to) const;

private:
    std::vector<std::vector<int>> adjLists;
};

int main() {
    int v, n;
    std::cin >> v >> n;
    auto graph = new ListGraph(v);

    int from, to;
    for (int i = 0; i < n; ++i) {
        std::cin >> from >> to;
        graph->AddEdge(from, to);
    }

    std::cin >> from >> to;
    std::cout << graph->CountDifferentShortestPaths(from, to);

    delete graph;
    return 0;
}


ListGraph::ListGraph(int vertexCount) {
    adjLists.resize(vertexCount);
}

void ListGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjLists.size());
    assert(to >= 0 && to < adjLists.size());

    adjLists[from].push_back(to);
    adjLists[to].push_back(from);
}

int ListGraph::VerticesCount() const {
    return adjLists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return adjLists[vertex];
}

int ListGraph::CountDifferentShortestPaths(int from, int to) const {
    // store count of paths 'from'-i, where i is vertex of graph
    std::vector<int> waveBfs(VerticesCount(), -1);
    std::vector<int> paths(VerticesCount(), 0);

    std::queue<int> bfsQ;
    bfsQ.push(from);
    paths[from] = 1;
    waveBfs[from] = 0;
    while (!bfsQ.empty()) {
        int current = bfsQ.front();
        bfsQ.pop();
        std::vector<int> adjList = GetNextVertices(current);
        for (int next: adjList) {
            if (waveBfs[next] == -1) {
                bfsQ.push(next);
                waveBfs[next] = waveBfs[current] + 1;
            }
            // if this path is shortest
            if (waveBfs[next] > waveBfs[current]) {
                paths[next] += paths[current];
            }
        }
    }
    return paths[to];
}
