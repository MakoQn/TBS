#include "Cell.h"
#include "Unit.h"

Cell::Cell(int x, int y) : coordinates(x, y), traversableState(true), currentOccupyingUnit(nullptr), isHighlighted(false) {}

sf::Vector2i Cell::getCoordinates() const
{
    return coordinates;
}

bool Cell::isTraversable() const
{
    return traversableState && (currentOccupyingUnit == nullptr);
}

void Cell::setTraversable(bool traversable)
{
    traversableState = traversable;
}

Unit* Cell::getOccupyingUnit() const
{
    return currentOccupyingUnit;
}

void Cell::setOccupyingUnit(Unit* unit)
{
    currentOccupyingUnit = unit;
}

bool Cell::getIsHighlighted() const
{
    return isHighlighted;
}

void Cell::setIsHighlighted(bool value)
{
    isHighlighted = value;
}