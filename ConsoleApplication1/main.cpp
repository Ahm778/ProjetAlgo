#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include <iostream>

int main() {
    try {
        // Initialisation du jeu
        Game game;

        // V�rification que l'initialisation du jeu s'est bien d�roul�e
        std::cout << "Jeu initialis� avec succ�s. D�marrage de la boucle principale..." << std::endl;

        // Lancement de l'ex�cution du jeu
        game.run();

        // Message de fin de jeu
        std::cout << "Jeu termin�. Fermeture de l'application." << std::endl;
    }
    catch (const std::exception& e) {
        // Gestion des erreurs
        std::cerr << "Erreur critique : " << e.what() << std::endl;
        return -1; // Retourne un code d'erreur
    }
    catch (...) {
        // Gestion des exceptions non pr�vues
        std::cerr << "Erreur inconnue !" << std::endl;
        return -1; // Retourne un code d'erreur
    }

    // Retourne 0 si tout s'est bien pass�
    return 0;
}