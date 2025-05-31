#include "GameStateManager.h"
#include "GameState.h"
#include <iostream>

GameStateManager& GameStateManager::getInstance()
{
    static GameStateManager instance;

    return instance;
}

GameStateManager::GameStateManager()
{
    std::cout << "GameStateManager создан." << std::endl;
}

GameStateManager::~GameStateManager()
{
    std::cout << "GameStateManager уничтожен. Очистка состояний..." << std::endl;

    while (!states.empty())
    {
        popState();
    }
}

void GameStateManager::pushState(std::unique_ptr<GameState> state, Game* game)
{
    if (!states.empty())
    {
        states.back()->pause();
    }

    state->init(game);
    states.push_back(std::move(state));

    std::cout << "Состояние добавлено. Текущее количество состояний: " << states.size() << std::endl;
}

void GameStateManager::popState()
{
    if (!states.empty())
    {
        states.pop_back();

        std::cout << "Состояние удалено." << std::endl;

        if (!states.empty())
        {
            states.back()->resume();

            std::cout << "Предыдущее состояние возобновлено." << std::endl;
        }
    }
}

void GameStateManager::changeState(std::unique_ptr<GameState> state, Game* game)
{
    if (!states.empty())
    {
        popState();
    }

    pushState(std::move(state), game);
}

GameState* GameStateManager::getCurrentState()
{
    if (!states.empty())
    {
        return states.back().get();
    }

    return nullptr;
}

void GameStateManager::handleInput(const sf::Event& event, sf::RenderWindow& window)
{
    if (getCurrentState())
    {
        getCurrentState()->handleInput(event, window);
    }
}

void GameStateManager::update(sf::Time deltaTime)
{
    if (getCurrentState())
    {
        getCurrentState()->update(deltaTime);
    }
}

void GameStateManager::render(sf::RenderWindow& window)
{
    if (getCurrentState())
    {
        getCurrentState()->render(window);
    }
}

bool GameStateManager::isEmpty() const
{
    return states.empty();
}