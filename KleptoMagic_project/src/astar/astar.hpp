/*
 * Header only library of A* algorithm by Bensuperpc
 * MIT License
 */

#ifndef BEN_ASTAR_HPP
#define BEN_ASTAR_HPP

#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename CoordinateType>
concept ArithmeticType = std::is_arithmetic<CoordinateType>::value;

template <typename CoordinateType>
concept IntegerType = std::is_integral<CoordinateType>::value;

template <typename CoordinateType>
concept FloatingPointType = std::is_floating_point<CoordinateType>::value;

namespace AStar {

template <IntegerType CoordinateType = int32_t>
class Vec2 {
   public:
    Vec2() = default;
    Vec2(CoordinateType x_, CoordinateType y_) : x(x_), y(y_) {}

    bool operator==(const Vec2& pos) const noexcept { return (x == pos.x && y == pos.y); }
    Vec2 operator=(const Vec2& pos) noexcept {
        x = pos.x;
        y = pos.y;
        return *this;
    }
    Vec2 operator+(const Vec2& pos) noexcept { return {x + pos.x, y + pos.y}; }
    Vec2 operator-(const Vec2& pos) noexcept { return {x - pos.x, y - pos.y}; }
    Vec2 operator*(const Vec2& pos) noexcept { return {x * pos.x, y * pos.y}; }
    Vec2 operator/(const Vec2& pos) noexcept { return {x / pos.x, y / pos.y}; }
    struct hash {
        size_t operator()(const Vec2& pos) const noexcept { return std::hash<size_t>()(pos.x ^ (pos.y << 4)); }
    };

    CoordinateType x = 0;
    CoordinateType y = 0;
};
typedef Vec2<int32_t> Vec2i;

template <IntegerType CoordinateType = uint32_t>
class Node {
   public:
    explicit Node() : pos(Vec2i(0, 0)), parentNode(nullptr) {}
    explicit Node(const Vec2i& pos, Node* parent = nullptr) : pos(pos), parentNode(parent) {}
    explicit Node(const Vec2i& pos, const CoordinateType pathCost, const CoordinateType heuristicCost, Node* parent = nullptr)
        : pathCost(pathCost), heuristicCost(heuristicCost), pos(pos), parentNode(parent) {}
    inline CoordinateType getTotalCost() const noexcept { return pathCost + heuristicCost; }
    struct hash {
        size_t operator()(const Node* node) const noexcept { return std::hash<size_t>()(node->pos.x ^ (node->pos.y << 4)); }
    };

    CoordinateType pathCost = 0;
    CoordinateType heuristicCost = 0;
    Vec2i pos = {0, 0};
    Node* parentNode = nullptr;
};

namespace Heuristic {
static inline Vec2i deltaVec(const Vec2i& source, const Vec2i& target) noexcept {
    return {std::abs(source.x - target.x), std::abs(source.y - target.y)};
}

static inline uint32_t manhattan(const Vec2i& source, const Vec2i& target, const uint32_t weight) noexcept {
    auto delta = deltaVec(source, target);
    return weight * (delta.x + delta.y);
}

static inline uint32_t octagonal(const Vec2i& source, const Vec2i& target, const uint32_t weight) noexcept {
    auto delta = deltaVec(source, target);
    return weight * (delta.x + delta.y) + (-6) * std::min(delta.x, delta.y);
}

static inline uint32_t euclidean(const Vec2i& source, const Vec2i& target, const uint32_t weight) noexcept {
    auto delta = deltaVec(source, target);
    return weight * static_cast<uint32_t>(std::sqrt(std::pow(delta.x, 2) + std::pow(delta.y, 2)));
}

static inline uint32_t chebyshev(const Vec2i& source, const Vec2i& target, const uint32_t weight) noexcept {
    auto delta = deltaVec(source, target);
    return weight * std::max(delta.x, delta.y);
}

static inline uint32_t euclideanNoSQR(const Vec2i& source, const Vec2i& target, const uint32_t weight) noexcept {
    auto delta = deltaVec(source, target);
    return weight * static_cast<uint32_t>(std::pow(delta.x, 2) + std::pow(delta.y, 2));
}

static constexpr uint32_t dijkstra([[maybe_unused]] const Vec2i& source,
                                   [[maybe_unused]] const Vec2i& target,
                                   const uint32_t weight = 0) noexcept {
    return 0;
}
};  // namespace Heuristic

template <IntegerType CoordinateType = uint32_t, bool enableDebug = false>
class AStarVirtual {
   public:
    explicit AStarVirtual()
        : _heuristicFunction(&Heuristic::euclidean),
          _directionsCount(4),
          _heuristicWeight(10),
          _mouvemementCost(10),
          _debugCurrentNode([](Node<CoordinateType>*) {}),
          _debugOpenNode([](Node<CoordinateType>*) {}) {
        _directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    }
    void setHeuristic(const std::function<uint32_t(Vec2i, Vec2i, uint32_t)>& heuristic) {
        _heuristicFunction = std::bind(heuristic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    }
    std::function<uint32_t(Vec2i, Vec2i, uint32_t)>& getHeuristic() noexcept { return _heuristicFunction; }

    void setHeuristicWeight(const uint32_t weight) noexcept { _heuristicWeight = weight; }
    uint32_t getHeuristicWeight() const noexcept { return _heuristicWeight; }

    void setDiagonalMovement(const bool enableDiagonalMovement) noexcept {
        _directionsCount = (enableDiagonalMovement ? _directions.size() : _directions.size() / 2);
    }

    void setMouvemementCost(const size_t cost) noexcept { _mouvemementCost = cost; }
    size_t getMouvemementCost() const noexcept { return _mouvemementCost; }

    void setCustomDirections(const std::vector<Vec2i>& directions) noexcept {
        _directions = directions;
        _directionsCount = static_cast<size_t>(directions.size());
    }

    std::vector<Vec2i>& getDirections() noexcept { return _directions; }

    void setDebugCurrentNode(const std::function<void(Node<CoordinateType>*)>& debugCurrentNode) noexcept { _debugCurrentNode = debugCurrentNode; }
    void setDebugOpenNode(const std::function<void(Node<CoordinateType>*)>& debugOpenNode) noexcept { _debugOpenNode = debugOpenNode; }

   protected:
    std::function<uint32_t(Vec2i, Vec2i, uint32_t)> _heuristicFunction;
    std::vector<Vec2i> _directions;
    size_t _directionsCount;
    CoordinateType _heuristicWeight;
    size_t _mouvemementCost = 10;

    // Only used if enableDebug is true
    std::function<void(Node<CoordinateType>*)> _debugCurrentNode;
    std::function<void(Node<CoordinateType>*)> _debugOpenNode;
};

template <IntegerType CoordinateType = uint32_t, bool enableDebug = false>
class AStar final : public AStarVirtual<CoordinateType, enableDebug> {
   public:
    explicit AStar() {}

    std::vector<Vec2i> findPath(const Vec2i source, const Vec2i& target) {
        if (target.x < 0 || target.x >= _worldSize.x || target.y < 0 || target.y >= _worldSize.y) {
            return {};
        }

        Node<CoordinateType>* currentNode = nullptr;

        static auto compareFn = [](const Node<CoordinateType>* a, const Node<CoordinateType>* b) { return a->getTotalCost() > b->getTotalCost(); };
        std::priority_queue<Node<CoordinateType>*, std::vector<Node<CoordinateType>*>, decltype(compareFn)> openNodeVecPQueue =
            std::priority_queue<Node<CoordinateType>*, std::vector<Node<CoordinateType>*>, decltype(compareFn)>(compareFn);

        std::unordered_map<Vec2i, Node<CoordinateType>*, Vec2i::hash> openNodeMap;
        std::unordered_map<Vec2i, Node<CoordinateType>*, Vec2i::hash> closedNodeMap;

        openNodeVecPQueue.push(new Node<CoordinateType>(source));
        openNodeMap.insert({source, openNodeVecPQueue.top()});

        while (!openNodeVecPQueue.empty()) {
            currentNode = openNodeVecPQueue.top();

            if constexpr (enableDebug) {
                AStarVirtual<CoordinateType, enableDebug>::_debugCurrentNode(currentNode);
            }

            if (currentNode->pos == target) {
                break;
            }

            openNodeVecPQueue.pop();
            openNodeMap.erase(currentNode->pos);
            closedNodeMap.insert({currentNode->pos, currentNode});

            for (size_t i = 0; i < AStarVirtual<CoordinateType, enableDebug>::_directionsCount; ++i) {
                Vec2i newPos = currentNode->pos + AStarVirtual<CoordinateType, enableDebug>::_directions[i];

                if (_obstacles.contains(newPos)) {
                    continue;
                }

                if (closedNodeMap.contains(newPos)) {
                    continue;
                }

                if (newPos.x < 0 || newPos.x >= _worldSize.x || newPos.y < 0 || newPos.y >= _worldSize.y) {
                    continue;
                }

                CoordinateType nextCost = currentNode->pathCost + AStarVirtual<CoordinateType, enableDebug>::_mouvemementCost;
                Node<CoordinateType>* nextNode = openNodeMap.find(newPos) != openNodeMap.end() ? openNodeMap[newPos] : nullptr;

                if (nextNode == nullptr) {
                    nextNode = new Node<CoordinateType>(newPos, currentNode);
                    nextNode->pathCost = nextCost;
                    nextNode->heuristicCost = static_cast<CoordinateType>(AStarVirtual<CoordinateType, enableDebug>::_heuristicFunction(
                        nextNode->pos, target, AStarVirtual<CoordinateType, enableDebug>::_heuristicWeight));
                    openNodeVecPQueue.push(nextNode);
                    openNodeMap.insert({nextNode->pos, nextNode});
                } else if (nextCost < nextNode->pathCost) {
                    nextNode->parentNode = currentNode;
                    nextNode->pathCost = nextCost;
                }

                if constexpr (enableDebug) {
                    AStarVirtual<CoordinateType, enableDebug>::_debugOpenNode(nextNode);
                }
            }
        }

        std::vector<Vec2i> path;

        if (currentNode->pos == target) [[likely]] {
            path.reserve(currentNode->getTotalCost() / 10);
            while (currentNode != nullptr) {
                path.push_back(currentNode->pos);
                currentNode = currentNode->parentNode;
            }
        }
        for (auto& [key, value] : openNodeMap) {
            delete value;
        }

        for (auto& [key, value] : closedNodeMap) {
            delete value;
        }

        return path;
    }

    void addObstacle(const Vec2i& pos) { _obstacles.insert(pos); }
    void removeObstacle(const Vec2i& pos) { _obstacles.erase(pos); }
    std::unordered_set<Vec2i, Vec2i::hash>& getObstacles() noexcept { return _obstacles; }

    void clear() { _obstacles.clear(); }
    void setWorldSize(const Vec2i& worldSize_) noexcept { _worldSize = worldSize_; }

   private:
    std::unordered_set<Vec2i, Vec2i::hash> _obstacles;
    Vec2i _worldSize = {0, 0};
};

// Fast AStar are faster than normal AStar but use more ram and direct access to the map
template <IntegerType CoordinateType = uint32_t, bool enableDebug = false, IntegerType MapElementType = uint32_t>
class AStarFast final : public AStarVirtual<CoordinateType, enableDebug> {
   public:
    explicit AStarFast() : _isObstacleFunction([](MapElementType value) { return value == 1; }) {}

    // Same as AStar::findPath() but use direct access to the map
    std::vector<Vec2i> findPath(const Vec2i& source, const Vec2i& target, const std::vector<MapElementType>& map, const Vec2i& worldSize) {
        if (target.x < 0 || target.x >= worldSize.x || target.y < 0 || target.y >= worldSize.y) {
            return {};
        }

        Node<CoordinateType>* currentNode = nullptr;

        static auto compareFn = [](const Node<CoordinateType>* a, const Node<CoordinateType>* b) { return a->getTotalCost() > b->getTotalCost(); };
        std::priority_queue<Node<CoordinateType>*, std::vector<Node<CoordinateType>*>, decltype(compareFn)> openNodeVecPQueue =
            std::priority_queue<Node<CoordinateType>*, std::vector<Node<CoordinateType>*>, decltype(compareFn)>(compareFn);
        std::unordered_map<Vec2i, Node<CoordinateType>*, Vec2i::hash> openNodeMap;
        std::unordered_map<Vec2i, Node<CoordinateType>*, Vec2i::hash> closedNodeMap;

        openNodeVecPQueue.push(new Node<CoordinateType>(source));
        openNodeMap.insert({source, openNodeVecPQueue.top()});

        while (!openNodeVecPQueue.empty()) {
            currentNode = openNodeVecPQueue.top();

            if constexpr (enableDebug) {
                AStarVirtual<CoordinateType, enableDebug>::_debugCurrentNode(currentNode);
            }

            if (currentNode->pos == target) {
                break;
            }

            openNodeVecPQueue.pop();
            openNodeMap.erase(currentNode->pos);
            closedNodeMap.insert({currentNode->pos, currentNode});

            for (size_t i = 0; i < AStarVirtual<CoordinateType, enableDebug>::_directionsCount; ++i) {
                Vec2i newPos = currentNode->pos + AStarVirtual<CoordinateType, enableDebug>::_directions[i];

                if (_isObstacleFunction(map[newPos.x + newPos.y * worldSize.x])) {
                    continue;
                }

                if (closedNodeMap.contains(newPos)) {
                    continue;
                }

                if (newPos.x < 0 || newPos.x >= worldSize.x || newPos.y < 0 || newPos.y >= worldSize.y) {
                    continue;
                }

                CoordinateType nextCost = currentNode->pathCost + AStarVirtual<CoordinateType, enableDebug>::_mouvemementCost;
                Node<CoordinateType>* nextNode = openNodeMap.find(newPos) != openNodeMap.end() ? openNodeMap[newPos] : nullptr;
                if (nextNode == nullptr) {
                    nextNode = new Node<CoordinateType>(newPos, currentNode);
                    nextNode->pathCost = nextCost;
                    nextNode->heuristicCost = static_cast<CoordinateType>(AStarVirtual<CoordinateType, enableDebug>::_heuristicFunction(
                        nextNode->pos, target, AStarVirtual<CoordinateType, enableDebug>::_heuristicWeight));
                    openNodeVecPQueue.push(nextNode);
                    openNodeMap.insert({nextNode->pos, nextNode});
                } else if (nextCost < nextNode->pathCost) [[likely]] {
                    nextNode->parentNode = currentNode;
                    nextNode->pathCost = nextCost;
                }

                if constexpr (enableDebug) {
                    AStarVirtual<CoordinateType, enableDebug>::_debugOpenNode(nextNode);
                }
            }
        }

        std::vector<Vec2i> path;

        if (currentNode->pos == target) [[likely]] {
            path.reserve(currentNode->getTotalCost() / 10);
            while (currentNode != nullptr) {
                path.push_back(currentNode->pos);
                currentNode = currentNode->parentNode;
            }
        }
        for (auto& [key, value] : openNodeMap) {
            delete value;
        }

        for (auto& [key, value] : closedNodeMap) {
            delete value;
        }

        return path;
    }
    void setObstacle(const std::function<bool(MapElementType)>& isObstacleFunction) noexcept { _isObstacleFunction = isObstacleFunction; }
    std::function<bool(MapElementType)>& getObstacle() noexcept { return _isObstacleFunction; }

   private:
    std::function<bool(MapElementType)> _isObstacleFunction;
};

}  // namespace AStar

#endif
