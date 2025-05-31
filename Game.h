#pragma once

#include "Defines.h"
#include "InputHandler.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

class AssetManager;
class InputHandler;
class GameStateManager;
class GameplayState;

class Game
{
private:
    sf::RenderWindow mainWindow;
    sf::Clock deltaClock;
    InputHandler mainInputHandler;

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void initialize();
    void cleanup();
public:
    Game();
    ~Game();

    void run();
    sf::RenderWindow& getWindow();
    AssetManager& getAssetManager();
    InputHandler& getInputHandler();
    void requestStateChange(GameStateId stateId, GameplayMode mode = GameplayMode::PVP, const std::string& message = "");
};