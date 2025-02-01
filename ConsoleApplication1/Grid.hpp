#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>

// Structure pour représenter une case dans la grille
struct Node {
    char letter = '\0';     // Lettre de la case
    bool isBlack = false;   // Si la case est noire (lettre inutile)
    bool isSelected = false;// Si la case est sélectionnée
    bool isStart = false;   // Si la case est le début d'un mot
    bool isEnd = false;     // Si la case est la fin d'un mot
    bool isHint = false;    // Si la case est un indice
};

// Classe pour représenter la grille
class Grid {
private:
    int rows, cols;
    std::vector<std::vector<Node>> grid;
    std::vector<sf::Vector2i> validPath; // Stocke le chemin valide pour les indices

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

    // Fonction utilitaire pour générer un nombre entier aléatoire
    int randomInt(int min, int max);
};

#endif // GRID_HPP