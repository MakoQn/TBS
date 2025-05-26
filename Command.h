#pragma once

#include "Defines.h"
#include <memory>

class Unit;
class Grid;

class ICommand
{
public:
    virtual ~ICommand() {}

    virtual bool execute() = 0;
};

class MoveCommand : public ICommand
{
private:
    Unit* unitToMove;
    sf::Vector2i targetPosition;
    Grid* gameGrid;
public:
    MoveCommand(Unit* unit, const sf::Vector2i& targetPosition, Grid* grid);

    bool execute() override;
};

class AttackCommand : public ICommand
{
private:
    Unit* attackingUnit;
    Unit* targetUnit;
public:
    AttackCommand(Unit* attacker, Unit* target);

    bool execute() override;
};