#pragma once

#include "Defines.h"
#include <vector>
#include <memory>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class GameState;
class Game;

class GameStateManager
{
private:
    GameStateManager();
    ~GameStateManager();

    std::vector<std::unique_ptr<GameState>> states;
public:
    GameStateManager(const GameStateManager&) = delete;
    GameStateManager& operator=(const GameStateManager&) = delete;

    static GameStateManager& getInstance();

    void pushState(std::unique_ptr<GameState> state, Game* game);
    void popState();
    void changeState(std::unique_ptr<GameState> state, Game* game);
    GameState* getCurrentState();
    void handleInput(const sf::Event& event, sf::RenderWindow& window);
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);
    bool isEmpty() const;
};