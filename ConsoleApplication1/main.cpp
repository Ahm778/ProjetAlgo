#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <stack>
#include <queue>
#include "Grid.cpp"
#include "Bubbles.hpp"

// Fonction pour afficher la fenêtre de félicitations
void showCongratulationWindow(sf::RenderWindow& window, const sf::Font& font, int score, int timeElapsed) {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;
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
void showMatrixWindow(sf::RenderWindow& window, Grid& grid, const sf::Font& font, int& score, const std::vector<std::string>& themeWords, sf::Clock& gameClock, std::unordered_set<std::string>& foundWords, float cellSize) {
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
                        hintIndex++;
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
                cells[i].setFillColor(sf::Color::Green); // Case indice (vert)
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
                letterText.setPosition(cells[i].getPosition().x + 6.0f, cells[i].getPosition().y + 0.0f);
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
        if (foundWords.size() == 5) { //if (foundWords.size() == themeWords.size()) {
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
int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 720;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Jeu de Mots");

    // Charger une police
    sf::Font font;
    if (!font.loadFromFile("Roboto.ttf")) {
        std::cerr << "Erreur: Impossible de charger la police Roboto.ttf" << std::endl;
        return -1;
    }

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
        250 // Position verticale
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
        450 // Position verticale
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
    // Définir les thèmes
    std::vector<std::string> fruits = { "Pomme", "Banane", "Orange", "Fraise", "Kiwi", "Mangue", "bsal", "besbes", "khorchof", "bousaa" };
    std::vector<std::string> pays = { "France", "Tunis", "Qatar", "Pero", "Japon", "Canada", "Djibouti", "Cuba" };
    std::vector<std::string> prenoms = { "Alice", "Bob", "Charlie", "David", "Eve", "Frank", "ahmed", "zeineb", "asma", "saif" };
   
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
        themeOptionsBackground.getPosition().y + 20
    );
    sf::Text paysText;
    paysText.setFont(font);
    paysText.setString("Pays");
    paysText.setCharacterSize(24);
    paysText.setFillColor(sf::Color::White);
    paysText.setPosition(
        themeOptionsBackground.getPosition().x + 50,
        themeOptionsBackground.getPosition().y + 60
    );
    sf::Text prenomsText;
    prenomsText.setFont(font);
    prenomsText.setString("Prénoms");
    prenomsText.setCharacterSize(24);
    prenomsText.setFillColor(sf::Color::White);
    prenomsText.setPosition(
        themeOptionsBackground.getPosition().x + 50,
        themeOptionsBackground.getPosition().y + 100
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

                // Sélection d'une difficulté
                if (showDifficultyOptions) {
                    if (easyText.getGlobalBounds().contains(mousePos)) {
                        cellSize = 40.0f;
                        gridSize = 12;
                        selectedDifficultyName = "Facile";
                        showDifficultyOptions = false;
                    }
                    else if (mediumText.getGlobalBounds().contains(mousePos)) {
                        cellSize = 30.0f;
                        gridSize = 15;
                        selectedDifficultyName = "Moyen";
                        showDifficultyOptions = false;
                    }
                    else if (hardText.getGlobalBounds().contains(mousePos)) {
                        cellSize = 25.0f;
                        gridSize = 18;
                        selectedDifficultyName = "Difficile";
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
                        grid.fillWithTheme(selectedTheme, 0.0f); // Pas de cases noires pour l'instant
                        grid.generateContinuousPath(selectedTheme);
                        showMatrixWindow(window, grid, font, score, selectedTheme, gameClock, foundWords, cellSize);
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