#ifndef NODE_HPP
#define NODE_HPP

#include <SFML/Graphics.hpp> // Pour utiliser sf::Color si n�cessaire

struct Node {
    char letter = '\0';       // Lettre associ�e au n�ud (par d�faut vide)
    bool isBlack = false;     // Indique si la case est bloqu�e
    bool isSelected = false;  // Indique si la case est s�lectionn�e
    bool isStart = false;     // Indique si la case est le point de d�part
    bool isEnd = false;       // Indique si la case est le point d'arriv�e
    bool isHint = false;      // Indique si la case est un indice

    // Optionnel : Ajouter une couleur pour l'affichage
    sf::Color color = sf::Color::White; // Couleur par d�faut
};

#endif // NODE_HPP