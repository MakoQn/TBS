#pragma once
#include "GameState.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class MainMenuState : public GameState
{
private:
    sf::Font menuFont;
    sf::Text titleText;
    sf::Text playPvPText;
    sf::Text playPvEText;
    sf::Text exitText;

    int selectedOption;

    void setupText();
    void updateSelectionDisplay();
public:
    MainMenuState();

    void init(Game* game) override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;
};