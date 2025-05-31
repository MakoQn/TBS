#include "Game.h"
#include "AssetManager.h"
#include "GameStateManager.h"
#include "MainMenuState.h"
#include "GameplayState.h"
#include "GameOverState.h"
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <optional>

Game::Game() : mainWindow(sf::VideoMode({ static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT) }), "TBS")
{
    mainWindow.setFramerateLimit(60);
    initialize();
}

Game::~Game()
{
    cleanup();
}

void Game::initialize()
{
    std::cout << "Инициализация игры..." << std::endl;

    AssetManager::getInstance();
    GameStateManager::getInstance();

    AssetManager::getInstance().getTexture("assets/background.png");
    AssetManager::getInstance().getTexture("assets/warrior.png");
    AssetManager::getInstance().getTexture("assets/archer.png");
    AssetManager::getInstance().getTexture("assets/tank.png");

    GameStateManager::getInstance().pushState(std::make_unique<MainMenuState>(), this);
}

void Game::cleanup()
{
    std::cout << "Очистка игры..." << std::endl;
}

void Game::run()
{
    std::cout << "Запуск игрового цикла..." << std::endl;

    while (mainWindow.isOpen())
    {
        sf::Time deltaTime = deltaClock.restart();

        processEvents();
        update(deltaTime);
        render();
    }

    std::cout << "Игровой цикл завершен." << std::endl;
}

void Game::processEvents()
{
    while (const std::optional eventOptional = mainWindow.pollEvent())
    {
        if (eventOptional)
        {
            const sf::Event& event = *eventOptional;

            if (event.is<sf::Event::Closed>())
            {
                mainWindow.close();
            }

            GameStateManager::getInstance().handleInput(event, mainWindow);
        }
    }
}

void Game::update(sf::Time deltaTime)
{
    GameStateManager::getInstance().update(deltaTime);
}

void Game::render()
{
    mainWindow.clear(sf::Color::Black);
    GameStateManager::getInstance().render(mainWindow);
    mainWindow.display();
}

sf::RenderWindow& Game::getWindow()
{
    return mainWindow;
}

AssetManager& Game::getAssetManager()
{
    return AssetManager::getInstance();
}

InputHandler& Game::getInputHandler()
{
    return mainInputHandler;
}

void Game::requestStateChange(GameStateId stateId, GameplayMode mode, const std::string& message)
{
    std::cout << "Запрос на смену состояния на: " << (int)stateId << std::endl;

    if (stateId == GameStateId::MAIN_MENU)
    {
        GameStateManager::getInstance().changeState(std::make_unique<MainMenuState>(), this);
    }
    else if (stateId == GameStateId::GAMEPLAY)
    {
        GameStateManager::getInstance().changeState(std::make_unique<GameplayState>(mode), this);
    }
    else if (stateId == GameStateId::GAME_OVER)
    {
        GameStateManager::getInstance().changeState(std::make_unique<GameOverState>(message), this);
    }
}