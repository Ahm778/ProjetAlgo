#ifndef MATRIXWINDOW_HPP
#define MATRIXWINDOW_HPP

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <vector>
#include "Grid.hpp"

class MatrixWindow {
private:
    sf::RenderWindow& window; // R�f�rence � la fen�tre SFML
    Grid& grid;               // R�f�rence � la grille du jeu
    const sf::Font& font;     // R�f�rence � la police de caract�res
    int& score;               // R�f�rence au score du joueur
    const std::vector<std::string>& themeWords; // R�f�rence aux mots du th�me
    sf::Clock& gameClock;     // R�f�rence � l'horloge du jeu
    std::unordered_set<std::string>& foundWords; // R�f�rence aux mots trouv�s

public:
    // Constructeur
    MatrixWindow(sf::RenderWindow& window, Grid& grid, const sf::Font& font, int& score, const std::vector<std::string>& themeWords, sf::Clock& gameClock, std::unordered_set<std::string>& foundWords);

    // M�thode pour ex�cuter la fen�tre
    void run();

private:
    // M�thode pour dessiner les �l�ments de la fen�tre
    void render();
};

#endif // MATRIXWINDOW_HPP
