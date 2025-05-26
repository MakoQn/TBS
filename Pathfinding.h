#pragma once

#include "Defines.h"
#include <vector>
#include <SFML/System/Vector2.hpp>

class Grid;

class Pathfinding
{
private:
    static int calculateHeuristic(const sf::Vector2i& a, const sf::Vector2i& b);
    static std::vector<sf::Vector2i> reconstructPath(PathNode* endNode);
public:
    static std::vector<sf::Vector2i> findPath(Grid* grid, const sf::Vector2i& start, const sf::Vector2i& end);
};