#include "GameplayState.h"
#include "Game.h"
#include "AssetManager.h"
#include "GameStateManager.h"
#include "GameOverState.h"
#include "Unit.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <sstream>
#include <algorithm>

GameplayState::GameplayState(GameplayMode mode)
    : GameState(GameStateId::GAMEPLAY),
    currentMode(mode),
    uiFont(),
    currentPlayerText(this->uiFont, ""),
    currentTurnText(this->uiFont, ""),
    selectedUnitInfoText(this->uiFont, ""),
    gameOverMessageText(this->uiFont, ""),
    nextUnitId(0),
    isGameOver(false)
{
    if (!this->uiFont.openFromFile("assets/arial.ttf"))
    {
        std::cerr << "ОШИБКА: Не удалось загрузить шрифт 'assets/arial.ttf' в GameplayState!" << std::endl;
    }
}

void GameplayState::init(Game* game)
{
    gamePtr = game;

    playerOneController = std::make_unique<PlayerController>(PlayerId::PLAYER_ONE, &gameGrid, &gamePtr->getInputHandler(), &turnManager);

    if (currentMode == GameplayMode::PVP)
    {
        playerTwoController = std::make_unique<PlayerController>(PlayerId::PLAYER_TWO, &gameGrid, &gamePtr->getInputHandler(), &turnManager);
    }
    else
    {
        aiController = std::make_unique<AiController>(PlayerId::PLAYER_TWO, &gameGrid, &turnManager);
        aiController->setStrategy(AiStrategyType::AGGRESSIVE);
    }

    setupUnits();
    setupUI();

    turnManager.attach(this);

    for (auto& unitPtr : allUnits)
    {
        if (unitPtr) unitPtr->attach(this);
    }

    turnManager.startGame(PlayerId::PLAYER_ONE);
    updateUI();
}

void GameplayState::setupUnits()
{
    addUnit(UnitType::WARRIOR, PlayerId::PLAYER_ONE, sf::Vector2i(1, GRID_HEIGHT - 2));
    addUnit(UnitType::ARCHER, PlayerId::PLAYER_ONE, sf::Vector2i(2, GRID_HEIGHT - 2));
    addUnit(UnitType::TANK, PlayerId::PLAYER_ONE, sf::Vector2i(3, GRID_HEIGHT - 2));

    PlayerId p2Id = PlayerId::PLAYER_TWO;

    addUnit(UnitType::WARRIOR, p2Id, sf::Vector2i(GRID_WIDTH - 2, 1));
    addUnit(UnitType::ARCHER, p2Id, sf::Vector2i(GRID_WIDTH - 3, 1));
    addUnit(UnitType::TANK, p2Id, sf::Vector2i(GRID_WIDTH - 4, 1));

    playerOneController->setUnits(&playerOneUnits);

    if (currentMode == GameplayMode::PVP && playerTwoController)
    {
        playerTwoController->setUnits(&playerTwoUnits);
    }
    else if (currentMode == GameplayMode::PVE && aiController)
    {
        aiController->setUnits(&playerTwoUnits);
    }
}

void GameplayState::addUnit(UnitType type, PlayerId owner, sf::Vector2i position)
{
    std::unique_ptr<Unit> newUnit;

    switch (type)
    {
    case UnitType::WARRIOR:
        newUnit = warriorFactory.createUnit(owner, nextUnitId++, &gameGrid, position); 
        break;
    case UnitType::ARCHER:
        newUnit = archerFactory.createUnit(owner, nextUnitId++, &gameGrid, position);
        break;
    case UnitType::TANK:
        newUnit = tankFactory.createUnit(owner, nextUnitId++, &gameGrid, position);
        break;
    default:
        return;
    }

    if (newUnit)
    {
        newUnit->attach(this);

        if (owner == PlayerId::PLAYER_ONE)
        {
            playerOneUnits.push_back(newUnit.get());
        }
        else
        {
            playerTwoUnits.push_back(newUnit.get());
        }

        allUnits.push_back(std::move(newUnit));
    }
}

void GameplayState::setupUI()
{
    currentPlayerText.setCharacterSize(20);
    currentPlayerText.setFillColor(sf::Color::White);
    currentPlayerText.setPosition(sf::Vector2f(10.f, WINDOW_HEIGHT - 90.f));

    currentTurnText.setCharacterSize(20);
    currentTurnText.setFillColor(sf::Color::White);
    currentTurnText.setPosition(sf::Vector2f(10.f, WINDOW_HEIGHT - 65.f));

    selectedUnitInfoText.setCharacterSize(18);
    selectedUnitInfoText.setFillColor(sf::Color::White);
    selectedUnitInfoText.setPosition(sf::Vector2f(10.f, WINDOW_HEIGHT - 40.f));

    gameOverMessageText.setCharacterSize(40);
    gameOverMessageText.setFillColor(sf::Color::Red);
}

void GameplayState::updateUI()
{
    std::ostringstream pStream;

    pStream << "Current player: " << (turnManager.getCurrentPlayer() == PlayerId::PLAYER_ONE ? "Player 1" : (currentMode == GameplayMode::PVP ? "Player 2" : "AI"));
    
    currentPlayerText.setString(pStream.str());

    std::ostringstream tStream;
    
    tStream << "Turn: " << turnManager.getCurrentTurnNumber();
    
    currentTurnText.setString(tStream.str());

    Unit* selected = nullptr;
    
    if (turnManager.getCurrentPlayer() == PlayerId::PLAYER_ONE && playerOneController)
    {
        selected = playerOneController->getSelectedUnit();
    }
    else if (turnManager.getCurrentPlayer() == PlayerId::PLAYER_TWO && currentMode == GameplayMode::PVP && playerTwoController)
    {
        selected = playerTwoController->getSelectedUnit();
    }

    if (selected)
    {
        std::ostringstream sStream;
        
        sStream << "Chosen unit: ID " << selected->getId() << " | Type: ";
        
        switch (selected->getType())
        {
        case UnitType::WARRIOR:
            sStream << "Warrior";
            break;
        case UnitType::ARCHER:
            sStream << "Archer";
            break;
        case UnitType::TANK:
            sStream << "Tank";
            break;
        }

        sStream << " | HP: " << selected->getHealth() << "/" << selected->getMaxHealth() << " | AP: " << selected->getActionPoints();
        
        selectedUnitInfoText.setString(sStream.str());
    }
    else
    {
        selectedUnitInfoText.setString("The unit was not selected");
    }
}

void GameplayState::handleInput(const sf::Event& event, sf::RenderWindow& window)
{
    if (isGameOver)
    {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            gamePtr->requestStateChange(GameStateId::MAIN_MENU);
        }

        return;
    }

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        if (turnManager.getCurrentPlayer() == PlayerId::PLAYER_ONE && playerOneController)
        {
            playerOneController->handleUnitSelection(window);
        }
        else if (turnManager.getCurrentPlayer() == PlayerId::PLAYER_TWO && currentMode == GameplayMode::PVP && playerTwoController)
        {
            playerTwoController->handleUnitSelection(window);
        }
    }
    else if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
        {
            if (turnManager.getCurrentPlayer() == PlayerId::PLAYER_ONE && playerOneController)
            {
                if (playerOneController->getSelectedUnit())
                {
                    playerOneController->deselectUnit();
                }

                turnManager.nextTurn();
            }
            else if (turnManager.getCurrentPlayer() == PlayerId::PLAYER_TWO && currentMode == GameplayMode::PVP && playerTwoController)
            {
                if (playerTwoController->getSelectedUnit())
                {
                    playerTwoController->deselectUnit();
                }

                turnManager.nextTurn();
            }
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) // Пример: Отменить выбор юнита
        {
            if (turnManager.getCurrentPlayer() == PlayerId::PLAYER_ONE && playerOneController)
            {
                playerOneController->deselectUnit();
            }
            else if (turnManager.getCurrentPlayer() == PlayerId::PLAYER_TWO && currentMode == GameplayMode::PVP && playerTwoController)
            {
                playerTwoController->deselectUnit();
            }
        }
    }
}

void GameplayState::update(sf::Time deltaTime)
{
    if (isGameOver) return;

    for (auto& unit : allUnits)
    {
        if (unit) unit->update(deltaTime);
    }

    if (turnManager.getCurrentPlayer() == PlayerId::PLAYER_TWO && currentMode == GameplayMode::PVE && aiController)
    {
        std::vector<Unit*> p1UnitsForAI;

        for (auto u : playerOneUnits)
        {
            if (u && u->isAlive())
            {
                p1UnitsForAI.push_back(u);
            }
        }

        std::vector<Unit*> aiUnitsForAI;

        for (auto u : playerTwoUnits)
        {
            if (u && u->isAlive())
            {
                aiUnitsForAI.push_back(u);
            }
        }

        aiController->handleTurn(p1UnitsForAI, aiUnitsForAI);
    }

    removeDeadUnits();
    checkWinConditions();
    updateUI();
}

void GameplayState::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(50, 50, 50));

    sf::Texture& bgTexture = AssetManager::getInstance().getTexture("assets/background.png");
    sf::Sprite backgroundSprite(bgTexture);

    window.draw(backgroundSprite);
    gameGrid.render(window);

    for (auto& unit : allUnits)
    {
        if (unit && unit->isAlive())
        {
            unit->render(window);
        }
    }

    window.draw(currentPlayerText);
    window.draw(currentTurnText);
    window.draw(selectedUnitInfoText);

    if (isGameOver)
    {
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        window.draw(gameOverMessageText);
    }
}

void GameplayState::onNotify(const GameEvent& event)
{
    switch (event.type)
    {
    case EventType::TURN_CHANGED:
        std::cout << "GameplayState уведомлен: Смена хода на игрока " << (int)event.playerId << std::endl;

        for (auto& unit : allUnits)
        {
            if (unit && unit->getOwner() == event.playerId)
            {
                unit->onTurnStart();
            }
            else if (unit)
            {
                unit->onTurnEnd();
            }
        }

        if (playerOneController)
        {
            playerOneController->deselectUnit();
        }

        if (playerTwoController)
        {
            playerTwoController->deselectUnit();
        }

        updateUI();
        break;
    case EventType::UNIT_DIED:
        std::cout << "GameplayState уведомлен: Юнит " << event.unitId << " погиб." << std::endl;

        updateUI();
        break;
    case EventType::UNIT_HEALTH_CHANGED:
    case EventType::UNIT_MOVED:
    case EventType::UNIT_ACTION_TAKEN:
        updateUI();
        break;
    case EventType::GAME_OVER_EVENT:
        isGameOver = true;
        updateUI();
        break;
    default:
        break;
    }
}

Grid* GameplayState::getGrid()
{
    return &gameGrid;
}

void GameplayState::removeDeadUnits()
{
    auto removeLambda = [&](std::vector<Unit*>& unitList) {
        unitList.erase(std::remove_if(unitList.begin(), unitList.end(),
            [](Unit* u) {
            return u == nullptr || !u->isAlive();
        }), unitList.end());
        };

    removeLambda(playerOneUnits);
    removeLambda(playerTwoUnits);

    allUnits.erase(std::remove_if(allUnits.begin(), allUnits.end(),
        [](const std::unique_ptr<Unit>& u) {
        return u == nullptr || !u->isAlive();
    }), allUnits.end());
}

void GameplayState::checkWinConditions()
{
    if (isGameOver)
    {
        return;
    }

    bool playerOneHasUnits = false;

    for (Unit* u : playerOneUnits)
    {
        if (u && u->isAlive())
        {
            playerOneHasUnits = true;
            break;
        }
    }

    bool playerTwoHasUnits = false;

    for (Unit* u : playerTwoUnits)
    {
        if (u && u->isAlive())
        {
            playerTwoHasUnits = true;
            break;
        }
    }

    std::string winMessage;

    if (!playerOneHasUnits && playerTwoHasUnits)
    {
        winMessage = (currentMode == GameplayMode::PVP ? "Player 2 win!" : "AI Win!");
        isGameOver = true;
    }
    else if (!playerTwoHasUnits && playerOneHasUnits)
    {
        winMessage = "Player 1 win!";
        isGameOver = true;
    }
    else if (!playerOneHasUnits && !playerTwoHasUnits && !allUnits.empty())
    {
        bool gameHadUnits = false;

        for (const auto& u_ptr : allUnits)
        {
            if (u_ptr)
            {
                gameHadUnits = true;
            }
        }

        if (turnManager.getCurrentTurnNumber() > 0 || !allUnits.empty())
        {
            winMessage = "It's a draw! All units are destroyed.";
            isGameOver = true;
        }
    }

    if (isGameOver)
    {
        std::cout << "Game over: " << winMessage << std::endl;
        gameOverMessageText.setString(winMessage + "\nPress Enter to exit the menu.");

        sf::FloatRect bounds = gameOverMessageText.getLocalBounds();

        gameOverMessageText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f));
        gameOverMessageText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));

        GameEvent ge(EventType::GAME_OVER_EVENT);
        
        turnManager.notify(ge);
    }
}