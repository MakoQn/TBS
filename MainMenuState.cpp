#include "MainMenuState.h"
#include "Game.h"
#include "AssetManager.h"
#include "GameplayState.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

MainMenuState::MainMenuState()
    : GameState(GameStateId::MAIN_MENU),
    menuFont(),
    titleText(this->menuFont, ""),
    playPvPText(this->menuFont, ""),
    playPvEText(this->menuFont, ""),
    exitText(this->menuFont, ""),
    selectedOption(0)
{
    if (!this->menuFont.openFromFile("assets/arial.ttf"))
    {
        std::cerr << "ОШИБКА: Не удалось загрузить шрифт 'assets/arial.ttf' в MainMenuState!" << std::endl;
    }
}

void MainMenuState::init(Game* game)
{
    gamePtr = game;

    setupText();
    updateSelectionDisplay();
}

void MainMenuState::setupText()
{
    titleText.setString("TBS");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);

    sf::FloatRect titleBounds = titleText.getLocalBounds();

    titleText.setOrigin(sf::Vector2f(titleBounds.position.x + titleBounds.size.x / 2.0f, titleBounds.position.y + titleBounds.size.y / 2.0f));
    titleText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 4.0f));

    playPvPText.setFont(menuFont);
    playPvPText.setString("Play PvP");
    playPvPText.setCharacterSize(30);

    sf::FloatRect pvpBounds = playPvPText.getLocalBounds();

    playPvPText.setOrigin(sf::Vector2f(pvpBounds.position.x + pvpBounds.size.x / 2.0f, pvpBounds.position.y + pvpBounds.size.y / 2.0f));
    playPvPText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));

    playPvEText.setFont(menuFont);
    playPvEText.setString("Play PvE");
    playPvEText.setCharacterSize(30);

    sf::FloatRect pveBounds = playPvEText.getLocalBounds();

    playPvEText.setOrigin(sf::Vector2f(pveBounds.position.x + pveBounds.size.x / 2.0f, pveBounds.position.y + pveBounds.size.y / 2.0f));
    playPvEText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 50.0f));

    exitText.setFont(menuFont);
    exitText.setString(L"Exit");
    exitText.setCharacterSize(30);

    sf::FloatRect exitBounds = exitText.getLocalBounds();

    exitText.setOrigin(sf::Vector2f(exitBounds.position.x + exitBounds.size.x / 2.0f, exitBounds.position.y + exitBounds.size.y / 2.0f));
    exitText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 100.0f));
}

void MainMenuState::updateSelectionDisplay()
{
    playPvPText.setFillColor(sf::Color::White);
    playPvEText.setFillColor(sf::Color::White);
    exitText.setFillColor(sf::Color::White);

    if (selectedOption == 0)
    {
        playPvPText.setFillColor(sf::Color::Yellow);
    }
    else if (selectedOption == 1)
    {
        playPvEText.setFillColor(sf::Color::Yellow);
    }
    else if (selectedOption == 2)
    {
        exitText.setFillColor(sf::Color::Yellow);
    }
}

void MainMenuState::handleInput(const sf::Event& event, sf::RenderWindow& window)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
        {
            selectedOption = (selectedOption + 3 - 1) % 3;
            updateSelectionDisplay();
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
        {
            selectedOption = (selectedOption + 1) % 3;
            updateSelectionDisplay();
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
        {
            if (selectedOption == 0)
            {
                gamePtr->requestStateChange(GameStateId::GAMEPLAY, GameplayMode::PVP);
            }
            else if (selectedOption == 1)
            {
                gamePtr->requestStateChange(GameStateId::GAMEPLAY, GameplayMode::PVE);
            }
            else if (selectedOption == 2)
            {
                window.close();
            }
        }
    }
}

void MainMenuState::update(sf::Time deltaTime) {}

void MainMenuState::render(sf::RenderWindow& window)
{
    window.draw(titleText);
    window.draw(playPvPText);
    window.draw(playPvEText);
    window.draw(exitText);
}