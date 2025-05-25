#include "Grid.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <iostream>

Grid::Grid()
{
    initializeGrid();

    gridLineShape.setFillColor(sf::Color::Transparent);
    gridLineShape.setOutlineColor(sf::Color(100, 100, 100));
    gridLineShape.setOutlineThickness(1.0f);
}

void Grid::initializeGrid()
{
    cells.resize(GRID_WIDTH);

    for (int i = 0; i < GRID_WIDTH; ++i)
    {
        cells[i].resize(GRID_HEIGHT);

        for (int j = 0; j < GRID_HEIGHT; ++j)
        {
            cells[i][j] = Cell(i, j);
        }
    }
}

Cell* Grid::getCell(int x, int y)
{
    if (isValidCoordinate(x, y))
    {
        return &cells[x][y];
    }

    return nullptr;
}

Cell* Grid::getCell(const sf::Vector2i& coordinates)
{
    return getCell(coordinates.x, coordinates.y);
}

bool Grid::isValidCoordinate(int x, int y) const
{
    return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}

bool Grid::isValidCoordinate(const sf::Vector2i& coordinates) const
{
    return isValidCoordinate(coordinates.x, coordinates.y);
}

std::vector<Cell*> Grid::getNeighbours(Cell* cell, bool includeDiagonals)
{
    std::vector<Cell*> neighbours;

    if (!cell)
    {
        return neighbours;
    }

    sf::Vector2i pos = cell->getCoordinates();

    int directions[8][2] = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0},
        {-1, -1}, {1, -1}, {-1, 1}, {1, 1}
    };

    int numDirections = includeDiagonals ? 8 : 4;

    for (int i = 0; i < numDirections; ++i)
    {
        int newX = pos.x + directions[i][0];
        int newY = pos.y + directions[i][1];

        if (isValidCoordinate(newX, newY))
        {
            neighbours.push_back(getCell(newX, newY));
        }
    }

    return neighbours;
}

void Grid::render(sf::RenderWindow& window)
{
    for (int i = 0; i < GRID_WIDTH; ++i)
    {
        for (int j = 0; j < GRID_HEIGHT; ++j)
        {
            if (cells[i][j].getIsHighlighted())
            {
                sf::RectangleShape highlightShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));

                highlightShape.setPosition(sf::Vector2f(static_cast<float>(i) * CELL_SIZE, static_cast<float>(j) * CELL_SIZE));
                highlightShape.setFillColor(sf::Color(0, 255, 0, 100));
                window.draw(highlightShape);
            }
        }
    }

    gridLineShape.setSize(sf::Vector2f(CELL_SIZE - gridLineShape.getOutlineThickness() * 2.0f, CELL_SIZE - gridLineShape.getOutlineThickness() * 2.0f));

    for (int i = 0; i < GRID_WIDTH; ++i)
    {
        for (int j = 0; j < GRID_HEIGHT; ++j)
        {
            gridLineShape.setPosition(sf::Vector2f(static_cast<float>(i) * CELL_SIZE + gridLineShape.getOutlineThickness(), static_cast<float>(j) * CELL_SIZE + gridLineShape.getOutlineThickness()));
            window.draw(gridLineShape);
        }
    }
}

sf::Vector2i Grid::mapPixelToGridCoords(const sf::Vector2i& pixelCoords, const sf::View& currentView, sf::RenderWindow& window) const
{
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelCoords, currentView);

    int gridX = static_cast<int>(worldPos.x / CELL_SIZE);
    int gridY = static_cast<int>(worldPos.y / CELL_SIZE);

    if (gridX < 0)
    {
        gridX = 0;
    }

    if (gridX >= GRID_WIDTH)
    {
        gridX = GRID_WIDTH - 1;
    }

    if (gridY < 0)
    {
        gridY = 0;
    }

    if (gridY >= GRID_HEIGHT)
    {
        gridY = GRID_HEIGHT - 1;
    }

    return sf::Vector2i(gridX, gridY);
}