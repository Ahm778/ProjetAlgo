#include "MatrixWindow.hpp"

// D�finition du constructeur
MatrixWindow::MatrixWindow(sf::RenderWindow& window, Grid& grid, const sf::Font& font, int& score, const std::vector<std::string>& themeWords, sf::Clock& gameClock, std::unordered_set<std::string>& foundWords)
    : window(window), grid(grid), font(font), score(score), themeWords(themeWords), gameClock(gameClock), foundWords(foundWords) {
    // Corps du constructeur (si n�cessaire)
}

// D�finition de la m�thode run
void MatrixWindow::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        render();
    }
}

// D�finition de la m�thode render
void MatrixWindow::render() {
    window.clear();
    // Dessiner la grille, les boutons, etc.
    window.display();
}