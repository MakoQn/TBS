#pragma once

#include "Unit.h"
#include <memory>

class Grid;

class UnitFactory
{
public:
    virtual ~UnitFactory() {}

    virtual std::unique_ptr<Unit> createUnit(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i position) = 0;
};

class WarriorFactory : public UnitFactory
{
public:
    std::unique_ptr<Unit> createUnit(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i position) override;
};

class ArcherFactory : public UnitFactory
{
public:
    std::unique_ptr<Unit> createUnit(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i position) override;
};

class TankFactory : public UnitFactory
{
public:
    std::unique_ptr<Unit> createUnit(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i position) override;
};