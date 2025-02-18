#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <stack>
#include <queue>
#include "Bubbles.hpp"
#include "Grid.cpp"

// Fonction pour afficher la fenêtre de félicitations
void showCongratulationWindow(sf::RenderWindow& window, const sf::Font& font, int score, int timeElapsed) {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 720;
    // Créer une nouvelle fenêtre pour les félicitations
    sf::RenderWindow congratsWindow(sf::VideoMode(windowWidth, windowHeight), "Félicitations !");
    // Texte de félicitations
    sf::Text congratsText;
    congratsText.setFont(font);
    congratsText.setString("Félicitations !");
    congratsText.setCharacterSize(50);
    congratsText.setFillColor(sf::Color::White);
    congratsText.setStyle(sf::Text::Bold);
    congratsText.setPosition(
        static_cast<float>(windowWidth) / 2 - congratsText.getLocalBounds().width / 2, // Centré horizontalement
        100 // Position verticale
    );
    // Texte du score
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(
        static_cast<float>(windowWidth) / 2 - scoreText.getLocalBounds().width / 2, // Centré horizontalement
        200 // Position verticale
    );
    // Texte du temps écoulé
    sf::Text timeText;
    timeText.setFont(font);
    timeText.setString("Temps écoulé: " + std::to_string(timeElapsed) + " secondes");
    timeText.setCharacterSize(30);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(
        static_cast<float>(windowWidth) / 2 - timeText.getLocalBounds().width / 2, // Centré horizontalement
        250 // Position verticale
    );
    // Bouton "Recommencer"
    sf::RectangleShape restartButton(sf::Vector2f(200, 50));
    restartButton.setFillColor(sf::Color(70, 130, 180)); // Couleur bleue
    restartButton.setPosition(
        static_cast<float>(windowWidth) / 2 - restartButton.getSize().x / 2, // Centré horizontalement
        350 // Position verticale
    );
    sf::Text restartButtonText;
    restartButtonText.setFont(font);
    restartButtonText.setString("Recommencer");
    restartButtonText.setCharacterSize(24);
    restartButtonText.setFillColor(sf::Color::White);
    restartButtonText.setPosition(
        restartButton.getPosition().x + 30,
        restartButton.getPosition().y + 10
    );
    // Bouton "Menu Principal"
    sf::RectangleShape mainMenuButton(sf::Vector2f(200, 50));
    mainMenuButton.setFillColor(sf::Color(70, 130, 180)); // Couleur bleue
    mainMenuButton.setPosition(
        static_cast<float>(windowWidth) / 2 - mainMenuButton.getSize().x / 2, // Centré horizontalement
        450 // Position verticale
    );
    sf::Text mainMenuButtonText;
    mainMenuButtonText.setFont(font);
    mainMenuButtonText.setString("Menu Principal");
    mainMenuButtonText.setCharacterSize(24);
    mainMenuButtonText.setFillColor(sf::Color::White);
    mainMenuButtonText.setPosition(
        mainMenuButton.getPosition().x + 20,
        mainMenuButton.getPosition().y + 10
    );
    // Boucle principale de la fenêtre de félicitations
    while (congratsWindow.isOpen()) {
        sf::Event event;
        while (congratsWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                congratsWindow.close();
            }
            // Gestion des clics sur les boutons
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = congratsWindow.mapPixelToCoords(sf::Mouse::getPosition(congratsWindow));
                // Bouton "Recommencer"
                if (restartButton.getGlobalBounds().contains(mousePos)) {
                    congratsWindow.close();
                    return; // Retourner au jeu pour recommencer
                }
                // Bouton "Menu Principal"
                if (mainMenuButton.getGlobalBounds().contains(mousePos)) {
                    congratsWindow.close();
                    return; // Retourner au menu principal
                }
            }
        }
        // Effacer l'écran avec un dégradé de couleurs
        sf::VertexArray background(sf::Quads, 4);
        background[0].position = sf::Vector2f(0.0f, 0.0f);
        background[1].position = sf::Vector2f(static_cast<float>(windowWidth), 0);
        background[2].position = sf::Vector2f(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
        background[3].position = sf::Vector2f(0, static_cast<float>(windowHeight));
        background[0].color = sf::Color(100, 150, 200);
        background[1].color = sf::Color(100, 150, 200);
        background[2].color = sf::Color(200, 100, 150);
        background[3].color = sf::Color(200, 100, 150);
        congratsWindow.draw(background);
        // Dessiner les textes
        congratsWindow.draw(congratsText);
        congratsWindow.draw(scoreText);
        congratsWindow.draw(timeText);
        // Dessiner les boutons
        congratsWindow.draw(restartButton);
        congratsWindow.draw(restartButtonText);
        congratsWindow.draw(mainMenuButton);
        congratsWindow.draw(mainMenuButtonText);
        // Afficher à l'écran
        congratsWindow.display();
    }
}

// Fonction pour afficher la deuxième fenêtre (matrice)
void showMatrixWindow(sf::RenderWindow& window, Grid& grid, const sf::Font& font, int& score, const std::vector<std::string>& themeWords, sf::Clock& gameClock, std::unordered_set<std::string>& foundWords, float cellSize, int nbWords) {
    const int gridSize = grid.getRows();
    const float marginX = (window.getSize().x - gridSize * cellSize) / 2; // Centrer horizontalement
    const float marginY = (window.getSize().y - gridSize * cellSize) / 2 - 100; // Centrer verticalement

    // Création des cases
    std::vector<sf::RectangleShape> cells;
    for (unsigned int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            if (grid.getNode(x, y)->isBlack) {
                cell.setFillColor(sf::Color::Black); // Case noire
            }
            else if (grid.getNode(x, y)->isSelected) {
                cell.setFillColor(sf::Color::Yellow); // Case sélectionnée
            }
            else if (grid.getNode(x, y)->isStart) {
                cell.setFillColor(sf::Color::Cyan); // Case de départ en vert
            }
            else if (grid.getNode(x, y)->isEnd) {
                cell.setFillColor(sf::Color(204, 153, 255)); // Case d'arrivée en violet pastel
            }
            else if (grid.getNode(x, y)->isHint) {
                cell.setFillColor(sf::Color::Green); // Case indice (vert)
            }
            else {
                cell.setFillColor(sf::Color(240, 248, 255)); // Couleur des cases (blanc cassé)
            }
            cell.setOutlineThickness(2.f);
            cell.setOutlineColor(sf::Color::Black);
            cell.setPosition(marginX + x * cellSize, marginY + y * cellSize);
            cells.push_back(cell);
        }
    }

    // Bouton "Retour"
    sf::RectangleShape backButton(sf::Vector2f(100, 50));
    backButton.setFillColor(sf::Color(70, 130, 180)); // Couleur bleue
    backButton.setPosition(20, 20); // Position en haut à gauche

    sf::Text backButtonText;
    backButtonText.setFont(font);
    backButtonText.setString("Retour");
    backButtonText.setCharacterSize(24);
    backButtonText.setFillColor(sf::Color::White);
    backButtonText.setPosition(backButton.getPosition().x + 15, backButton.getPosition().y + 10);

    // Rectangle pour afficher le mot formé
    sf::RectangleShape wordBox(sf::Vector2f(300, 50));
    wordBox.setFillColor(sf::Color::White);
    wordBox.setPosition(marginX, marginY + gridSize * cellSize + 20);

    sf::Text wordText;
    wordText.setFont(font);
    wordText.setCharacterSize(24);
    wordText.setFillColor(sf::Color::Black);
    wordText.setPosition(wordBox.getPosition().x + 10, wordBox.getPosition().y + 10);

    // Bouton "Valider"
    sf::RectangleShape validateButton(sf::Vector2f(150, 50));
    validateButton.setFillColor(sf::Color(70, 130, 180)); // Couleur bleue
    validateButton.setPosition(marginX + gridSize * cellSize - 150, marginY + gridSize * cellSize + 20);

    sf::Text validateButtonText;
    validateButtonText.setFont(font);
    validateButtonText.setString("Valider");
    validateButtonText.setCharacterSize(24);
    validateButtonText.setFillColor(sf::Color::White);
    validateButtonText.setPosition(validateButton.getPosition().x + 30, validateButton.getPosition().y + 10);

    // Bouton "Hint"
    sf::RectangleShape hintButton(sf::Vector2f(150, 50));
    hintButton.setFillColor(sf::Color(70, 130, 180)); // Couleur bleue
    hintButton.setPosition(marginX + gridSize * cellSize - 150, marginY + gridSize * cellSize + 80);

    sf::Text hintButtonText;
    hintButtonText.setFont(font);
    hintButtonText.setString("Hint");
    hintButtonText.setCharacterSize(24);
    hintButtonText.setFillColor(sf::Color::White);
    hintButtonText.setPosition(hintButton.getPosition().x + 50, hintButton.getPosition().y + 10);

    // Texte du score
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(marginX, marginY + gridSize * cellSize + 80);

    // Texte du minuteur
    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(window.getSize().x - 150, 20);

    // Variables pour gérer la sélection des lettres
    std::vector<sf::Vector2i> selectedLetters;
    std::string currentWord;

    // Variable pour gérer l'indice de la lettre suivante à afficher avec le hint
    int hintIndex = 0;

    // Variable pour stocker le temps de validation du dernier mot
    sf::Time lastWordValidationTime = sf::Time::Zero;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Gestion du clic sur le bouton "Retour"
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (backButton.getGlobalBounds().contains(mousePos)) {
                    return; // Retour au menu principal
                }

                // Gestion du clic sur le bouton "Valider"
                if (validateButton.getGlobalBounds().contains(mousePos)) {
                    // Convertir le mot sélectionné en majuscules
                       // ken 3andi  l 1er case w last case ; nchouf longueur =? langueur de plus courte chemin wala le : ken yes ==> valid
                    //code yacin
                    if (validateButton.getGlobalBounds().contains(mousePos)) {
                        // Vérifier si la première et la dernière case sont bien le début et la fin du chemin
                        auto start = selectedLetters.begin();
                        auto end = std::prev(selectedLetters.end());
                        bool isStartNode = grid.getNode(start->x, start->y)->isStart;
                        bool isEndNode = grid.getNode(end->x, end->y)->isEnd;

                        if (isStartNode && isEndNode) {
                            // Calculer le plus court chemin entre le début et la fin
                            std::vector<sf::Vector2i> shortestPath = grid.dijkstra(start->x, start->y, end->x, end->y);
                            int shortestPathLength = shortestPath.size();
                            int selectedPathLength = selectedLetters.size();

                            // Calculer le score en fonction de la longueur du chemin sélectionné
                            if (selectedPathLength == shortestPathLength) {
                                std::cout << "Plus court chemin" << std::endl;
                                score = 50; // Ajouter 50 points si le chemin est le plus court
                            }
                            else {
                                int extraSteps = selectedPathLength - shortestPathLength;
                                score = 50 - 5 * extraSteps; // Ajouter 50 points et soustraire 2 points par case supplémentaire
                                std::cout << "Pas le plus court chemin" << std::endl;
                            }

                            // Réinitialiser le temps de validation du dernier mot
                            lastWordValidationTime = sf::Time::Zero;

                            // Mettre à jour l'affichage du temps
                            timerText.setString("Temps: 0s");

                            // Afficher le score bonus
                            int timeBonus = 0;
                            sf::Time currentTime = gameClock.getElapsedTime();
                            sf::Time timeSinceLastWord = currentTime - lastWordValidationTime;

                            if (timeSinceLastWord.asSeconds() <= 20) {
                                timeBonus = 20 - static_cast<int>(timeSinceLastWord.asSeconds());
                            }

                            int hintPenalty = hintIndex * 2;
                            int wordScore = (selectedPathLength * 3) + timeBonus - hintPenalty;
                            wordScore = std::max(wordScore, 0);

                            score += wordScore;
                            scoreText.setString("Score: " + std::to_string(score));

                            // Désélectionner le chemin sélectionné
                            for (const auto& pos : selectedLetters) {
                                grid.getNode(pos.x, pos.y)->isSelected = false;
                            }
                            selectedLetters.clear();
                            currentWord.clear();
                            wordText.setString("");

                            // Colorer le plus court chemin en gris (sans utiliser isHint)
                            //for (const auto& pos : shortestPath) {
                             //   grid.getNode(pos.x, pos.y)->isHint = true;
                           // }
                        }
                    }
                    //code yacin wfe ;

                    std::string upperCurrentWord = currentWord;
                    for (char& c : upperCurrentWord) {
                        c = toupper(c);
                    }

                    // Vérifier si le mot est valide (appartient au thème)
                    bool isValid = false;
                    for (const auto& word : themeWords) {
                        std::string upperWord = word;
                        for (char& c : upperWord) {
                            c = toupper(c);
                        }
                        if (upperCurrentWord == upperWord) {
                            isValid = true;
                            break;
                        }
                    }

                    // Dans la fonction showMatrixWindow, après la validation d'un mot
                    if (isValid) {
                        // Vérifier si le mot a déjà été trouvé
                        if (foundWords.find(upperCurrentWord) == foundWords.end()) {
                            foundWords.insert(upperCurrentWord); // Ajouter le mot à la liste des mots trouvés

                            // Calculer le score selon la nouvelle formule
                            int wordLength = upperCurrentWord.length();
                            sf::Time currentTime = gameClock.getElapsedTime();
                            sf::Time timeSinceLastWord = currentTime - lastWordValidationTime;

                            // Calculer le bonus de temps
                            int timeBonus = 0;
                            if (timeSinceLastWord.asSeconds() <= 20) {
                                timeBonus = 20 - static_cast<int>(timeSinceLastWord.asSeconds());
                            }

                            // Pénalité pour les indices utilisés
                            int hintPenalty = hintIndex * 2;

                            // Calculer le score du mot
                            int wordScore = (wordLength * 3) + timeBonus - hintPenalty;

                            // Assurer que le score ne devienne pas négatif
                            wordScore = std::max(wordScore, 0);

                            // Ajouter le score au score total
                            score += wordScore;
                            scoreText.setString("Score: " + std::to_string(score));

                            // Mettre à jour le temps de validation du dernier mot
                            lastWordValidationTime = currentTime;
                        }

                        // Colorer les cases sélectionnées en vert
                        for (const auto& pos : selectedLetters) {
                            grid.getNode(pos.x, pos.y)->isSelected = false;
                            grid.getNode(pos.x, pos.y)->isHint = true; // Marquer comme indice
                        }
                        selectedLetters.clear();
                        currentWord.clear();
                        wordText.setString(""); // Effacer le mot affiché
                    }
                    else {
                        // Afficher un message d'erreur ou un feedback visuel
                        wordText.setString("Mot invalide");
                    }
                }

                // Gestion du clic sur le bouton "Hint"
                if (hintButton.getGlobalBounds().contains(mousePos)) {
                    if (hintIndex < grid.getValidPath().size()) {
                        auto pos = grid.getValidPath()[hintIndex];
                        grid.getNode(pos.x, pos.y)->isHint = true;
                        hintIndex++; // Passer à la prochaine lettre du chemin
                    }
                    else {
                        std::cout << "Tous les mots ont été trouvés !" << std::endl; // Log pour vérifier
                    }
                }

                // Gestion du clic sur les cases
                for (int y = 0; y < gridSize; ++y) {
                    for (int x = 0; x < gridSize; ++x) {
                        if (cells[y * gridSize + x].getGlobalBounds().contains(mousePos) && !grid.getNode(x, y)->isBlack) {
                            // Vérifier si la case est adjacente à la dernière case sélectionnée
                            if (selectedLetters.empty() ||
                                (abs(x - selectedLetters.back().x) <= 1 && abs(y - selectedLetters.back().y) <= 1)) {
                                grid.getNode(x, y)->isSelected = !grid.getNode(x, y)->isSelected;
                                if (grid.getNode(x, y)->isSelected) {
                                    selectedLetters.push_back(sf::Vector2i(x, y));
                                    currentWord += grid.getNode(x, y)->letter;
                                }
                                else {
                                    selectedLetters.erase(std::remove(selectedLetters.begin(), selectedLetters.end(), sf::Vector2i(x, y)), selectedLetters.end());
                                    currentWord.erase(std::remove(currentWord.begin(), currentWord.end(), grid.getNode(x, y)->letter), currentWord.end());
                                }
                                wordText.setString(currentWord);
                            }
                        }
                    }
                }
            }
        }

        // Effacer l'écran avec un dégradé de couleurs
        sf::VertexArray background(sf::Quads, 4);
        background[0].position = sf::Vector2f(0, 0);
        background[1].position = sf::Vector2f(static_cast<float>(window.getSize().x), 0);
        background[2].position = sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
        background[3].position = sf::Vector2f(0, static_cast<float>(window.getSize().y));
        background[0].color = sf::Color(100, 150, 200);
        background[1].color = sf::Color(100, 150, 200);
        background[2].color = sf::Color(200, 100, 150);
        background[3].color = sf::Color(200, 100, 150);
        window.draw(background);

        // Dessiner les cases


        // Dessiner les cases
        for (size_t i = 0; i < cells.size(); ++i) {
            int x = static_cast<int>(i) % gridSize;
            int y = static_cast<int>(i) / gridSize;
            if (grid.getNode(x, y)->isSelected) {
                cells[i].setFillColor(sf::Color::Yellow); // Case sélectionnée
            }
            else if (grid.getNode(x, y)->isBlack) {
                cells[i].setFillColor(sf::Color(21, 27, 84)); // Case noire (21, 27, 84)
            }
            else if (grid.getNode(x, y)->isStart) {
                cells[i].setFillColor(sf::Color::Cyan); // Case de départ en vert
            }
            else if (grid.getNode(x, y)->isEnd) {
                cells[i].setFillColor(sf::Color(204, 153, 255)); // Case d'arrivée en violet pastel
            }
            else if (grid.getNode(x, y)->isHint) {
                cells[i].setFillColor(sf::Color::Transparent); // Case indice (vert)
            }
            else {
                cells[i].setFillColor(sf::Color(240, 248, 255)); // Case normale
            }
            window.draw(cells[i]);

            // Afficher la lettre dans la case (si ce n'est pas une case noire)
            if (!grid.getNode(x, y)->isBlack) {
                sf::Text letterText;
                letterText.setFont(font);
                letterText.setString(std::string(1, grid.getNode(x, y)->letter));
                letterText.setCharacterSize(18);
                letterText.setFillColor(sf::Color::Black);
                letterText.setPosition(cells[i].getPosition().x + 2.0f, cells[i].getPosition().y + -1.5f);
                window.draw(letterText);
            }
        }

        // Dessiner le bouton "Retour"
        window.draw(backButton);
        window.draw(backButtonText);

        // Dessiner le rectangle pour afficher le mot formé
        window.draw(wordBox);
        window.draw(wordText);

        // Dessiner le bouton "Valider"
        window.draw(validateButton);
        window.draw(validateButtonText);

        // Dessiner le bouton "Hint"
        window.draw(hintButton);
        window.draw(hintButtonText);

        // Dessiner le score
        window.draw(scoreText);

        // Dessiner le minuteur
        sf::Time elapsed = gameClock.getElapsedTime();
        int seconds = static_cast<int>(elapsed.asSeconds());
        timerText.setString("Temps: " + std::to_string(seconds) + "s");
        window.draw(timerText);

        // Vérifier si tous les mots ont été trouvés
        if (foundWords.size() == nbWords) { //if (foundWords.size() == themeWords.size()) {
            int timeElapsed = static_cast<int>(gameClock.getElapsedTime().asSeconds());
            showCongratulationWindow(window, font, score, timeElapsed);
            return; // Retourner au menu principal ou recommencer
        }

        // Afficher à l'écran
        window.display();
    }
}

// Fonction pour afficher les mots du thème
void drawThemeWords(sf::RenderWindow& window, const sf::Font& font, const std::vector<std::string>& words, float x, float y) {
    sf::Text themeTitle;
    themeTitle.setFont(font);
    themeTitle.setCharacterSize(24);
    themeTitle.setFillColor(sf::Color::White);
    themeTitle.setPosition(x, y);
}

void showIntroductionWindow(sf::RenderWindow& window, const sf::Font& font, const std::string& text, int page) {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 720;

    // Créer une nouvelle fenêtre pour l'introduction
    sf::RenderWindow introWindow(sf::VideoMode(windowWidth, windowHeight), "Introduction");

    // Texte d'introduction
    sf::Text introText;
    introText.setFont(font);
    introText.setString(text);
    introText.setCharacterSize(65); // Taille initiale augmentée
    introText.setFillColor(sf::Color::White);
    introText.setStyle(sf::Text::Bold);

    // Ajuster la taille du texte pour qu'il tienne dans la fenêtre
    float maxTextWidth = windowWidth - 80; // Marge de 50 pixels de chaque côté
    while (introText.getLocalBounds().width > maxTextWidth && introText.getCharacterSize() > 20) {
        introText.setCharacterSize(introText.getCharacterSize() - 1); // Réduire la taille du texte
    }

    // Centrer le texte horizontalement et le déplacer un peu plus haut verticalement
    introText.setPosition(
        static_cast<float>(windowWidth) / 2 - introText.getLocalBounds().width / 2, // Centré horizontalement
        static_cast<float>(windowHeight) / 2 - introText.getLocalBounds().height / 2 - 70 // Déplacé 50 pixels plus haut
    );

    // Bouton "Skip"
    sf::RectangleShape skipButton(sf::Vector2f(200, 50));
    skipButton.setFillColor(sf::Color(204, 153, 255)); // Couleur bleue bbbb
    skipButton.setPosition(
        static_cast<float>(windowWidth) / 2 - skipButton.getSize().x / 2, // Centré horizontalement
        500 // Position verticale
    );
    skipButton.setOutlineThickness(2.f);
    skipButton.setOutlineColor(sf::Color(75, 0, 130));

    sf::Text skipButtonText;
    skipButtonText.setFont(font);
    skipButtonText.setString("Skip");
    skipButtonText.setCharacterSize(24);
    skipButtonText.setFillColor(sf::Color(75, 0, 130));
    skipButtonText.setPosition(
        skipButton.getPosition().x + 70,
        skipButton.getPosition().y + 10
    );

    // Effet de dégradé animé
    sf::Clock gradientClock;
    float gradientPhase = 0.0f;

    // Bulles flottantes
    std::vector<Bubble> bubbles = createBubbles(20, introWindow.getSize());

    // Boucle principale de la fenêtre d'introduction
    while (introWindow.isOpen()) {
        sf::Event event;
        while (introWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                introWindow.close();
            }

            // Gestion des clics sur les boutons
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = introWindow.mapPixelToCoords(sf::Mouse::getPosition(introWindow));
                // Bouton "Skip"
                if (skipButton.getGlobalBounds().contains(mousePos)) {
                    introWindow.close();
                    return; // Retourner au menu principal
                }
            }
        }

        // Mettre à jour le dégradé animé
        float gradientTime = gradientClock.getElapsedTime().asSeconds();
        gradientPhase = std::sin(gradientTime * 0.5f) * 0.5f + 0.5f; // Oscillation entre 0 et 1

        // Effacer l'écran avec un dégradé de couleurs animé
        sf::VertexArray background(sf::Quads, 4);
        background[0].position = sf::Vector2f(0.0f, 0.0f);
        background[1].position = sf::Vector2f(static_cast<float>(windowWidth), 0);
        background[2].position = sf::Vector2f(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
        background[3].position = sf::Vector2f(0, static_cast<float>(windowHeight));
        background[0].color = sf::Color(100, 150, static_cast<sf::Uint8>(200 * gradientPhase));
        background[1].color = sf::Color(100, 150, static_cast<sf::Uint8>(200 * gradientPhase));
        background[2].color = sf::Color(200, 100, static_cast<sf::Uint8>(150 * gradientPhase));
        background[3].color = sf::Color(200, 100, static_cast<sf::Uint8>(150 * gradientPhase));
        introWindow.draw(background);

        // Mettre à jour et dessiner les bulles
        updateBubbles(bubbles, introWindow.getSize(), 0.001f); // 0.016f pour ~60 FPS
        for (const auto& bubble : bubbles) {
            introWindow.draw(bubble.shape);
        }

        // Dessiner les textes
        introWindow.draw(introText);

        // Effet de texte pulsé
        float textScale = 1.0f + std::sin(gradientTime * 2.0f) * 0.1f; // Pulsation légère
        introText.setScale(textScale, textScale);

        // Dessiner les boutons
        introWindow.draw(skipButton);
        introWindow.draw(skipButtonText);

        // Afficher à l'écran
        introWindow.display();
    }
}

int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 720;
    int nbWords = 5;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Jeu de Mots");

    // Charger une police
    sf::Font font;
    if (!font.loadFromFile("Roboto.ttf")) {
        std::cerr << "Erreur: Impossible de charger la police Roboto.ttf" << std::endl;
        return -1;
    }
    // Afficher les fenêtres d'introduction
    showIntroductionWindow(window, font,
        "Bienvenue dans le jeu de mots !\n\n"
        "Votre mission est de trouver des mots cachés dans une grille de lettres.\n"
        "Ces mots appartiennent à un thème que vous aurez choisi, comme les fruits,\n"
        "les pays ou les prénoms.\n\n"
        "Explorez la grille, sélectionnez les lettres et formez des mots pour marquer\n"
        "des points. Êtes-vous prêt à relever le défi ?",
        1);

    showIntroductionWindow(window, font,
        "Comment jouer :\n\n"
        "1. Utilisez votre souris pour sélectionner les lettres dans la grille.\n"
        "2. Les lettres doivent être adjacentes (horizontalement, verticalement\n"
        "   ou en diagonale).\n"
        "3. Formez un mot valide en sélectionnant les lettres dans l'ordre.\n"
        "4. Cliquez sur 'Valider' pour vérifier si le mot est correct.\n\n"
        "Astuce : Les mots doivent appartenir au thème choisi. Soyez attentif !",
        2);

    showIntroductionWindow(window, font,
        "Besoin d'aide ? Utilisez les indices !\n\n"
        "Si vous êtes bloqué, cliquez sur le bouton 'Hint' pour obtenir un indice.\n"
        "L'indice vous montrera la prochaine lettre d'un mot à trouver.\n\n"
        "Attention : Utiliser des indices réduit légèrement votre score final.\n"
        "Alors, utilisez-les avec parcimonie !",
        3);

    showIntroductionWindow(window, font,
        "Objectifs et score :\n\n"
        "- Trouvez tous les mots du thème pour gagner la partie.\n"
        "- Votre score dépend de la longueur des mots trouvés et de la rapidité\n"
        "  avec laquelle vous les trouvez.\n"
        "- Bonus : Trouvez les mots rapidement pour gagner des points supplémentaires.\n"
        "- Pénalité : Utiliser des indices réduit votre score.\n\n"
        "Le chronomètre est lancé ! À vous de jouer !",
        4);

    // Couleurs d'arrière-plan
    sf::Color backgroundColorStart = sf::Color(100, 150, 200); // Début du dégradé
    sf::Color backgroundColorEnd = sf::Color(200, 100, 150);   // Fin du dégradé

    // Texte de bienvenue
    sf::Text welcomeText;
    welcomeText.setFont(font);
    welcomeText.setString("Bienvenue dans votre jeu !");
    welcomeText.setCharacterSize(50);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold);
    welcomeText.setPosition(
        static_cast<float>(windowWidth) / 2 - welcomeText.getLocalBounds().width / 2, // Centré horizontalement
        100 // Position verticale
    );

    // Boutons
    sf::RectangleShape themeButton(sf::Vector2f(300, 50));
    themeButton.setFillColor(sf::Color(70, 130, 180)); // Couleur bleue
    themeButton.setPosition(
        static_cast<float>(windowWidth) / 2 - themeButton.getSize().x / 2, // Centré horizontalement
        200 // Position verticale
    );

    sf::RectangleShape difficultyButton(sf::Vector2f(300, 50));
    difficultyButton.setFillColor(sf::Color(70, 130, 180)); // Couleur bleue
    difficultyButton.setPosition(
        static_cast<float>(windowWidth) / 2 - difficultyButton.getSize().x / 2, // Centré horizontalement
        350 // Position verticale
    );

    sf::RectangleShape startButton(sf::Vector2f(300, 50));
    startButton.setFillColor(sf::Color(70, 130, 180)); // Couleur bleue
    startButton.setPosition(
        static_cast<float>(windowWidth) / 2 - startButton.getSize().x / 2, // Centré horizontalement
        500 // Position verticale
    );

    // Texte des boutons
    sf::Text themeButtonText;
    themeButtonText.setFont(font);
    themeButtonText.setString("Choisir un thème");
    themeButtonText.setCharacterSize(24);
    themeButtonText.setFillColor(sf::Color::White);
    themeButtonText.setPosition(
        themeButton.getPosition().x + 50,
        themeButton.getPosition().y + 10
    );

    sf::Text difficultyButtonText;
    difficultyButtonText.setFont(font);
    difficultyButtonText.setString("Choisir la difficulté");
    difficultyButtonText.setCharacterSize(24);
    difficultyButtonText.setFillColor(sf::Color::White);
    difficultyButtonText.setPosition(
        difficultyButton.getPosition().x + 30,
        difficultyButton.getPosition().y + 10
    );

    sf::Text startButtonText;
    startButtonText.setFont(font);
    startButtonText.setString("Commencer");
    startButtonText.setCharacterSize(24);
    startButtonText.setFillColor(sf::Color::White);
    startButtonText.setPosition(
        startButton.getPosition().x + 90,
        startButton.getPosition().y + 10
    );

    // Variables pour stocker le thème choisi
    std::vector<std::string> selectedTheme;
    std::string selectedThemeName;

    // Variables pour stocker la difficulté choisie
    float cellSize = 0.0f;
    int gridSize = 0;

    // Créer la grille
    Grid grid(15, 15);
    std::vector<std::string> fruits = {
    "Pomme", "Banane", "Orange", "Fraise", "Kiwi", "Mangue", "Ananas", "Raisin", "Poire", "Cerise",
    "Abricot", "Myrtille", "Framboise", "Pastèque", "Melon", "Goyave", "Papaye", "Grenade", "Litchi", "Pêche"
    };
    std::vector<std::string> pays = {
     "France", "Tunisie", "Qatar", "Pérou", "Japon", "Canada", "Djibouti", "Cuba", "Brésil", "Espagne",
     "Italie", "Allemagne", "Maroc", "Russie", "Inde", "Chine", "Mexique", "Portugal", "Égypte", "Turquie"
    };
    std::vector<std::string> prenoms = {
    "Alice", "Bob", "Charlie", "David", "Eve", "Frank", "Ahmed", "Zeineb", "Asma", "Saif",
    "Hana", "Omar", "Lina", "Youssef", "Rania", "Jules", "Camille", "Nina", "Leo", "Lucas"
    };
    // Variable pour stocker la difficulté choisie
    float blackCellProbability = 0.0f; // Probabilité de cases noires
    // Variable pour stocker le nom de la difficulté choisie
    std::string selectedDifficultyName;
    // Variables pour gérer l'affichage des options de thème et de difficulté
    bool showThemeOptions = false;
    bool showDifficultyOptions = false;

    sf::RectangleShape themeOptionsBackground(sf::Vector2f(300, 150));
    themeOptionsBackground.setFillColor(sf::Color(70, 130, 180, 200)); // Fond semi-transparent
    themeOptionsBackground.setPosition(
        static_cast<float>(windowWidth) / 2 - themeOptionsBackground.getSize().x / 2,
        250
    );
    sf::RectangleShape difficultyOptionsBackground(sf::Vector2f(300, 150));
    difficultyOptionsBackground.setFillColor(sf::Color(70, 130, 180, 200)); // Fond semi-transparent
    difficultyOptionsBackground.setPosition(
        static_cast<float>(windowWidth) / 2 - difficultyOptionsBackground.getSize().x / 2,
        350
    );
    sf::Text fruitsText;
    fruitsText.setFont(font);
    fruitsText.setString("Fruits");
    fruitsText.setCharacterSize(24);
    fruitsText.setFillColor(sf::Color::White);
    fruitsText.setPosition(
        themeOptionsBackground.getPosition().x + 50,
        themeOptionsBackground.getPosition().y + 0
    );
    sf::Text paysText;
    paysText.setFont(font);
    paysText.setString("Pays");
    paysText.setCharacterSize(24);
    paysText.setFillColor(sf::Color::White);
    paysText.setPosition(
        themeOptionsBackground.getPosition().x + 50,
        themeOptionsBackground.getPosition().y + 40
    );
    sf::Text prenomsText;
    prenomsText.setFont(font);
    prenomsText.setString("Prénoms");
    prenomsText.setCharacterSize(24);
    prenomsText.setFillColor(sf::Color::White);
    prenomsText.setPosition(
        themeOptionsBackground.getPosition().x + 50,
        themeOptionsBackground.getPosition().y + 80
    );
    sf::Text easyText;
    easyText.setFont(font);
    easyText.setString("Facile");
    easyText.setCharacterSize(24);
    easyText.setFillColor(sf::Color::White);
    easyText.setPosition(
        difficultyOptionsBackground.getPosition().x + 50,
        difficultyOptionsBackground.getPosition().y + 20
    );
    sf::Text mediumText;
    mediumText.setFont(font);
    mediumText.setString("Moyen");
    mediumText.setCharacterSize(24);
    mediumText.setFillColor(sf::Color::White);
    mediumText.setPosition(
        difficultyOptionsBackground.getPosition().x + 50,
        difficultyOptionsBackground.getPosition().y + 60
    );
    sf::Text hardText;
    hardText.setFont(font);
    hardText.setString("Difficile");
    hardText.setCharacterSize(24);
    hardText.setFillColor(sf::Color::White);
    hardText.setPosition(
        difficultyOptionsBackground.getPosition().x + 50,
        difficultyOptionsBackground.getPosition().y + 100
    );
    // Texte d'erreur
    sf::Text errorText;
    errorText.setFont(font);
    errorText.setString("Veuillez choisir un thème et une difficulté !");
    errorText.setCharacterSize(24);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(
        static_cast<float>(windowWidth) / 2 - errorText.getLocalBounds().width / 2,
        500
    );
    bool showError = false;

    int score = 0; // Score initial
    sf::Clock gameClock;
    bool gameStarted = false;
    std::unordered_set<std::string> foundWords; // Liste des mots déjà trouvés
    std::vector<Bubble> bubbles = createBubbles(50, window.getSize()); // Créer les bulles
    sf::Clock deltaClock; // Minuteur pour le deltaTime

    // Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Gestion des clics sur les boutons
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Bouton "Choisir un thème"
                if (themeButton.getGlobalBounds().contains(mousePos)) {
                    showThemeOptions = !showThemeOptions; // Afficher ou masquer les options de thème
                    showDifficultyOptions = false; // Masquer les options de difficulté
                }

                // Bouton "Choisir la difficulté"
                if (difficultyButton.getGlobalBounds().contains(mousePos)) {
                    showDifficultyOptions = !showDifficultyOptions; // Afficher ou masquer les options de difficulté
                    showThemeOptions = false; // Masquer les options de thème
                }

                // Sélection d'un thème
                if (showThemeOptions) {
                    if (fruitsText.getGlobalBounds().contains(mousePos)) {
                        selectedTheme = fruits;
                        selectedThemeName = "Fruits";
                        showThemeOptions = false;
                    }
                    else if (paysText.getGlobalBounds().contains(mousePos)) {
                        selectedTheme = pays;
                        selectedThemeName = "Pays";
                        showThemeOptions = false;
                    }
                    else if (prenomsText.getGlobalBounds().contains(mousePos)) {
                        selectedTheme = prenoms;
                        selectedThemeName = "Prénoms";
                        showThemeOptions = false;
                    }

                    // Mettre à jour le texte du bouton "Choisir un thème"
                    if (!selectedThemeName.empty()) {
                        themeButtonText.setString("Thème : " + selectedThemeName);
                        themeButtonText.setPosition(
                            themeButton.getPosition().x + (themeButton.getSize().x - themeButtonText.getLocalBounds().width) / 2,
                            themeButton.getPosition().y + 10
                        );
                    }
                }

                // Sélection d'une difficulté + ajouter le nombre des mot variable global
                if (showDifficultyOptions) {
                    if (easyText.getGlobalBounds().contains(mousePos)) {
                        cellSize = 30.0f;
                        gridSize = 14;
                        selectedDifficultyName = "Facile";
                        nbWords = 4;
                        showDifficultyOptions = false;
                    }
                    else if (mediumText.getGlobalBounds().contains(mousePos)) {
                        cellSize = 24.0f;
                        gridSize = 19;
                        selectedDifficultyName = "Moyen";
                        nbWords = 6;
                        showDifficultyOptions = false;
                    }
                    else if (hardText.getGlobalBounds().contains(mousePos)) {
                        cellSize = 20.0f;
                        gridSize = 24;
                        selectedDifficultyName = "Difficile";
                        nbWords = 8;
                        showDifficultyOptions = false;
                    }

                    // Mettre à jour le texte du bouton "Choisir la difficulté"
                    if (!selectedDifficultyName.empty()) {
                        difficultyButtonText.setString("Difficulté : " + selectedDifficultyName);
                        difficultyButtonText.setPosition(
                            difficultyButton.getPosition().x + (difficultyButton.getSize().x - difficultyButtonText.getLocalBounds().width) / 2,
                            difficultyButton.getPosition().y + 10
                        );
                    }
                }

                // Bouton "Commencer"
                if (startButton.getGlobalBounds().contains(mousePos)) {
                    if (!selectedTheme.empty() && gridSize > 0) {
                        gameClock.restart(); // Réinitialiser le minuteur
                        gameStarted = true; // Démarrer le jeu
                        foundWords.clear(); // Réinitialiser la liste des mots trouvés
                        score = 0; // Réinitialiser le score

                        Grid grid(gridSize, gridSize);
                        grid.generateMazeWithMultiplePaths(); // Génère le labyrinthe
                      
                       
                        // grid.fillWithTheme(selectedTheme, 0.0f); // Pas de cases noires pour l'instant
                        //ajouter avec le nombre des mots a generer
                        grid.generateContinuousPath(selectedTheme, nbWords);
                        showMatrixWindow(window, grid, font, score, selectedTheme, gameClock, foundWords, cellSize, nbWords);
                        showError = false; // Réinitialiser l'erreur
                    }
                    else {
                        showError = true; // Afficher l'erreur
                    }
                }
            }

            // Gestion du hover effect
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Hover sur le bouton "Choisir un thème"
                themeButton.setFillColor(themeButton.getGlobalBounds().contains(mousePos) ? sf::Color(100, 150, 200) : sf::Color(70, 130, 180));

                // Hover sur le bouton "Choisir la difficulté"
                difficultyButton.setFillColor(difficultyButton.getGlobalBounds().contains(mousePos) ? sf::Color(100, 150, 200) : sf::Color(70, 130, 180));

                // Hover sur le bouton "Commencer"
                startButton.setFillColor(startButton.getGlobalBounds().contains(mousePos) ? sf::Color(100, 150, 200) : sf::Color(70, 130, 180));

                // Hover sur les options de thème
                if (showThemeOptions) {
                    fruitsText.setFillColor(fruitsText.getGlobalBounds().contains(mousePos) ? sf::Color(200, 200, 200) : sf::Color::White);
                    paysText.setFillColor(paysText.getGlobalBounds().contains(mousePos) ? sf::Color(200, 200, 200) : sf::Color::White);
                    prenomsText.setFillColor(prenomsText.getGlobalBounds().contains(mousePos) ? sf::Color(200, 200, 200) : sf::Color::White);
                }

                // Hover sur les options de difficulté
                if (showDifficultyOptions) {
                    easyText.setFillColor(easyText.getGlobalBounds().contains(mousePos) ? sf::Color(200, 200, 200) : sf::Color::White);
                    mediumText.setFillColor(mediumText.getGlobalBounds().contains(mousePos) ? sf::Color(200, 200, 200) : sf::Color::White);
                    hardText.setFillColor(hardText.getGlobalBounds().contains(mousePos) ? sf::Color(200, 200, 200) : sf::Color::White);
                }
            }
        }

        // Calculer le deltaTime
        float deltaTime = deltaClock.restart().asSeconds();

        // Mettre à jour les bulles
        updateBubbles(bubbles, window.getSize(), deltaTime);

        // Effacer l'écran avec un dégradé de couleurs
        sf::VertexArray background(sf::Quads, 4);
        background[0].position = sf::Vector2f(0, 0);
        background[1].position = sf::Vector2f(static_cast<float>(windowWidth), 0);
        background[2].position = sf::Vector2f(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
        background[3].position = sf::Vector2f(0, static_cast<float>(windowHeight));
        background[0].color = backgroundColorStart;
        background[1].color = backgroundColorStart;
        background[2].color = backgroundColorEnd;
        background[3].color = backgroundColorEnd;
        window.draw(background);

        // Dessiner les bulles
        for (const auto& bubble : bubbles) {
            window.draw(bubble.shape);
        }

        // Dessiner la phrase de bienvenue
        window.draw(welcomeText);

        // Dessiner les boutons
        window.draw(themeButton);
        window.draw(themeButtonText);
        window.draw(difficultyButton);
        window.draw(difficultyButtonText);
        window.draw(startButton);
        window.draw(startButtonText);

        // Afficher les options de thème si nécessaire
        if (showThemeOptions) {
            window.draw(themeOptionsBackground);
            window.draw(fruitsText);
            window.draw(paysText);
            window.draw(prenomsText);
        }

        // Afficher les options de difficulté si nécessaire
        if (showDifficultyOptions) {
            window.draw(difficultyOptionsBackground);
            window.draw(easyText);
            window.draw(mediumText);
            window.draw(hardText);
        }

        // Afficher l'erreur si nécessaire
        if (showError) {
            window.draw(errorText);
        }

        // Afficher à l'écran
        window.display();
    }

    return 0;
}