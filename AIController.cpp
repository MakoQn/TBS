#include "AIController.h"
#include "TurnManager.h"
#include "Unit.h"
#include "Grid.h"
#include "Command.h"
#include "Pathfinding.h"
#include <iostream>
#include <algorithm>
#include <limits>

AiController::AiController(PlayerId id, Grid* gameGrid, TurnManager* turnMgr) : aiPlayerId(id), grid(gameGrid), turnManagerRef(turnMgr), units(nullptr)
{
    setStrategy(AiStrategyType::AGGRESSIVE);
}

void AiController::setUnits(const std::vector<Unit*>* controllerUnits)
{
    units = controllerUnits;
}

void AiController::setStrategy(AiStrategyType strategyType)
{
    switch (strategyType)
    {
    case AiStrategyType::AGGRESSIVE:
        currentStrategy = std::make_unique<AggressiveStrategy>();
        break;
    case AiStrategyType::DEFENSIVE:
        currentStrategy = std::make_unique<DefensiveStrategy>();
        break;
    default:
        currentStrategy = std::make_unique<AggressiveStrategy>();
        break;
    }

    std::cout << "Контроллер ИИ для Игрока " << (int)aiPlayerId << " установил стратегию: "
        << (strategyType == AiStrategyType::AGGRESSIVE ? "Агрессивная" : "Оборонительная") << std::endl;
}

void AiController::handleTurn(const std::vector<Unit*>& playerUnits, const std::vector<Unit*>& aiControlledUnits)
{
    std::cout << "Ход ИИ Игрока " << (int)aiPlayerId << "." << std::endl;

    if (!units || units->empty() || !currentStrategy)
    {
        std::cout << "У ИИ нет юнитов или стратегии, завершение хода." << std::endl;

        turnManagerRef->nextTurn();

        return;
    }

    for (Unit* aiUnit : *units)
    {
        if (aiUnit && aiUnit->getOwner() == aiPlayerId && aiUnit->isAlive() && aiUnit->isActive())
        {
            while (aiUnit->getActionPoints() > 0 && aiUnit->isActive())
            {
                std::cout << "ИИ рассматривает действие для юнита " << aiUnit->getId() << " (ОД: " << aiUnit->getActionPoints() << ")" << std::endl;
                
                std::unique_ptr<ICommand> command = currentStrategy->decideAction(aiUnit, grid, playerUnits, *units);

                if (command)
                {
                    if (command->execute())
                    {
                        std::cout << "Юнит ИИ " << aiUnit->getId() << " выполнил команду." << std::endl;
                    }
                    else
                    {
                        std::cout << "Команда Юнита ИИ " << aiUnit->getId() << " не удалась или нет действия." << std::endl;
                        break;
                    }
                }
                else
                {
                    std::cout << "Юнит ИИ " << aiUnit->getId() << " решил не действовать." << std::endl;
                    break;
                }
            }

            aiUnit->setActive(false);
        }
    }

    std::cout << "ИИ Игрок " << (int)aiPlayerId << " завершил действия, конец хода." << std::endl;

    turnManagerRef->nextTurn();
}

PlayerId AiController::getPlayerId() const
{
    return aiPlayerId;
}

Unit* AiController::findWeakestEnemy(const std::vector<Unit*>& enemyUnits) const
{
    Unit* weakest = nullptr;
    int minHealth = std::numeric_limits<int>::max();

    for (Unit* enemy : enemyUnits)
    {
        if (enemy && enemy->isAlive() && enemy->getHealth() < minHealth)
        {
            minHealth = enemy->getHealth();
            weakest = enemy;
        }
    }

    return weakest;
}

Unit* AiController::findNearestEnemy(Unit* aiUnit, const std::vector<Unit*>& enemyUnits) const
{
    if (!aiUnit)
    {
        return nullptr;
    }

    Unit* nearest = nullptr;
    int minDistance = std::numeric_limits<int>::max();
    sf::Vector2i aiPos = aiUnit->getPosition();

    for (Unit* enemy : enemyUnits)
    {
        if (enemy && enemy->isAlive())
        {
            sf::Vector2i enemyPos = enemy->getPosition();
            int distance = std::abs(aiPos.x - enemyPos.x) + std::abs(aiPos.y - enemyPos.y);

            if (distance < minDistance)
            {
                minDistance = distance;
                nearest = enemy;
            }
        }
    }

    return nearest;
}