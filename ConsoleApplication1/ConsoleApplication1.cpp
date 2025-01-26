#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <stack>
#include <queue>

// Fonction pour générer un nombre aléatoire entre min et max
int randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// Structure pour représenter une case dans la grille
struct Node {
    char letter = '\0'; // Lettre de la case
    bool isBlack = false; // Si la case est noire (lettre inutile)
    bool isSelected = false; // Si la case est sélectionnée
    bool isStart = false; // Si la case est le début d'un mot
    bool isEnd = false; // Si la case est la fin d'un mot
    bool isHint = false; // Si la case est un indice
};

// Classe pour représenter la grille
class Grid {
private:
    int rows, cols;
    std::vector<std::vector<Node>> grid;
    std::vector<sf::Vector2i> validPath; // Stocke le chemin valide pour les indices

public:
    Grid(int r, int c) : rows(r), cols(c), grid(r, std::vector<Node>(c)) {}

    // Obtenir un nœud spécifique
    Node* getNode(int x, int y) {
        return &grid[x][y];
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }

    // Remplir la grille avec des lettres aléatoires
    void fillRandom() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                grid[i][j].letter = 'A' + randomInt(0, 25); // Lettre aléatoire
            }
        }
    }

    // Remplir la grille avec les lettres d'un thème spécifique
    void fillWithTheme(const std::vector<std::string>& words, float blackCellProbability) {
        // Récupérer toutes les lettres des mots du thème
        std::string allLetters;
        for (const auto& word : words) {
            allLetters += word;
        }

        // Convertir en majuscules
        for (char& c : allLetters) {
            c = toupper(c);
        }

        // Mélanger les lettres pour un placement aléatoire
        std::random_shuffle(allLetters.begin(), allLetters.end());

        // Remplir la grille avec les lettres du thème
        int index = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (index < allLetters.size()) {
                    grid[i][j].letter = allLetters[index];
                    grid[i][j].isBlack = false; // Case utile
                    index++;
                }
                else {
                    // Remplir le reste avec des lettres aléatoires ou des cases noires
                    if (static_cast<float>(randomInt(0, 100)) / 100 < blackCellProbability) {
                        grid[i][j].isBlack = true; // Case noire (lettre inutile)
                    }
                    else {
                        grid[i][j].letter = 'A' + randomInt(0, 25); // Lettre aléatoire
                        grid[i][j].isBlack = false; // Case utile
                    }
                }
            }
        }
    }

    // Générer un chemin continu pour les mots du thème
    void generateContinuousPath(const std::vector<std::string>& themeWords) {
        validPath.clear(); // Réinitialiser le chemin valide

        // Concaténer les mots pour former un chemin continu
        std::string continuousPath;
        for (const auto& word : themeWords) {
            continuousPath += word;
        }

        // Vérifier si le chemin est trop long pour la grille
        if (continuousPath.length() > rows * cols) {
            std::cerr << "Erreur: Le chemin est trop long pour la grille." << std::endl;
            return;
        }

        // Limite de tentatives pour éviter une boucle infinie
        int maxAttempts = 100;
        int attempts = 0;

        while (attempts < maxAttempts) {
            // Choisir une position de départ aléatoire
            int startX = randomInt(0, rows - 1);
            int startY = randomInt(0, cols - 1);
            grid[startX][startY].isStart = true;
            grid[startX][startY].letter = continuousPath[0];
            validPath.push_back(sf::Vector2i(startX, startY));

            // Utiliser DFS pour générer un chemin continu
            if (dfsContinuousPath(startX, startY, continuousPath, 1)) {
                return; // Chemin trouvé
            }

            // Réinitialiser la grille et réessayer
            for (auto& row : grid) {
                for (auto& node : row) {
                    node.isStart = false;
                    node.isEnd = false;
                }
            }
            validPath.clear();
            attempts++;
        }

        std::cerr << "Erreur: Impossible de générer un chemin valide après " << maxAttempts << " tentatives." << std::endl;
    }

    // Algorithme DFS pour générer un chemin continu
    bool dfsContinuousPath(int x, int y, const std::string& path, int index) {
        if (index >= path.length()) {
            grid[x][y].isEnd = true;
            return true;
        }

        // Liste des directions possibles (8 directions)
        std::vector<std::pair<int, int>> directions = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},          {0, 1},
            {1, -1},  {1, 0}, {1, 1}
        };

        // Mélanger les directions pour un choix aléatoire
        std::random_shuffle(directions.begin(), directions.end());

        for (const auto& dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;

            // Vérifier si la nouvelle position est valide (dans les limites de la grille)
            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols &&
                !grid[newX][newY].isBlack && !grid[newX][newY].isStart && !grid[newX][newY].isEnd) {
                grid[newX][newY].letter = path[index];
                validPath.push_back(sf::Vector2i(newX, newY));
                if (dfsContinuousPath(newX, newY, path, index + 1)) {
                    return true;
                }
                validPath.pop_back(); // Backtracking
                grid[newX][newY].letter = '\0'; // Réinitialiser la case
            }
        }

        return false;
    }

    // Obtenir le chemin valide pour les indices
    const std::vector<sf::Vector2i>& getValidPath() const {
        return validPath;
    }
};

// Fonction pour afficher la deuxième fenêtre (matrice)
void showMatrixWindow(sf::RenderWindow& window, Grid& grid, const sf::Font& font, int& score, const std::vector<std::string>& themeWords) {
    const int gridSize = grid.getRows();
    const float cellSize = 50.0f;
    const float marginX = (window.getSize().x - gridSize * cellSize) / 2; // Centrer horizontalement
    const float marginY = (window.getSize().y - gridSize * cellSize) / 2 - 100; // Centrer verticalement

    // Création des cases
    std::vector<sf::RectangleShape> cells;
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            if (grid.getNode(x, y)->isBlack) {
                cell.setFillColor(sf::Color::Black); // Case noire
            }
            else if (grid.getNode(x, y)->isSelected) {
                cell.setFillColor(sf::Color::Yellow); // Case sélectionnée
            }
            else if (grid.getNode(x, y)->isStart) {
                cell.setFillColor(sf::Color::Green); // Case de départ en vert
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
    sf::RectangleShape backButton(sf::Vector2f(150, 50));
    backButton.setFillColor(sf::Color(70, 130, 180)); // Couleur bleue
    backButton.setPosition(20, 20); // Position en haut à gauche

    sf::Text backButtonText;
    backButtonText.setFont(font);
    backButtonText.setString("Retour");
    backButtonText.setCharacterSize(24);
    backButtonText.setFillColor(sf::Color::White);
    backButtonText.setPosition(backButton.getPosition().x + 40, backButton.getPosition().y + 10);

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

    // Variables pour gérer la sélection des lettres
    std::vector<sf::Vector2i> selectedLetters;
    std::string currentWord;

    // Variable pour gérer l'indice de la lettre suivante à afficher avec le hint
    int hintIndex = 0;

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
                        // Colorer les cases sélectionnées en vert
                        for (const auto& pos : selectedLetters) {
                            grid.getNode(pos.x, pos.y)->isSelected = false;
                            grid.getNode(pos.x, pos.y)->isHint = true; // Marquer comme indice
                        }
                        selectedLetters.clear();
                        currentWord.clear();
                        score += 10; // Augmenter le score
                        scoreText.setString("Score: " + std::to_string(score));
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
        for (size_t i = 0; i < cells.size(); ++i) {
            int x = static_cast<int>(i) % gridSize;
            int y = static_cast<int>(i) / gridSize;
            if (grid.getNode(x, y)->isSelected) {
                cells[i].setFillColor(sf::Color::Yellow); // Case sélectionnée
            }
            else if (grid.getNode(x, y)->isBlack) {
                cells[i].setFillColor(sf::Color::Black); // Case noire
            }
            else if (grid.getNode(x, y)->isStart) {
                cells[i].setFillColor(sf::Color::Green); // Case de départ en vert
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
                letterText.setCharacterSize(24);
                letterText.setFillColor(sf::Color::Black);
                letterText.setPosition(cells[i].getPosition().x + 15.0f, cells[i].getPosition().y + 10.0f);
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
    themeTitle.setString("Mots du thème:");
    window.draw(themeTitle);

    for (size_t i = 0; i < words.size(); ++i) {
        sf::Text wordText;
        wordText.setFont(font);
        wordText.setCharacterSize(20);
        wordText.setFillColor(sf::Color::White);
        wordText.setPosition(x, y + 30 + i * 25);
        wordText.setString(words[i]);
        window.draw(wordText);
    }
}

int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    // Créer la fenêtre
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

    // Créer la grille
    Grid grid(10, 10);

    // Définir les thèmes
    std::vector<std::string> fruits = { "Pomme", "Banane", "Orange", "Fraise", "Kiwi", "Mangue" };
    std::vector<std::string> pays = { "France", "Tunis", "Qatar", "Pero", "Japon", "Canada" };
    std::vector<std::string> prenoms = { "Alice", "Bob", "Charlie", "David", "Eve", "Frank" };

    // Variable pour stocker le thème choisi
    std::vector<std::string> selectedTheme;

    // Variable pour stocker le nom du thème choisi
    std::string selectedThemeName;

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

    // Score initial
    int score = 0;

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
                        selectedThemeName = "Fruits"; // Mettre à jour le nom du thème
                        showThemeOptions = false;
                    }
                    else if (paysText.getGlobalBounds().contains(mousePos)) {
                        selectedTheme = pays;
                        selectedThemeName = "Pays"; // Mettre à jour le nom du thème
                        showThemeOptions = false;
                    }
                    else if (prenomsText.getGlobalBounds().contains(mousePos)) {
                        selectedTheme = prenoms;
                        selectedThemeName = "Prénoms"; // Mettre à jour le nom du thème
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
                        blackCellProbability = 0.1f; // 10% de cases noires
                        selectedDifficultyName = "Facile"; // Mettre à jour le nom de la difficulté
                        showDifficultyOptions = false;
                    }
                    else if (mediumText.getGlobalBounds().contains(mousePos)) {
                        blackCellProbability = 0.3f; // 30% de cases noires
                        selectedDifficultyName = "Moyen"; // Mettre à jour le nom de la difficulté
                        showDifficultyOptions = false;
                    }
                    else if (hardText.getGlobalBounds().contains(mousePos)) {
                        blackCellProbability = 0.5f; // 50% de cases noires
                        selectedDifficultyName = "Difficile"; // Mettre à jour le nom de la difficulté
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
                    if (!selectedTheme.empty() && blackCellProbability >= 0.0f) {
                        grid.fillWithTheme(selectedTheme, blackCellProbability); // Remplir la grille avec le thème et la difficulté choisis
                        grid.generateContinuousPath(selectedTheme); // Générer un chemin continu avec 4 mots du thème
                        showMatrixWindow(window, grid, font, score, selectedTheme); // Afficher la deuxième fenêtre (matrice)
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

        // Afficher les mots du thème si un thème est sélectionné
        if (!selectedTheme.empty()) {
            drawThemeWords(window, font, selectedTheme, 50, 500);
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