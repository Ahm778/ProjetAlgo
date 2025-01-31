
#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Node.hpp" // Inclure Node.hpp ici

class Grid {
private:
    int rows, cols;
    std::vector<std::vector<Node>> grid; // Utilisation de la classe Node
    std::vector<sf::Vector2i> validPath;

public:
    Grid(int r, int c);
    Node* getNode(int x, int y);
    int getRows() const;
    int getCols() const;
    void fillRandom();
    void fillWithTheme(const std::vector<std::string>& words, float blackCellProbability);
    void generateContinuousPath(const std::vector<std::string>& themeWords);
    bool dfsContinuousPath(int x, int y, const std::string& path, int index);
    const std::vector<sf::Vector2i>& getValidPath() const;

private:
    int randomInt(int min, int max);
};

#endif // GRID_HPP