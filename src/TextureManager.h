#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class TextureManager {
public:
    static sf::Texture& GetTexture(const std::string& name);
    static void Clear();

private:
    static std::map<std::string, sf::Texture> textures;
};

#endif