#include "TextureManager.h"
#include <stdexcept>

std::map<std::string, sf::Texture> TextureManager::textures;

sf::Texture& TextureManager::GetTexture(const std::string& name) {
    if (textures.find(name) == textures.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile("images/" + name)) {
            throw std::runtime_error("Failed to load texture: " + name);
        }
        textures[name] = texture;
    }
    return textures[name];
}

void TextureManager::Clear() {
    textures.clear();
}