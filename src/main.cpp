#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <random>
#include <algorithm>
#include <iostream>
#include "Tile.h"
#include "TextureManager.h"

void LoadConfigFile(const std::string& path, int& columns, int& rows, int& mineCount) {
    std::ifstream file(path);
    if (file.is_open()) {
        file >> columns >> rows >> mineCount;
    }
}

void LoadBoardConfig(const std::string& filename, std::vector<Tile>& tiles, int columns, int rows) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    int row = 0;
    while (std::getline(file, line) && row < rows) {
        for (int col = 0; col < std::min((int)line.size(), columns); ++col) {
            if (line[col] == '1') {
                tiles[row * columns + col].SetMine(true);
            }
        }
        ++row;
    }
}

void SetAdjacentMineCounts(std::vector<Tile>& tiles, int columns, int rows) {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < columns; ++x) {
            int index = y * columns + x;
            if (tiles[index].IsMine()) continue;
            int count = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && ny >= 0 && nx < columns && ny < rows)
                        if (tiles[ny * columns + nx].IsMine()) count++;
                }
            }
            tiles[index].SetAdjacentMines(count);
        }
    }
}

void PlaceRandomMines(std::vector<Tile>& tiles, int mineCount) {
    std::vector<int> indices(tiles.size());
    for (int i = 0; i < tiles.size(); ++i) indices[i] = i;
    std::shuffle(indices.begin(), indices.end(), std::default_random_engine(static_cast<unsigned>(time(0))));
    for (int i = 0; i < mineCount; ++i) {
        tiles[indices[i]].SetMine(true);
    }
}

void UpdateDigitSprites(std::vector<sf::Sprite>& digits, int value, int startX, int startY) {
    digits.clear();

    bool isNegative = value < 0;
    int absVal = std::abs(value);

    int d1 = (absVal / 100) % 10;
    int d2 = (absVal / 10) % 10;
    int d3 = absVal % 10;

    for (int i = 0; i < 3; ++i) {
        sf::Sprite sprite(TextureManager::GetTexture("digits.png"));

        if (i == 0 && isNegative) {
            sprite.setTextureRect(sf::IntRect(210, 0, 21, 32));
        } else {
            int digit = (i == 0) ? d1 : (i == 1) ? d2 : d3;
            sprite.setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
        }

        sprite.setPosition(startX + (i * 21), startY);
        digits.push_back(sprite);
    }
}

int main() {
    int columns, rows, mineCount;
    LoadConfigFile("boards/config.cfg", columns, rows, mineCount);

    const int tileSize = 32;
    const int windowWidth = columns * tileSize;
    const int windowHeight = (rows * tileSize) + 100;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Minesweeper");
    window.setFramerateLimit(60);

    std::vector<Tile> tiles(rows * columns);
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < columns; ++x)
            tiles[y * columns + x].SetPosition(x * tileSize, y * tileSize);

    const int buttonY = rows * tileSize;

    sf::Sprite smiley(TextureManager::GetTexture("face_happy.png"));
    smiley.setPosition((windowWidth / 2) - 32, buttonY);

    const int buttonSpacing = 64;
    const int buttonStartX = windowWidth - buttonSpacing * 4;

    sf::Sprite debug(TextureManager::GetTexture("debug.png"));
    sf::Sprite test1(TextureManager::GetTexture("test_1.png"));
    sf::Sprite test2(TextureManager::GetTexture("test_2.png"));
    sf::Sprite test3(TextureManager::GetTexture("test_3.png"));

    debug.setPosition(buttonStartX + buttonSpacing * 0, buttonY);
    test1.setPosition(buttonStartX + buttonSpacing * 1, buttonY);
    test2.setPosition(buttonStartX + buttonSpacing * 2, buttonY);
    test3.setPosition(buttonStartX + buttonSpacing * 3, buttonY);

    bool isGameOver = false, isWin = false, debugMode = false;
    int flagsRemaining = mineCount;

    std::vector<sf::Sprite> mineCounterDigits;

    auto resetBoard = [&](bool randomize = true, const std::string& file = "") {
        for (auto& tile : tiles) tile.Reset();

        if (randomize) {
            PlaceRandomMines(tiles, mineCount);
        } else {
            LoadBoardConfig(file, tiles, columns, rows);
        }

        SetAdjacentMineCounts(tiles, columns, rows);
        isGameOver = false;
        isWin = false;
        debugMode = false;
        smiley.setTexture(TextureManager::GetTexture("face_happy.png"));
    };

    resetBoard(true);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f worldPos = sf::Vector2f(sf::Mouse::getPosition(window));

                if (smiley.getGlobalBounds().contains(worldPos)) {
                    resetBoard(true);
                }

                else if (!isGameOver && !isWin) {
                    if (debug.getGlobalBounds().contains(worldPos)) {
                        debugMode = !debugMode;
                    } else if (test1.getGlobalBounds().contains(worldPos)) {
                        resetBoard(false, "boards/testboard1.brd");
                    } else if (test2.getGlobalBounds().contains(worldPos)) {
                        resetBoard(false, "boards/testboard2.brd");
                    } else if (test3.getGlobalBounds().contains(worldPos)) {
                        resetBoard(false, "boards/testboard3.brd");
                    } else {
                        for (auto& tile : tiles) {
                            if (tile.Contains(worldPos)) {
                                if (event.mouseButton.button == sf::Mouse::Left) {
                                    if (!tile.IsFlagged()) {
                                        if (tile.IsMine()) {
                                            isGameOver = true;
                                            smiley.setTexture(TextureManager::GetTexture("face_lose.png"));
                                            for (auto& t : tiles)
                                                if (t.IsMine()) t.ForceReveal();
                                        } else {
                                            tile.Reveal(tiles, columns, rows);
                                        }
                                    }
                                } else if (event.mouseButton.button == sf::Mouse::Right) {
                                    tile.ToggleFlag();
                                }
                            }
                        }
                    }
                }
            }
        }

        if (!isGameOver && !isWin) {
            int unrevealed = 0;
            for (auto& tile : tiles)
                if (!tile.IsRevealed() && !tile.IsMine()) unrevealed++;

            if (unrevealed == 0) {
                isWin = true;
                smiley.setTexture(TextureManager::GetTexture("face_win.png"));
                for (auto& tile : tiles)
                    if (tile.IsMine() && !tile.IsFlagged())
                        tile.ToggleFlag();
            }
        }

        int flagsUsed = 0;
        for (auto& tile : tiles) if (tile.IsFlagged()) flagsUsed++;
        UpdateDigitSprites(mineCounterDigits, flagsRemaining - flagsUsed, 21, rows * tileSize);

        window.clear();
        for (auto& tile : tiles) {
            window.draw(tile.GetBaseSprite());
            if (tile.ShowNumber())    window.draw(tile.GetNumberSprite());
            if (tile.ShowFlag())      window.draw(tile.GetFlagSprite());
            if (tile.ShowMine())      window.draw(tile.GetMineSprite());

            if (debugMode && !tile.IsRevealed() && tile.IsMine()) {
                sf::Sprite debugMine = tile.GetMineSprite();
                window.draw(debugMine);
            }
        }

        for (auto& digit : mineCounterDigits)
            window.draw(digit);
        window.draw(smiley);
        window.draw(debug);
        window.draw(test1);
        window.draw(test2);
        window.draw(test3);
        window.display();
    }

    return 0;
}