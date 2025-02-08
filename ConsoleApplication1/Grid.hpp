#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>

// Structure pour repr�senter une case dans la grille
struct Node {
    char letter = '\0';     // Lettre de la case
    bool isBlack = false;   // Si la case est noire (lettre inutile)
    bool isSelected = false;// Si la case est s�lectionn�e
    bool isStart = false;   // Si la case est le d�but d'un mot
    bool isEnd = false;     // Si la case est la fin d'un mot
    bool isHint = false;    // Si la case est un indice
};

// Classe pour repr�senter la grille
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

    // Fonction utilitaire pour g�n�rer un nombre entier al�atoire
    int randomInt(int min, int max);

    // Fonction pour g�n�rer un labyrinthe avec plusieurs chemins
    void generateMazeWithMultiplePaths();
    // Fonction pour afficher la grille dans la console
    void display() const;
};

#endif // GRID_HPP