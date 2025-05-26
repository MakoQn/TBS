#include "Command.h"
#include "Unit.h"
#include "Grid.h"
#include <cmath>
#include <iostream>

MoveCommand::MoveCommand(Unit* unit, const sf::Vector2i& target, Grid* grid) : unitToMove(unit), targetPosition(target), gameGrid(grid) {}

bool MoveCommand::execute()
{
    if (!unitToMove || !gameGrid || !unitToMove->isActive() || unitToMove->getActionPoints() == 0)
    {
        std::cerr << "MoveCommand: Невозможно выполнить - юнит недействителен, неактивен или нет ОД." << std::endl;

        return false;
    }

    sf::Vector2i currentPos = unitToMove->getPosition();
    int distance = std::abs(currentPos.x - targetPosition.x) + std::abs(currentPos.y - targetPosition.y);

    if (distance == 0)
    {
        std::cout << "MoveCommand: Юнит уже в цели." << std::endl;

        return false;
    }

    if (distance > unitToMove->getMovementRange())
    {
        std::cerr << "MoveCommand: Цель вне диапазона движения." << std::endl;

        return false;
    }

    Cell* targetCell = gameGrid->getCell(targetPosition);

    if (!targetCell || !targetCell->isTraversable() || targetCell->getOccupyingUnit() != nullptr)
    {
        std::cerr << "MoveCommand: Целевая клетка непроходима или занята." << std::endl;

        return false;
    }

    std::cout << "MoveCommand: Выполнение перемещения для юнита " << unitToMove->getId() << " в (" << targetPosition.x << "," << targetPosition.y << ")" << std::endl;
    
    return unitToMove->moveTo(targetPosition);
}

AttackCommand::AttackCommand(Unit* attacker, Unit* target) : attackingUnit(attacker), targetUnit(target) {}

bool AttackCommand::execute()
{
    if (!attackingUnit || !targetUnit || !attackingUnit->isActive() || attackingUnit->getActionPoints() == 0)
    {
        std::cerr << "AttackCommand: Невозможно выполнить - юниты недействительны, атакующий неактивен или нет ОД." << std::endl;

        return false;
    }
    if (!targetUnit->isAlive())
    {
        std::cerr << "AttackCommand: Целевой юнит уже мертв." << std::endl;

        return false;
    }

    sf::Vector2i attackerPos = attackingUnit->getPosition();
    sf::Vector2i targetPos = targetUnit->getPosition();
    int distance = std::abs(attackerPos.x - targetPos.x) + std::abs(attackerPos.y - targetPos.y);

    int attackRange = 1;

    if (attackingUnit->getType() == UnitType::ARCHER)
    {
        attackRange = 5;
    }

    if (distance > attackRange)
    {
        std::cerr << "AttackCommand: Цель вне диапазона атаки." << std::endl;

        return false;
    }

    std::cout << "AttackCommand: Выполнение атаки юнитом " << attackingUnit->getId() << " по юниту " << targetUnit->getId() << std::endl;
    
    return attackingUnit->attack(*targetUnit);
}