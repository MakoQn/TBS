#pragma once

#include "Defines.h"
#include <SFML/System/Vector2.hpp>

class Unit;

class Cell
{
private:
    sf::Vector2i coordinates;
    bool traversableState;
    Unit* currentOccupyingUnit;
    bool isHighlighted;
public:
    Cell(int x = 0, int y = 0);

    sf::Vector2i getCoordinates() const;
    bool isTraversable() const;
    void setTraversable(bool traversable);
    Unit* getOccupyingUnit() const;
    void setOccupyingUnit(Unit* unit);
    bool getIsHighlighted() const;
    void setIsHighlighted(bool value);
};