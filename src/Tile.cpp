#include "Tile.h"
#include "TextureManager.h"
#include <iostream>

Tile::Tile() {
    baseTile.setTexture(TextureManager::GetTexture("tile_hidden.png"));
    mineOverlay.setTexture(TextureManager::GetTexture("mine.png"));
    flagOverlay.setTexture(TextureManager::GetTexture("flag.png"));

    numberOverlay.setTexture(TextureManager::GetTexture("tile_revealed.png"));
    numberOverlay.setColor(sf::Color(0, 0, 0, 0)); // Fully transparent initially
}

void Tile::SetPosition(float x, float y) {
    baseTile.setPosition(x, y);
    numberOverlay.setPosition(x, y);
    mineOverlay.setPosition(x, y);
    flagOverlay.setPosition(x, y);
}

void Tile::Reset() {
    isRevealed = false;
    isFlagged = false;
    isMine = false;
    adjacentMines = 0;

    baseTile.setTexture(TextureManager::GetTexture("tile_hidden.png"));

    numberOverlay.setTexture(TextureManager::GetTexture("tile_revealed.png"));
    numberOverlay.setColor(sf::Color(0, 0, 0, 0)); // Fully transparent
}

void Tile::SetMine(bool mine) {
    isMine = mine;
}

bool Tile::IsMine() const {
    return isMine;
}

void Tile::SetAdjacentMines(int count) {
    std::cout << "Calling SetAdjacentMines(" << count << ")\n";

    adjacentMines = count;
    if (count > 0) {
        std::string filename = "number_" + std::to_string(count) + ".png";
        auto& tex = TextureManager::GetTexture(filename);
        numberOverlay.setTexture(tex);
        numberOverlay.setColor(sf::Color::White);  // Reset visibility
        std::cout << "Loaded: " << filename << " for adjacentMines = " << count << std::endl;
    } else {
        numberOverlay.setColor(sf::Color(0, 0, 0, 0));
    }
}

int Tile::GetAdjacentMines() const {
    return adjacentMines;
}

void Tile::Reveal(std::vector<Tile>& tiles, int columns, int rows) {
    if (isRevealed || isFlagged) return;

    isRevealed = true;
    baseTile.setTexture(TextureManager::GetTexture("tile_revealed.png"));

    if (isMine) return;

    if (adjacentMines > 0) {
        std::string filename = "number_" + std::to_string(adjacentMines) + ".png";
        numberOverlay.setTexture(TextureManager::GetTexture(filename));
        numberOverlay.setColor(sf::Color::White);
    }

    if (adjacentMines == 0) {
        int x = static_cast<int>(baseTile.getPosition().x) / 32;
        int y = static_cast<int>(baseTile.getPosition().y) / 32;

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx;
                int ny = y + dy;

                if (nx >= 0 && ny >= 0 && nx < columns && ny < rows) {
                    tiles[ny * columns + nx].Reveal(tiles, columns, rows);
                }
            }
        }
    }
}

void Tile::ForceReveal() {
    if (!isRevealed) {
        isRevealed = true;
        baseTile.setTexture(TextureManager::GetTexture("tile_revealed.png"));
    }
}

void Tile::ToggleFlag() {
    if (!isRevealed) {
        isFlagged = !isFlagged;
    }
}

bool Tile::Contains(sf::Vector2f point) const {
    return baseTile.getGlobalBounds().contains(point);
}

bool Tile::IsRevealed() const {
    return isRevealed;
}

bool Tile::IsFlagged() const {
    return isFlagged;
}

sf::Sprite Tile::GetBaseSprite() const {
    return baseTile;
}

bool Tile::ShowFlag() const {
    return isFlagged && !isRevealed;
}

sf::Sprite Tile::GetFlagSprite() const {
    return flagOverlay;
}

bool Tile::ShowNumber() const {
    return isRevealed && !isMine && adjacentMines > 0;
}

sf::Sprite Tile::GetNumberSprite() const {
    return numberOverlay;
}

bool Tile::ShowMine() const {
    return isRevealed && isMine;
}

sf::Sprite Tile::GetMineSprite() const {
    return mineOverlay;
}
