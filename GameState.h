#pragma once

#include "Defines.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

class Game;

class GameState
{
protected:
    Game* gamePtr;
    GameStateId stateId;

    GameState(GameStateId id) : gamePtr(nullptr), stateId(id) {}
public:
    virtual ~GameState() {}

    virtual void init(Game* game) = 0;
    virtual void handleInput(const sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void update(sf::Time deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void pause() {};
    virtual void resume() {};
    
    GameStateId getId() const 
    { 
        return stateId;
    }
};