#include "Grid.hpp"
#include <iostream>
#include <algorithm>
#include <random>

// Constructeur
Grid::Grid(int r, int c) : rows(r), cols(c), grid(r, std::vector<Node>(c)) {}

// Getter pour un n�ud � la position (x, y)
Node* Grid::getNode(int x, int y) {
    if (x >= 0 && x < rows && y >= 0 && y < cols) {
        return &grid[x][y];
    }
    return nullptr; // Retourne nullptr si les coordonn�es sont hors limites
}

// Getter pour le nombre de lignes
int Grid::getRows() const { return rows; }

// Getter pour le nombre de colonnes
int Grid::getCols() const { return cols; }

// Remplit la grille avec des lettres al�atoires
void Grid::fillRandom() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].letter = 'A' + randomInt(0, 25); // Lettre al�atoire entre 'A' et 'Z'
            grid[i][j].isBlack = false; // Par d�faut, la case n'est pas bloqu�e
        }
    }
}

// Remplit la grille avec des mots d'un th�me donn�
void Grid::fillWithTheme(const std::vector<std::string>& words, float blackCellProbability) {
    // Impl�mentez cette m�thode pour int�grer les mots dans la grille
}

// G�n�re un chemin continu dans la grille
void Grid::generateContinuousPath(const std::vector<std::string>& themeWords) {
    // Impl�mentez cette m�thode pour g�n�rer un chemin valide
}

// V�rifie si un chemin continu existe (DFS)
bool Grid::dfsContinuousPath(int x, int y, const std::string& path, int index) {
    // Impl�mentez cette m�thode pour v�rifier un chemin
    return false;
}

// Getter pour le chemin valide
const std::vector<sf::Vector2i>& Grid::getValidPath() const { return validPath; }

// G�n�re un entier al�atoire entre min et max
int Grid::randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}