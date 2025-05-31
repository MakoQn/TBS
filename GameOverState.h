#pragma once
#include "GameState.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>

class GameOverState : public GameState
{
private:
    sf::Font font;
    sf::Text gameOverText;
    sf::Text instructionText;
    std::string endMessage;

    void setupText();
public:
    GameOverState(const std::string& message);

    void init(Game* game) override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;
};