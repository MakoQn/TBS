#include "Pathfinding.h"
#include "Grid.h"
#include "Unit.h"
#include <queue>
#include <vector>
#include <algorithm>
#include <cmath>

int Pathfinding::calculateHeuristic(const sf::Vector2i& a, const sf::Vector2i& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<sf::Vector2i> Pathfinding::reconstructPath(PathNode* endNode)
{
    std::vector<sf::Vector2i> path;
    PathNode* current = endNode;

    while (current != nullptr)
    {
        path.push_back(current->position);
        current = current->parent;
    }

    std::reverse(path.begin(), path.end());

    if (!path.empty())
    {
        path.erase(path.begin());
    }

    return path;
}

std::vector<sf::Vector2i> Pathfinding::findPath(Grid* grid, const sf::Vector2i& start, const sf::Vector2i& end)
{
    if (!grid || !grid->isValidCoordinate(start) || !grid->isValidCoordinate(end))
    {
        return {};
    }
    if (start == end)
    {
        return {};
    }

    std::priority_queue<PathNode*, std::vector<PathNode*>, ComparePathNode> openSet;
    std::vector<PathNode*> allNodes;
    PathNode* startNode = new PathNode(start);

    startNode->gCost = 0;
    startNode->hCost = calculateHeuristic(start, end);
    allNodes.push_back(startNode);
    openSet.push(startNode);

    std::vector<sf::Vector2i> closedSetPositions;

    while (!openSet.empty())
    {
        PathNode* currentNode = openSet.top();
        openSet.pop();

        if (currentNode->position == end)
        {
            std::vector<sf::Vector2i> path = reconstructPath(currentNode);

            for (PathNode* node : allNodes)
            {
                delete node;
            }

            return path;
        }

        bool alreadyClosed = false;

        for (const auto& closedPos : closedSetPositions)
        {
            if (closedPos == currentNode->position)
            {
                alreadyClosed = true;
                break;
            }
        }

        if (alreadyClosed)
        {
            continue;
        }

        closedSetPositions.push_back(currentNode->position);

        Cell* currentCell = grid->getCell(currentNode->position);

        if (!currentCell)
        {
            continue;
        }

        for (Cell* neighbourCell : grid->getNeighbours(currentCell, false))
        {
            if (!neighbourCell)
            {
                continue;
            }

            bool isTraversableForPath = neighbourCell->isTraversable() || (neighbourCell->getCoordinates() == end && neighbourCell->getOccupyingUnit() == nullptr);
            
            if (!isTraversableForPath && neighbourCell->getCoordinates() != end)
            {
                if (neighbourCell->getCoordinates() != end || (neighbourCell->getOccupyingUnit() != nullptr && neighbourCell->getOccupyingUnit()->getPosition() != end))
                {
                    continue;
                }
            }

            bool inClosed = false;

            for (const auto& closedPos : closedSetPositions)
            {
                if (closedPos == neighbourCell->getCoordinates())
                {
                    inClosed = true;
                    break;
                }
            }

            if (inClosed)
            {
                continue;
            }

            int tentativeGCost = currentNode->gCost + 1;
            PathNode* neighbourNode = new PathNode(neighbourCell->getCoordinates(), currentNode);

            neighbourNode->gCost = tentativeGCost;
            neighbourNode->hCost = calculateHeuristic(neighbourCell->getCoordinates(), end);
            allNodes.push_back(neighbourNode);
            openSet.push(neighbourNode);
        }
    }

    for (PathNode* node : allNodes)
    {
        delete node;
    }

    return {};
}