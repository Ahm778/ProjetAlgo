#include "Bulle.hpp"
#include <random>

// Constructeur
Bulle::Bulle(float rayon, const sf::Vector2f& position, float vitesseX, float vitesseY, float vitesseEchelle)
    : vitesseX(vitesseX), vitesseY(vitesseY), vitesseEchelle(vitesseEchelle), rayonInitial(rayon) {
    forme.setRadius(rayon);
    forme.setPosition(position);
    forme.setFillColor(genererCouleurAleatoire());
}

// Méthode pour générer une couleur aléatoire
sf::Color Bulle::genererCouleurAleatoire() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 255);
    return sf::Color(distrib(gen), distrib(gen), distrib(gen), 100); // Couleur semi-transparente
}

// Méthode pour mettre à jour la position et l'échelle de la bulle
void Bulle::mettreAJour(float deltaTime, const sf::Vector2u& tailleFenetre) {
    // Déplacement de la bulle
    forme.move(vitesseX * deltaTime * 60, vitesseY * deltaTime * 60);

    // Gestion des bords de la fenêtre
    sf::Vector2f position = forme.getPosition();
    float rayon = forme.getRadius();

    if (position.x + rayon < 0) position.x = tailleFenetre.x + rayon;
    else if (position.x - rayon > tailleFenetre.x) position.x = -rayon;

    if (position.y + rayon < 0) position.y = tailleFenetre.y + rayon;
    else if (position.y - rayon > tailleFenetre.y) position.y = -rayon;

    forme.setPosition(position);

    // Animation de la taille (grandir et rétrécir)
    float scale = 1.0f + std::sin(vitesseEchelle * static_cast<float>(clock.getElapsedTime().asSeconds())) * 0.2f;
    forme.setRadius(rayonInitial * scale);
    forme.setOrigin(forme.getRadius(), forme.getRadius()); // Garder la bulle centrée
}

// Méthode pour vérifier les collisions entre bulles
bool Bulle::enCollisionAvec(const Bulle& autre) const {
    float distance = std::hypot(forme.getPosition().x - autre.forme.getPosition().x,
        forme.getPosition().y - autre.forme.getPosition().y);
    return distance < (forme.getRadius() + autre.forme.getRadius());
}