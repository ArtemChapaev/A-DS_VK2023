// 3. Требуется отыскать самый выгодный маршрут между городами.
// Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.

// Формат входных данных.
// Первая строка содержит число N – количество городов.
// Вторая строка содержит число M - количество дорог.
// Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
// Последняя строка содержит маршрут (откуда и куда нужно доехать).

// Формат выходных данных.
// Вывести длину самого выгодного маршрута.

#include <iostream>
#include <set>
#include <vector>

// хранит граф в виде массива списков смежности
class ListGraph {
public:
    explicit ListGraph(int vertexCount);

    void AddEdge(int from, int to, int weight);
    int VerticesCount() const;
    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const;

private:
    std::vector<std::vector<std::pair<int, int>>> adjLists;
};

int GetPathLength(const ListGraph &graph, int from, int to);

int main() {
    int v, n;
    std::cin >> v >> n;
    auto graph = new ListGraph(v);

    int from, to, weight;
    for (int i = 0; i < n; ++i) {
        std::cin >> from >> to >> weight;
        graph->AddEdge(from, to, weight);
    }

    std::cin >> from >> to;
    std::cout << GetPathLength(*graph, from, to);

    delete graph;
    return 0;
}

int GetPathLength(const ListGraph &graph, int from, int to) {
    std::set<std::pair<int, int>> DijkstraQ;
    std::vector<int> dist(graph.VerticesCount(), INT32_MAX);

    dist[from] = 0;
    DijkstraQ.insert({0, from});

    while (!DijkstraQ.empty()) {
        int current = DijkstraQ.begin()->second;
        DijkstraQ.erase(DijkstraQ.begin());

        // first - vertex, second - length
        for (auto next: graph.GetNextVertices(current)) {
            if (dist[next.second] == INT32_MAX) {
                dist[next.second] = dist[current] + next.first;
                DijkstraQ.insert(next);
            } else if (dist[current] + next.first < dist[next.second]) {
                DijkstraQ.erase(next);
                dist[next.second] = dist[current] + next.first;
                DijkstraQ.insert({dist[current] + next.first, next.second});
            }
        }
    }

    return dist[to];
}

ListGraph::ListGraph(int vertexCount) {
    adjLists.resize(vertexCount);
}

void ListGraph::AddEdge(int from, int to, int weight) {
    adjLists[from].push_back({weight, to});
    adjLists[to].push_back({weight, from});
}

int ListGraph::VerticesCount() const {
    return adjLists.size();
}

std::vector<std::pair<int, int>> ListGraph::GetNextVertices(int vertex) const {
    return adjLists[vertex];
}
