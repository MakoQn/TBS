#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    static sf::Texture& getTexture(const std::string& filename);
    static sf::Font& getFont(const std::string& filename);

private:
    static std::map<std::string, std::unique_ptr<sf::Texture>> s_Textures;
    static std::map<std::string, std::unique_ptr<sf::Font>> s_Fonts;
    static sf::Font s_fallbackFont;
    static bool s_fallbackFontLoaded;
};

#endif