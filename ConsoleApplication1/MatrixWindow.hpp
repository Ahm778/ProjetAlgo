#ifndef MATRIXWINDOW_HPP
#define MATRIXWINDOW_HPP

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <vector>
#include "Grid.hpp"

class MatrixWindow {
private:
    sf::RenderWindow& window; // Référence à la fenêtre SFML
    Grid& grid;               // Référence à la grille du jeu
    const sf::Font& font;     // Référence à la police de caractères
    int& score;               // Référence au score du joueur
    const std::vector<std::string>& themeWords; // Référence aux mots du thème
    sf::Clock& gameClock;     // Référence à l'horloge du jeu
    std::unordered_set<std::string>& foundWords; // Référence aux mots trouvés

public:
    // Constructeur
    MatrixWindow(sf::RenderWindow& window, Grid& grid, const sf::Font& font, int& score, const std::vector<std::string>& themeWords, sf::Clock& gameClock, std::unordered_set<std::string>& foundWords);

    // Méthode pour exécuter la fenêtre
    void run();

private:
    // Méthode pour dessiner les éléments de la fenêtre
    void render();
};

#endif // MATRIXWINDOW_HPP
