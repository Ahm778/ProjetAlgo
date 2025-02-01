#include "Bubbles.hpp"
#include <random>

// Fonction pour générer un nombre aléatoire entre min et max
int randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// Fonction pour créer des bulles
std::vector<Bubble> createBubbles(int count, const sf::Vector2u& windowSize) {
    std::vector<Bubble> bubbles;
    for (int i = 0; i < count; ++i) {
        Bubble bubble;
        float radius = static_cast<float>(randomInt(10, 50));
        bubble.shape.setRadius(radius);
        bubble.shape.setFillColor(sf::Color(255, 255, 255, randomInt(50, 150))); // Couleur semi-transparente
        bubble.shape.setPosition(
            static_cast<float>(randomInt(0, windowSize.x)),
            static_cast<float>(randomInt(0, windowSize.y))
        );
        bubble.speedX = static_cast<float>(randomInt(-50, 50)) / 100.0f;
        bubble.speedY = static_cast<float>(randomInt(-50, 50)) / 100.0f;
        bubble.scaleSpeed = static_cast<float>(randomInt(1, 10)) / 1000.0f;
        bubbles.push_back(bubble);
    }
    return bubbles;
}

// Fonction pour mettre à jour les bulles
void updateBubbles(std::vector<Bubble>& bubbles, const sf::Vector2u& windowSize, float deltaTime) {
    for (auto& bubble : bubbles) {
        bubble.shape.move(bubble.speedX * deltaTime * 60, bubble.speedY * deltaTime * 60);

        sf::Vector2f position = bubble.shape.getPosition();
        float radius = bubble.shape.getRadius();

        if (position.x + radius < 0) {
            position.x = windowSize.x + radius;
        }
        else if (position.x - radius > windowSize.x) {
            position.x = -radius;
        }

        if (position.y + radius < 0) {
            position.y = windowSize.y + radius;
        }
        else if (position.y - radius > windowSize.y) {
            position.y = -radius;
        }

        bubble.shape.setPosition(position);
    }
}

// Fonction pour dessiner les bulles
void drawBubbles(sf::RenderWindow& window, const std::vector<Bubble>& bubbles) {
    for (const auto& bubble : bubbles) {
        window.draw(bubble.shape);
    }
}
