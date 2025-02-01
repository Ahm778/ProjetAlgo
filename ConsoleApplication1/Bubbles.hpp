#ifndef BUBBLES_HPP
#define BUBBLES_HPP

#include <SFML/Graphics.hpp>
#include <vector>


struct Bubble {
    sf::CircleShape shape;
    float speedX, speedY;
    float scaleSpeed;
};

// Fonction pour g�n�rer un nombre al�atoire entre min et max
int randomInt(int min, int max);

// Fonction pour cr�er des bulles
std::vector<Bubble> createBubbles(int count, const sf::Vector2u& windowSize);

// Fonction pour mettre � jour les bulles
void updateBubbles(std::vector<Bubble>& bubbles, const sf::Vector2u& windowSize, float deltaTime);

// Fonction pour dessiner les bulles
void drawBubbles(sf::RenderWindow& window, const std::vector<Bubble>& bubbles);

#endif // BUBBLES_HPP
