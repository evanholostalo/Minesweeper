#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include <vector>

class Tile {
private:
    sf::Sprite baseTile;
    sf::Sprite numberOverlay;
    sf::Sprite mineOverlay;
    sf::Sprite flagOverlay;

    bool isRevealed = false;
    bool isFlagged = false;
    bool isMine = false;
    int adjacentMines = 0;

public:
    Tile();

    void SetPosition(float x, float y);
    void Reset();

    void SetMine(bool mine);
    bool IsMine() const;

    void SetAdjacentMines(int count);
    int GetAdjacentMines() const;

    void Reveal(std::vector<Tile>& tiles, int columns, int rows);
    void ForceReveal();
    void ToggleFlag();

    bool Contains(sf::Vector2f point) const;
    bool IsRevealed() const;
    bool IsFlagged() const;

    sf::Sprite GetBaseSprite() const;
    bool ShowFlag() const;
    sf::Sprite GetFlagSprite() const;
    bool ShowNumber() const;
    sf::Sprite GetNumberSprite() const;
    bool ShowMine() const;
    sf::Sprite GetMineSprite() const;
};

#endif
