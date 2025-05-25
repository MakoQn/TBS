#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <map>
#include <string>
#include <memory>

class AssetManager
{
private:
    AssetManager();
    ~AssetManager();

    std::map<std::string, sf::Texture> textures;
    static sf::Texture defaultTextureInstance;
    static bool defaultTextureLoaded;

    void loadTexture(const std::string& name, const std::string& filename);
    static void ensureDefaultTextureLoaded();
public:
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    static AssetManager& getInstance();

    sf::Texture& getTexture(const std::string& filename);
    static sf::Texture& getDefaultTexture();
};