#pragma once

#include "Defines.h"
#include "Command.h"
#include <vector>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

class Unit;
class Grid;
class InputHandler;
class TurnManager;

class PlayerController
{
private:
    PlayerId playerId;
    Grid* grid;
    InputHandler* inputHandler;
    TurnManager* turnManagerRef;
    Unit* selectedUnit;
    const std::vector<Unit*>* units;

    enum class ActionState
    {
        SELECTING_UNIT,
        UNIT_SELECTED,
        PERFORMING_ACTION
    };

    ActionState currentActionState;

    bool tryAttackWithSelectedUnit(const sf::Vector2i& targetGridCoords);
public:
    PlayerController(PlayerId id, Grid* gameGrid, InputHandler* input, TurnManager* turnManager);

    void selectUnit(Unit* unit);
    Unit* getSelectedUnit() const;
    PlayerId getPlayerId() const;
    void setUnits(const std::vector<Unit*>* playerUnits);
    void handleUnitSelection(sf::RenderWindow& window);
    bool tryMoveSelectedUnit(const sf::Vector2i& targetGridCoords);
    void deselectUnit();
};