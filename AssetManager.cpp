#include "AssetManager.h"
#include <SFML/Graphics/Image.hpp>
#include <iostream>

sf::Texture AssetManager::defaultTextureInstance;
bool AssetManager::defaultTextureLoaded = false;

AssetManager& AssetManager::getInstance()
{
    static AssetManager instance;

    ensureDefaultTextureLoaded();

    return instance;
}

AssetManager::AssetManager()
{
    std::cout << "AssetManager создан." << std::endl;
}

AssetManager::~AssetManager()
{
    std::cout << "AssetManager уничтожен." << std::endl;

    textures.clear();
}

void AssetManager::ensureDefaultTextureLoaded()
{
    if (!defaultTextureLoaded)
    {
        sf::Image image({8, 8}, sf::Color::Red);

        if (defaultTextureInstance.loadFromImage(image))
        {
            std::cout << "Стандартная текстура успешно создана." << std::endl;
        }
        else
        {
            std::cerr << "Не удалось создать стандартную текстуру" << std::endl;
        }

        defaultTextureLoaded = true;
    }
}

sf::Texture& AssetManager::getDefaultTexture()
{
    ensureDefaultTextureLoaded();

    return defaultTextureInstance;
}

void AssetManager::loadTexture(const std::string& name, const std::string& filename)
{
    sf::Texture tex;

    if (tex.loadFromFile(filename))
    {
        textures[name] = tex;

        std::cout << "Загружена текстура: " << filename << " как " << name << std::endl;
    }
    else
    {
        std::cerr << "Не удалось загрузить текстуру: " << filename << std::endl;
    }
}

sf::Texture& AssetManager::getTexture(const std::string& name)
{
    if (textures.find(name) == textures.end())
    {
        loadTexture(name, name);
    }

    if (textures.find(name) != textures.end())
    {
        return textures.at(name);
    }
    else
    {
        std::cerr << "Текстура '" << name << "' не найдена после попытки загрузки. Возвращается стандартная текстура." << std::endl;

        return getDefaultTexture();
    }
}