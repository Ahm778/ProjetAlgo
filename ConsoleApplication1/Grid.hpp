#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <SFML/Graphics.hpp>

struct Node {
    char letter = ' ';
    bool isBlack = true;
    bool isSelected = false;
    bool isStart = false;
    bool isEnd = false;
    bool isHint = false;
};

struct Edge {
    int x1, y1, x2, y2;
};

class Grid {
public:
    Grid(int rows, int cols);

    Node* getNode(int x, int y);
    int getRows() const;
    int getCols() const;
    void fillRandom();
    void fillWithTheme(const std::vector<std::string>& words, float blackCellProbability);
    void generateMazeWithMultiplePaths();
    void generateContinuousPath(const std::vector<std::string>& themeWords);
    std::vector<std::string> selectRandomWords(const std::vector<std::string>& themeWords, int count);
    int randomInt(int min, int max);
    void display() const;

private:
    int rows, cols;
    std::vector<std::vector<Node>> grid;
    std::vector<sf::Vector2i> validPath;

    bool dfsContinuousPath(int x, int y, const std::string& path, int index);
};

#endif // GRID_HPP