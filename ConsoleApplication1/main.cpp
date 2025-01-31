#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include <iostream>

int main() {
    try {
        // Initialisation du jeu
        Game game;

        // Vérification que l'initialisation du jeu s'est bien déroulée
        std::cout << "Jeu initialisé avec succès. Démarrage de la boucle principale..." << std::endl;

        // Lancement de l'exécution du jeu
        game.run();

        // Message de fin de jeu
        std::cout << "Jeu terminé. Fermeture de l'application." << std::endl;
    }
    catch (const std::exception& e) {
        // Gestion des erreurs
        std::cerr << "Erreur critique : " << e.what() << std::endl;
        return -1; // Retourne un code d'erreur
    }
    catch (...) {
        // Gestion des exceptions non prévues
        std::cerr << "Erreur inconnue !" << std::endl;
        return -1; // Retourne un code d'erreur
    }

    // Retourne 0 si tout s'est bien passé
    return 0;
}