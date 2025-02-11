#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <queue>
#include <utility> // Pour std::pair

// Structure pour représenter une case dans la grille
struct Node {
    char letter = '\0';     // Lettre de la case
    bool isBlack = false;   // Si la case est noire (lettre inutile)
    bool isSelected = false;// Si la case est sélectionnée
    bool isStart = false;   // Si la case est le début d'un mot
    bool isEnd = false;     // Si la case est la fin d'un mot
    bool isHint = false;    // Si la case est un indice
};

// Structure pour Dijkstra
struct DijkstraNode {
    int x, y;
    int distance;
    DijkstraNode(int x, int y, int distance) : x(x), y(y), distance(distance) {}
    bool operator>(const DijkstraNode& other) const {
        return distance > other.distance;
    }
};

// Structure pour représenter une arête dans le labyrinthe
struct Edge {
    int x1, y1, x2, y2;
};

// Classe pour représenter la grille
// Dans Grid.hpp
class Grid {
private:
    int rows, cols;
    std::vector<std::vector<Node>> grid;
    std::vector<sf::Vector2i> validPath; // Stocke le chemin valide pour les indices

public:
    Grid(int r, int c);
    std::vector<std::string> selectRandomWords(const std::vector<std::string>& themeWords, int count);
    Node* getNode(int x, int y);
    int getRows() const;
    int getCols() const;

    void fillRandom();
    void fillWithTheme(const std::vector<std::string>& words, float blackCellProbability);
    void generateContinuousPath(const std::vector<std::string>& themeWords, int nbWords);
    bool dfsContinuousPath(int x, int y, const std::string& path, int index);

    const std::vector<sf::Vector2i>& getValidPath() const;

    // Fonction utilitaire pour générer un nombre entier aléatoire
    int randomInt(int min, int max);

    // Fonction pour générer un labyrinthe avec plusieurs chemins
    void generateMazeWithMultiplePaths();
    // Fonction pour afficher la grille dans la console
    void display() const;

    // Fonction pour trouver le plus court chemin avec Dijkstra
    std::vector<sf::Vector2i> dijkstra(int startX, int startY, int endX, int endY);
};

#endif // GRID_HPP