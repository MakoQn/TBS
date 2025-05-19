#include "AssetManager.h"
#include <iostream>

std::map<std::string, std::unique_ptr<sf::Texture>> AssetManager::s_Textures;
std::map<std::string, std::unique_ptr<sf::Font>> AssetManager::s_Fonts;
sf::Font AssetManager::s_fallbackFont;
bool AssetManager::s_fallbackFontLoaded = false;

AssetManager::AssetManager()
{
}
AssetManager::~AssetManager()
{
}

sf::Texture& AssetManager::getTexture(const std::string& filename)
{
    auto found = s_Textures.find(filename);
    if (found != s_Textures.end())
    {
        return *found->second;
    }
    else
    {
        auto texture = std::make_unique<sf::Texture>();
        if (texture->loadFromFile(filename))
        {
            s_Textures[filename] = std::move(texture);
            return *s_Textures[filename];
        }
        else
        {
            std::cerr << "Failed to load texture: " << filename << std::endl;
            static sf::Texture errorTexture;
            static bool errorTextureCreated = false;
            if (!errorTextureCreated)
            {
                errorTexture.create(32, 32);
                errorTextureCreated = true;
            }
            return errorTexture;
        }
    }
}

sf::Font& AssetManager::getFont(const std::string& filename)
{
    auto found = s_Fonts.find(filename);
    if (found != s_Fonts.end())
    {
        return *found->second;
    }
    else
    {
        auto font = std::make_unique<sf::Font>();
        if (font->loadFromFile(filename))
        {
            s_Fonts[filename] = std::move(font);
            return *s_Fonts[filename];
        }
        else
        {
            std::cerr << "Failed to load font: " << filename << std::endl;
            if (!s_fallbackFontLoaded)
            {
                s_fallbackFontLoaded = true;
            }
            return s_fallbackFont;
        }
    }
}