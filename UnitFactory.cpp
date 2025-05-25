#include "UnitFactory.h"

std::unique_ptr<Unit> WarriorFactory::createUnit(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i position)
{
    return std::make_unique<Warrior>(owner, id, gameGrid, position);
}

std::unique_ptr<Unit> ArcherFactory::createUnit(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i position)
{
    return std::make_unique<Archer>(owner, id, gameGrid, position);
}

std::unique_ptr<Unit> TankFactory::createUnit(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i position)
{
    return std::make_unique<Tank>(owner, id, gameGrid, position);
}