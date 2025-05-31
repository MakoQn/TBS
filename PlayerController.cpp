#include "PlayerController.h"
#include "InputHandler.h"
#include "Grid.h"
#include "Unit.h"
#include "Command.h"
#include "TurnManager.h"
#include "Pathfinding.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <cmath>

PlayerController::PlayerController(PlayerId id, Grid* gameGrid, InputHandler* input, TurnManager* turnMgr) : playerId(id), grid(gameGrid), inputHandler(input), turnManagerRef(turnMgr),
    selectedUnit(nullptr), currentActionState(ActionState::SELECTING_UNIT), units(nullptr) {}

void PlayerController::setUnits(const std::vector<Unit*>* playerUnits)
{
    units = playerUnits;
}

void PlayerController::handleUnitSelection(sf::RenderWindow& window)
{
    sf::Vector2i mousePixelPos = inputHandler->getMousePosition(window);
    sf::Vector2i gridCoords = grid->mapPixelToGridCoords(mousePixelPos, window.getView(), window);

    if (grid->isValidCoordinate(gridCoords))
    {
        Cell* clickedCell = grid->getCell(gridCoords);

        if (clickedCell && clickedCell->getOccupyingUnit() != nullptr)
        {
            Unit* unitInCell = clickedCell->getOccupyingUnit();

            if (unitInCell->getOwner() == playerId && unitInCell->isActive())
            {
                selectUnit(unitInCell);
                currentActionState = ActionState::UNIT_SELECTED;

                std::cout << "Игрок " << (int)playerId << " выбрал Юнита " << selectedUnit->getId() << " в (" << gridCoords.x << "," << gridCoords.y << ")" << std::endl;

                for (int i = 0; i < GRID_WIDTH; ++i)
                {
                    for (int j = 0; j < GRID_HEIGHT; ++j)
                    {
                        grid->getCell(i, j)->setIsHighlighted(false);
                    }
                }
                
                if (selectedUnit)
                {
                    for (int dx = -selectedUnit->getMovementRange(); dx <= selectedUnit->getMovementRange(); ++dx)
                    {
                        for (int dy = -selectedUnit->getMovementRange(); dy <= selectedUnit->getMovementRange(); ++dy)
                        {
                            if (std::abs(dx) + std::abs(dy) <= selectedUnit->getMovementRange() && std::abs(dx) + std::abs(dy) != 0)
                            {
                                sf::Vector2i potentialPos = selectedUnit->getPosition() + sf::Vector2i(dx, dy);

                                if (grid->isValidCoordinate(potentialPos) && grid->getCell(potentialPos)->isTraversable())
                                {
                                    grid->getCell(potentialPos)->setIsHighlighted(true);
                                }
                            }
                        }
                    }
                }
            }
            else if (selectedUnit != nullptr)
            {
                if (unitInCell->getOwner() != playerId && unitInCell->isAlive())
                {
                    tryAttackWithSelectedUnit(gridCoords);
                }
            }
        }
        else if (selectedUnit != nullptr && clickedCell)
        {
            tryMoveSelectedUnit(gridCoords);
        }
    }
}

bool PlayerController::tryMoveSelectedUnit(const sf::Vector2i& targetGridCoords)
{
    if (!selectedUnit || !selectedUnit->isActive() || selectedUnit->getActionPoints() == 0)
    {
        return false;
    }

    MoveCommand moveCmd(selectedUnit, targetGridCoords, grid);

    if (moveCmd.execute())
    {
        std::cout << "Игрок " << (int)playerId << " переместил Юнита " << selectedUnit->getId() << std::endl;

        if (selectedUnit->getActionPoints() == 0 || !selectedUnit->isActive())
        {
            deselectUnit();
        }

        return true;
    }

    return false;
}

bool PlayerController::tryAttackWithSelectedUnit(const sf::Vector2i& targetGridCoords)
{
    if (!selectedUnit || !selectedUnit->isActive() || selectedUnit->getActionPoints() == 0)
    {
        return false;
    }

    Cell* targetCell = grid->getCell(targetGridCoords);

    if (targetCell && targetCell->getOccupyingUnit() != nullptr && targetCell->getOccupyingUnit()->getOwner() != playerId)
    {
        Unit* targetUnit = targetCell->getOccupyingUnit();
        AttackCommand attackCmd(selectedUnit, targetUnit);

        if (attackCmd.execute())
        {
            std::cout << "Игрок " << (int)playerId << " атаковал Юнитом " << selectedUnit->getId() << std::endl;

            if (selectedUnit->getActionPoints() == 0 || !selectedUnit->isActive())
            {
                deselectUnit();
            }

            return true;
        }
    }

    return false;
}

void PlayerController::selectUnit(Unit* unit)
{
    selectedUnit = unit;

    if (selectedUnit)
    {
        currentActionState = ActionState::UNIT_SELECTED;
    }
}

Unit* PlayerController::getSelectedUnit() const
{
    return selectedUnit;
}

PlayerId PlayerController::getPlayerId() const
{
    return playerId;
}

void PlayerController::deselectUnit()
{
    if (selectedUnit)
    {
        std::cout << "Игрок " << (int)playerId << " отменил выбор Юнита " << selectedUnit->getId() << std::endl;
    }

    for (int i = 0; i < GRID_WIDTH; ++i)
    {
        for (int j = 0; j < GRID_HEIGHT; ++j)
        {
            grid->getCell(i, j)->setIsHighlighted(false);
        }
    }

    selectedUnit = nullptr;
    currentActionState = ActionState::SELECTING_UNIT;
}