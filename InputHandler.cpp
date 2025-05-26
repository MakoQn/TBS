#include "InputHandler.h"

InputHandler::InputHandler() {}

bool InputHandler::isMouseButtonPressed(sf::Mouse::Button button, const sf::Event& event) const
{
    if (const auto* mbEvent = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mbEvent->button == button)
        {
            return true;
        }
    }

    return false;
}

sf::Vector2i InputHandler::getMousePosition(sf::RenderWindow& window) const
{
    return sf::Mouse::getPosition(window);
}