#include <array>
#include <algorithm>
#include <cassert>
#include <queue>
#include <iostream>
#include <string>
#include <set>
#include <unordered_map>

const int SideSize = 4;
const int FieldSize = SideSize * SideSize;
const std::array<char, FieldSize> FinishField({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0});

class GameState {
public:
    explicit GameState(const std::array<char, FieldSize> &_field);

    bool CanMoveLeft() const;
    bool CanMoveUp() const;
    bool CanMoveRight() const;
    bool CanMoveDown() const;

    GameState MoveLeft() const;
    GameState MoveUp() const;
    GameState MoveRight() const;
    GameState MoveDown() const;

    bool IsFinish() const;

    char GetFieldValue(size_t pos) const;

    bool IsSolvable() const;

    bool operator<(const GameState &state) const {
        return field < state.field;
    }

    bool operator==(const GameState &state) const {
        return field == state.field;
    }

    bool operator!=(const GameState &state) const {
        return !(*this == state);
    }

private:
    std::array<char, FieldSize> field;
    size_t zeroPos;
};

namespace std {
    template<>
    struct less<std::pair<int, GameState>> {
        bool operator()(std::pair<int, GameState> lhs, std::pair<int, GameState> rhs) const {
            if (lhs.first == rhs.first) {
                return lhs.second < rhs.second;
            }
            return lhs.first < rhs.first;
        }
    };
}

struct StateHasher {
    size_t operator()(const GameState &state) const {
        size_t hash = 0;
        size_t p = 1;
        for (int i = 0; i < FieldSize; ++i) {
            p *= 137;
            hash += state.GetFieldValue(i) * p;
        }
        return hash;
    }
};

std::string Get15thSolution(const GameState &state);

int main() {
    std::array<char, FieldSize> field{};
    int n = 0;
    for (int i = 0; i < FieldSize; ++i) {
        std::cin >> n;
        field[i] = static_cast<char>(n);
    }

    GameState state(field);
    if (!state.IsSolvable()) {
        std::cout << -1;
    } else {
        std::string result = Get15thSolution(state);
        std::cout << result.size() << std::endl << result;
    }
    return 0;
}

std::pair<int, int> GetCoords(int block) {
    char block_x, block_y;
    if (block % SideSize == 0) {
        block_x = 4;
        block_y = static_cast<char>(block / static_cast<char>(4));
    } else {
        block_x = static_cast<char>(block % SideSize);
        block_y = static_cast<char>(1 + block / 4);
    }
    return std::make_pair(block_x, block_y);
}

int GetDiffWithFinishField(const GameState &state) {
    int manhattanDist = 0;
    for (int i = 0; i < FieldSize; ++i) {
        // finishBlock show which element should be here for FinishField
        char finishBlock = static_cast<char>((i + 1) % FieldSize);
        auto finishBlockCoords = GetCoords(finishBlock);

        // currentBlock show which element is located here
        char currentBlock = state.GetFieldValue(i);
        auto currentBlockCoords = GetCoords(currentBlock);

        manhattanDist += std::abs(finishBlockCoords.first - currentBlockCoords.first);
        manhattanDist += std::abs(finishBlockCoords.second - currentBlockCoords.second);
    }
    return manhattanDist;
}

std::string Get15thSolution(const GameState &state) {
    std::set<std::pair<int, GameState>, std::less<>> AStarQ;
    AStarQ.insert({0, state});

    std::unordered_map<GameState, char, StateHasher> visited;
    visited[state] = 'S';
    while (!AStarQ.empty()) {
        GameState current = AStarQ.begin()->second;
        AStarQ.erase(AStarQ.begin());
        if (current.IsFinish()) {
            break;
        }
        if (current.CanMoveLeft()) {
            GameState newState = current.MoveLeft();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'L';
                AStarQ.insert({GetDiffWithFinishField(newState), newState});
            }
        }
        if (current.CanMoveUp()) {
            GameState newState = current.MoveUp();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'U';
                AStarQ.insert({GetDiffWithFinishField(newState), newState});
            }
        }
        if (current.CanMoveRight()) {
            GameState newState = current.MoveRight();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'R';
                AStarQ.insert({GetDiffWithFinishField(newState), newState});
            }
        }
        if (current.CanMoveDown()) {
            GameState newState = current.MoveDown();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'D';
                AStarQ.insert({GetDiffWithFinishField(newState), newState});
            }
        }
    }

    std::string result;
    GameState current(FinishField);
    char move = visited[current];
    while (move != 'S') {
        result += move;
        switch (move) {
            case 'L':
                current = current.MoveRight();
                break;
            case 'U':
                current = current.MoveDown();
                break;
            case 'R':
                current = current.MoveLeft();
                break;
            case 'D':
                current = current.MoveUp();
                break;
        }
        move = visited[current];
    }

    std::reverse(result.begin(), result.end());
    return result;
}

GameState::GameState(const std::array<char, FieldSize> &_field) : field(_field), zeroPos(-1) {
    for (size_t i = 0; i < field.size(); ++i) {
        if (field[i] == 0) {
            zeroPos = i;
            break;
        }
    }
    assert(zeroPos != -1);
}

bool GameState::CanMoveLeft() const {
    return zeroPos % SideSize < SideSize - 1;
}

bool GameState::CanMoveUp() const {
    return zeroPos < FieldSize - SideSize;
}

bool GameState::CanMoveRight() const {
    return zeroPos % SideSize != 0;
}

bool GameState::CanMoveDown() const {
    return zeroPos >= SideSize;
}

GameState GameState::MoveLeft() const {
    assert(CanMoveLeft());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos + 1]);
    ++newState.zeroPos;
    return newState;
}

GameState GameState::MoveUp() const {
    assert(CanMoveUp());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos + SideSize]);
    newState.zeroPos += SideSize;
    return newState;
}

GameState GameState::MoveRight() const {
    assert(CanMoveRight());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos - 1]);
    --newState.zeroPos;
    return newState;
}

GameState GameState::MoveDown() const {
    assert(CanMoveDown());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos - SideSize]);
    newState.zeroPos -= SideSize;
    return newState;
}

bool GameState::IsFinish() const {
    return field == FinishField;
}

char GameState::GetFieldValue(size_t pos) const {
    assert(pos >= 0 && pos < FieldSize);
    return field[pos];
}

bool GameState::IsSolvable() const {
    int inversionsCount = 0;
    int zeroLine = zeroPos / SideSize + 1;

    for (int i = 0; i < FieldSize; ++i) {
        char block = field[i];
        if (block == 0) {
            continue;
        }

        // count inversions for i'th block in order
        for (int j = 0; j < i; ++j) {
            if (field[j] != 0 && field[j] > block) {
                ++inversionsCount;
            }
        }
    }
    return (inversionsCount + zeroLine) % 2 == 0;
}
