#pragma once

#include "Defines.h"
#include "AIStrategy.h"
#include <vector>
#include <memory>

class Unit;
class Grid;
class ICommand;
class TurnManager;

class AiController
{
private:
    PlayerId aiPlayerId;
    Grid* grid;
    TurnManager* turnManagerRef;
    const std::vector<Unit*>* units;
    std::unique_ptr<IAIStrategy> currentStrategy;

    Unit* findWeakestEnemy(const std::vector<Unit*>& enemyUnits) const;
    Unit* findNearestEnemy(Unit* aiUnit, const std::vector<Unit*>& enemyUnits) const;
public:
    AiController(PlayerId id, Grid* gameGrid, TurnManager* turnManager);

    void handleTurn(const std::vector<Unit*>& playerUnits, const std::vector<Unit*>& aiUnits);
    PlayerId getPlayerId() const;
    void setUnits(const std::vector<Unit*>* controllerUnits);
    void setStrategy(AiStrategyType strategyType);
};