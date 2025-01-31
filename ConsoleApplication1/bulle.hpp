#ifndef BULLE_HPP
#define BULLE_HPP

#include <SFML/Graphics.hpp>

class Bulle {
private:
    sf::CircleShape forme; // Forme de la bulle
    float vitesseX; // Vitesse horizontale
    float vitesseY; // Vitesse verticale
    float vitesseEchelle; // Vitesse de changement d'échelle (pour l'animation)
    float rayonInitial; // Rayon initial de la bulle

public:
    // Constructeur pour initialiser la bulle avec la position, la vitesse et la vitesse d'échelle
    Bulle(float rayon, const sf::Vector2f& position, float vitesseX, float vitesseY, float vitesseEchelle);

    // Méthode pour générer une couleur aléatoire
    sf::Color genererCouleurAleatoire();

    // Met à jour la position et l'échelle de la bulle
    void mettreAJour(float deltaTime, const sf::Vector2u& tailleFenetre);

    // Vérifie si la bulle est en collision avec une autre bulle
    bool enCollisionAvec(const Bulle& autre) const;
};

#endif // BULLE_HPP