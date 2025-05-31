#include "GameOverState.h"
#include "Game.h"
#include <iostream>

GameOverState::GameOverState(const std::string& message)
    : GameState(GameStateId::GAME_OVER), 
    font(),
    gameOverText(this->font, ""),
    instructionText(this->font, ""),
    endMessage(message)
{
    if (!this->font.openFromFile("assets/arial.ttf"))
    {
        std::cerr << "ОШИБКА: Не удалось загрузить шрифт 'assets/arial.ttf' в GameOverState!" << std::endl;
    }
}

void GameOverState::init(Game* game)
{
    gamePtr = game;

    setupText();
}

void GameOverState::setupText()
{
    gameOverText.setString(endMessage);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = gameOverText.getLocalBounds();

    gameOverText.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));
    gameOverText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 3.0f));

    instructionText.setString("Press Enter to exit the menu.");
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);

    sf::FloatRect instrBounds = instructionText.getLocalBounds();

    instructionText.setOrigin(sf::Vector2f(instrBounds.position.x + instrBounds.size.x / 2.0f, instrBounds.position.y + instrBounds.size.y / 2.0f));
    instructionText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 2.0f / 3.0f));
}

void GameOverState::handleInput(const sf::Event& event, sf::RenderWindow& window)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
        {
            if (gamePtr)
            {
                gamePtr->requestStateChange(GameStateId::MAIN_MENU);
            }
        }
    }
}

void GameOverState::update(sf::Time deltaTime) {}

void GameOverState::render(sf::RenderWindow& window)
{
    window.draw(gameOverText);
    window.draw(instructionText);
}