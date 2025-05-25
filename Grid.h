#pragma once

#include "Cell.h"
#include "Defines.h"
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Grid
{
private:
    std::vector<std::vector<Cell>> cells;
    sf::RectangleShape gridLineShape;

    void initializeGrid();
public:
    Grid();

    Cell* getCell(int x, int y);
    Cell* getCell(const sf::Vector2i& coordinates);
    bool isValidCoordinate(int x, int y) const;
    bool isValidCoordinate(const sf::Vector2i& coordinates) const;
    std::vector<Cell*> getNeighbours(Cell* cell, bool includeDiagonals = false);
    void render(sf::RenderWindow& window);
    sf::Vector2i mapPixelToGridCoords(const sf::Vector2i& pixelCoords, const sf::View& view, sf::RenderWindow& window) const;
};