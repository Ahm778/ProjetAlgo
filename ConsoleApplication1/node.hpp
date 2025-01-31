#ifndef NODE_HPP
#define NODE_HPP

#include <SFML/Graphics.hpp> // Pour utiliser sf::Color si nécessaire

struct Node {
    char letter = '\0';       // Lettre associée au nœud (par défaut vide)
    bool isBlack = false;     // Indique si la case est bloquée
    bool isSelected = false;  // Indique si la case est sélectionnée
    bool isStart = false;     // Indique si la case est le point de départ
    bool isEnd = false;       // Indique si la case est le point d'arrivée
    bool isHint = false;      // Indique si la case est un indice

    // Optionnel : Ajouter une couleur pour l'affichage
    sf::Color color = sf::Color::White; // Couleur par défaut
};

#endif // NODE_HPP