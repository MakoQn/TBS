#include "AIStrategy.h"
#include "Unit.h"
#include "Grid.h"
#include "Command.h"
#include "Pathfinding.h"
#include <iostream>
#include <limits>
#include <cmath>

std::unique_ptr<ICommand> AggressiveStrategy::decideAction(Unit* unit, Grid* grid, const std::vector<Unit*>& playerUnits, const std::vector<Unit*>& aiUnits)
{
    if (!unit || !unit->isActive() || unit->getActionPoints() == 0 || playerUnits.empty())
    {
        return nullptr;
    }

    Unit* bestTarget = nullptr;
    int minDistanceToTarget = std::numeric_limits<int>::max();

    for (Unit* playerUnit : playerUnits)
    {
        if (playerUnit && playerUnit->isAlive())
        {
            int dist = std::abs(unit->getPosition().x - playerUnit->getPosition().x) + std::abs(unit->getPosition().y - playerUnit->getPosition().y);
            
            if (dist < minDistanceToTarget)
            {
                minDistanceToTarget = dist;
                bestTarget = playerUnit;
            }
        }
    }

    if (!bestTarget)
    {
        return nullptr;
    }

    int distanceToBestTarget = std::abs(unit->getPosition().x - bestTarget->getPosition().x) + std::abs(unit->getPosition().y - bestTarget->getPosition().y);
    int attackRange = 1;

    if (unit->getType() == UnitType::ARCHER)
    {
        attackRange = 5;
    }

    if (distanceToBestTarget <= attackRange)
    {
        std::cout << "ИИ (Агрессивный): Юнит " << unit->getId() << " атакует цель " << bestTarget->getId() << std::endl;

        return std::make_unique<AttackCommand>(unit, bestTarget);
    }

    std::vector<sf::Vector2i> path = Pathfinding::findPath(grid, unit->getPosition(), bestTarget->getPosition());

    if (!path.empty())
    {
        sf::Vector2i moveTargetPos = unit->getPosition();
        int stepsTaken = 0;

        for (const auto& step : path)
        {
            if (stepsTaken < unit->getMovementRange())
            {
                Cell* stepCell = grid->getCell(step);

                if (stepCell && (stepCell->isTraversable() || (stepCell->getOccupyingUnit() == nullptr && step == bestTarget->getPosition())))
                {
                    moveTargetPos = step;
                    stepsTaken++;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        if (moveTargetPos != unit->getPosition())
        {
            int distAfterMove = std::abs(moveTargetPos.x - bestTarget->getPosition().x) + std::abs(moveTargetPos.y - bestTarget->getPosition().y);
            
            if (distAfterMove <= attackRange && unit->getActionPoints() >= 2)
            {
                std::cout << "ИИ (Агрессивный): Юнит " << unit->getId() << " движется к цели " << bestTarget->getId() << " в (" << moveTargetPos.x << "," << moveTargetPos.y << ")" << std::endl;

                return std::make_unique<MoveCommand>(unit, moveTargetPos, grid);
            }
            else if (distAfterMove > attackRange)
            {
                std::cout << "ИИ (Агрессивный): Юнит " << unit->getId() << " движется к цели " << bestTarget->getId() << " в (" << moveTargetPos.x << "," << moveTargetPos.y << ")" << std::endl;
                
                return std::make_unique<MoveCommand>(unit, moveTargetPos, grid);
            }
        }
    }

    std::cout << "ИИ (Агрессивный): Юнит " << unit->getId() << " не имеет четкого агрессивного действия." << std::endl;

    return nullptr;
}

std::unique_ptr<ICommand> DefensiveStrategy::decideAction(Unit* unit, Grid* grid, const std::vector<Unit*>& playerUnits, const std::vector<Unit*>& aiUnits)
{
    if (!unit || !unit->isActive() || unit->getActionPoints() == 0 || playerUnits.empty())
    {
        return nullptr;
    }

    Unit* targetInRange = nullptr;
    int attackRange = (unit->getType() == UnitType::ARCHER) ? 5 : 1;

    for (Unit* playerUnit : playerUnits)
    {
        if (playerUnit && playerUnit->isAlive())
        {
            int dist = std::abs(unit->getPosition().x - playerUnit->getPosition().x) + std::abs(unit->getPosition().y - playerUnit->getPosition().y);
            
            if (dist <= attackRange)
            {
                targetInRange = playerUnit;
                break;
            }
        }
    }

    if (targetInRange)
    {
        std::cout << "ИИ (Оборонительный): Юнит " << unit->getId() << " атакует цель " << targetInRange->getId() << std::endl;
        
        return std::make_unique<AttackCommand>(unit, targetInRange);
    }

    std::cout << "ИИ (Оборонительный): Юнит " << unit->getId() << " удерживает позицию." << std::endl;
    
    return nullptr;
}