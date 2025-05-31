#pragma once
#include "GameState.h"
#include "Grid.h"
#include "TurnManager.h"
#include "PlayerController.h"
#include "AIController.h"
#include "UnitFactory.h"
#include "InputHandler.h"
#include "Observer.h"
#include "Defines.h"
#include <vector>
#include <memory>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class Unit;

class GameplayState : public GameState, public IObserver
{
private:
    GameplayMode currentMode;
    Grid gameGrid;
    TurnManager turnManager;
    InputHandler inputHandler;

    std::unique_ptr<PlayerController> playerOneController;
    std::unique_ptr<PlayerController> playerTwoController;
    std::unique_ptr<AiController> aiController;

    std::vector<std::unique_ptr<Unit>> allUnits;
    std::vector<Unit*> playerOneUnits;
    std::vector<Unit*> playerTwoUnits;

    WarriorFactory warriorFactory;
    ArcherFactory archerFactory;
    TankFactory tankFactory;

    int nextUnitId;

    sf::Font uiFont;
    sf::Text currentPlayerText;
    sf::Text currentTurnText;
    sf::Text selectedUnitInfoText;
    sf::Text gameOverMessageText;
    bool isGameOver;

    void setupUnits();
    void setupUI();
    void updateUI();
    void addUnit(UnitType type, PlayerId owner, sf::Vector2i position);
    void removeDeadUnits();
    void checkWinConditions();
public:
    GameplayState(GameplayMode mode);

    void init(Game* game) override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void onNotify(const GameEvent& event) override;
    Grid* getGrid();
};