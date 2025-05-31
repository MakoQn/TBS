#pragma once

#include "Defines.h"
#include <vector>
#include <memory>

class Unit;
class Grid;
class ICommand;

class IAIStrategy
{
public:
    virtual ~IAIStrategy() {}

    virtual std::unique_ptr<ICommand> decideAction(Unit* unit, Grid* grid, const std::vector<Unit*>& playerUnits, const std::vector<Unit*>& aiUnits) = 0;
};

class AggressiveStrategy : public IAIStrategy
{
public:
    std::unique_ptr<ICommand> decideAction(Unit* unit, Grid* grid, const std::vector<Unit*>& playerUnits, const std::vector<Unit*>& aiUnits) override;
};

class DefensiveStrategy : public IAIStrategy
{
public:
    std::unique_ptr<ICommand> decideAction(Unit* unit, Grid* grid, const std::vector<Unit*>& playerUnits, const std::vector<Unit*>& aiUnits) override;
};