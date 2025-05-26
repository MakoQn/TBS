#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class InputHandler
{
public:
    InputHandler();

    bool isMouseButtonPressed(sf::Mouse::Button button, const sf::Event& event) const;
    sf::Vector2i getMousePosition(sf::RenderWindow& window) const;
};