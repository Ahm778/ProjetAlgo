#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <fstream>
#include <ctime>
#include <unordered_map>
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
    char letter = '\0'; // Initialisation par défaut
    bool isBlocked = false; // Initialisation par défaut
    bool isSpecial = false; // Initialisation par défaut
    std::vector<Node*> neighbors; // Liste des cases adjacentes
};

// Classe pour représenter la grille
class Grid {
private:
    int rows, cols;
    std::vector<std::vector<Node>> grid;

public:
    Grid(int r, int c) : rows(r), cols(c), grid(r, std::vector<Node>(c)) {}

    // Ajouter une arête entre deux nœuds
    void addEdge(int x1, int y1, int x2, int y2) {
        grid[x1][y1].neighbors.push_back(&grid[x2][y2]);
        grid[x2][y2].neighbors.push_back(&grid[x1][y1]);
    }

    // Obtenir un nœud spécifique
    Node* getNode(int x, int y) {
        return &grid[x][y];
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }
};

// Classe pour le dictionnaire de mots (Trie)
class Trie {
private:
    struct TrieNode {
        std::unordered_map<char, TrieNode*> children;
        bool isEndOfWord = false; // Initialisation par défaut
    };

    TrieNode* root;

public:
    Trie() : root(new TrieNode()) {}

    // Insérer un mot dans le Trie
    void insert(const std::string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = new TrieNode();
            }
            current = current->children[ch];
        }
        current->isEndOfWord = true;
    }

    // Rechercher un mot dans le Trie
    bool search(const std::string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                return false;
            }
            current = current->children[ch];
        }
        return current->isEndOfWord;
    }
};

// Fonction pour générer la grille
void generateGrid(Grid& grid, const std::vector<std::string>& dictionary) {
    srand(static_cast<unsigned int>(time(0))); // Initialisation du générateur de nombres aléatoires
    for (int i = 0; i < grid.getRows(); ++i) {
        for (int j = 0; j < grid.getCols(); ++j) {
            grid.getNode(i, j)->letter = dictionary[rand() % dictionary.size()][0];
            grid.getNode(i, j)->isBlocked = (rand() % 10 == 0); // 10% de chances d'être bloqué
            grid.getNode(i, j)->isSpecial = (rand() % 20 == 0); // 5% de chances d'être spécial
        }
    }

    // Ajouter des arêtes pour les déplacements possibles
    for (int i = 0; i < grid.getRows(); ++i) {
        for (int j = 0; j < grid.getCols(); ++j) {
            if (i > 0) grid.addEdge(i, j, i - 1, j); // Haut
            if (i < grid.getRows() - 1) grid.addEdge(i, j, i + 1, j); // Bas
            if (j > 0) grid.addEdge(i, j, i, j - 1); // Gauche
            if (j < grid.getCols() - 1) grid.addEdge(i, j, i, j + 1); // Droite
            if (i > 0 && j > 0) grid.addEdge(i, j, i - 1, j - 1); // Haut-Gauche
            if (i > 0 && j < grid.getCols() - 1) grid.addEdge(i, j, i - 1, j + 1); // Haut-Droite
            if (i < grid.getRows() - 1 && j > 0) grid.addEdge(i, j, i + 1, j - 1); // Bas-Gauche
            if (i < grid.getRows() - 1 && j < grid.getCols() - 1) grid.addEdge(i, j, i + 1, j + 1); // Bas-Droite
        }
    }
}

// Fonction pour calculer le score
int calculateScore(const std::string& word, int pathLength) {
    int score = word.length(); // 1 point par lettre
    if (pathLength < 5) { // Bonus pour les chemins courts
        score += 10 - pathLength;
    }
    return score;
}

int main() {
    const int gridSize = 10;               // Taille de la grille (10x10 pour cet exemple)
    const unsigned int cellSize = 50;     // Taille de chaque case
    const unsigned int windowWidth = 1000; // Largeur de la fenêtre
    const unsigned int windowHeight = 700;
    const unsigned int margin = 50;       // Marge pour centrer la grille

    // Créer la fenêtre
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Labyrinthe de Mots");

    // Charger une police pour afficher les lettres
    sf::Font font;
    if (!font.loadFromFile("Roboto.ttf")) {
        std::cerr << "Erreur: Impossible de charger la police Roboto.ttf" << std::endl;
        return -1;
    }

    // Couleurs
    sf::Color backgroundColorStart = sf::Color(100, 150, 200); // Début du dégradé
    sf::Color backgroundColorEnd = sf::Color(200, 100, 150);   // Fin du dégradé
    sf::Color cellColor = sf::Color(240, 248, 255);            // Couleur des cases (blanc cassé)
    sf::Color blackCellColor = sf::Color::Black;               // Couleur des cases noires
    sf::Color outlineColor = sf::Color(25, 25, 112);           // Bleu foncé pour les contours des cases
    sf::Color buttonColor = sf::Color(70, 130, 180);           // Couleur des boutons
    sf::Color buttonHoverColor = sf::Color(100, 160, 210);     // Couleur des boutons au survol
    sf::Color buttonTextColor = sf::Color::White;              // Couleur du texte des boutons
    sf::Color selectedCellColor = sf::Color(255, 215, 0);      // Couleur des cases sélectionnées
    sf::Color validWordColor = sf::Color::Green;               // Couleur pour les mots valides

    std::vector<sf::RectangleShape> cells; // Cases de la grille
    std::vector<bool> isBlack(gridSize * gridSize, false); // Indique si une case est noire
    std::vector<bool> isSelected(gridSize * gridSize, false); // Indique si une case est sélectionnée

    // Création des cases
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            sf::RectangleShape cell(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
            cell.setFillColor(cellColor);               // Couleur de remplissage
            cell.setOutlineThickness(2.f);              // Bordure de la case
            cell.setOutlineColor(outlineColor);         // Couleur de la bordure
            cell.setPosition(
                static_cast<float>(margin + x * cellSize),
                static_cast<float>(margin + y * cellSize)
            );
            cells.push_back(cell);
        }
    }

    // Bouton "Commencer" (à droite)
    sf::RectangleShape startButton(sf::Vector2f(150, 50));
    startButton.setFillColor(buttonColor);
    startButton.setPosition(windowWidth - 200, 50);
    startButton.setOutlineThickness(2.f);
    startButton.setOutlineColor(sf::Color::White);
    sf::Text startButtonText;
    startButtonText.setFont(font);
    startButtonText.setString("Commencer");
    startButtonText.setCharacterSize(20);
    startButtonText.setFillColor(buttonTextColor);
    startButtonText.setPosition(windowWidth - 180, 60);

    // Bouton "Difficulté" (à droite)
    sf::RectangleShape difficultyButton(sf::Vector2f(150, 50));
    difficultyButton.setFillColor(buttonColor);
    difficultyButton.setPosition(windowWidth - 200, 120);
    difficultyButton.setOutlineThickness(2.f);
    difficultyButton.setOutlineColor(sf::Color::White);
    sf::Text difficultyButtonText;
    difficultyButtonText.setFont(font);
    difficultyButtonText.setString("Difficulté");
    difficultyButtonText.setCharacterSize(20);
    difficultyButtonText.setFillColor(buttonTextColor);
    difficultyButtonText.setPosition(windowWidth - 180, 130);

    // Bouton "Thème" (à droite)
    sf::RectangleShape themeButton(sf::Vector2f(150, 50));
    themeButton.setFillColor(buttonColor);
    themeButton.setPosition(windowWidth - 200, 190);
    themeButton.setOutlineThickness(2.f);
    themeButton.setOutlineColor(sf::Color::White);
    sf::Text themeButtonText;
    themeButtonText.setFont(font);
    themeButtonText.setString("Thème");
    themeButtonText.setCharacterSize(20);
    themeButtonText.setFillColor(buttonTextColor);
    themeButtonText.setPosition(windowWidth - 180, 200);

    // Minuteur (en haut à droite)
    sf::Clock timer;
    bool timerStarted = false;
    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::Black);
    timerText.setPosition(windowWidth - 150, 10);
    timerText.setString("Temps: 0s");

    // Bouton "Valider" (en bas)
    sf::RectangleShape validateButton(sf::Vector2f(150, 50));
    validateButton.setFillColor(buttonColor);
    validateButton.setPosition(windowWidth - 200, windowHeight - 100);
    validateButton.setOutlineThickness(2.f);
    validateButton.setOutlineColor(sf::Color::White);
    sf::Text validateButtonText;
    validateButtonText.setFont(font);
    validateButtonText.setString("Valider");
    validateButtonText.setCharacterSize(20);
    validateButtonText.setFillColor(buttonTextColor);
    validateButtonText.setPosition(windowWidth - 180, windowHeight - 90);

    // Score (en bas)
    int score = 0;
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(windowWidth - 200, windowHeight - 150);
    scoreText.setString("Score: 0");

    // Zone de texte pour afficher les lettres sélectionnées
    sf::Text selectedLettersText;
    selectedLettersText.setFont(font);
    selectedLettersText.setCharacterSize(24);
    selectedLettersText.setFillColor(sf::Color::Black);
    selectedLettersText.setPosition(50, windowHeight - 100);
    selectedLettersText.setString("Lettres sélectionnées: ");

    // Charger le dictionnaire de mots
    std::vector<std::string> dictionary;
    std::ifstream dictFile("dictionary.txt");
    if (!dictFile) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier dictionary.txt\n";
        return -1;
    }

    std::string word;
    while (std::getline(dictFile, word)) {
        dictionary.push_back(word);
    }
    std::cout << "Dictionnaire chargé avec " << dictionary.size() << " mots.\n";

    // Initialiser le Trie avec le dictionnaire
    Trie trie;
    for (const std::string& w : dictionary) {
        trie.insert(w);
    }
    std::cout << "Trie initialisé.\n";

    // Créer la grille
    Grid grid(gridSize, gridSize);
    generateGrid(grid, dictionary);
    std::cout << "Grille générée.\n";

    // Variables pour la sélection de lettres
    std::string currentWord;
    std::vector<size_t> selectedIndices;
    size_t startIndex = 0; // Index de la première case sélectionnée
    size_t endIndex = 0;   // Index de la dernière case sélectionnée

    // Boucle principale du programme
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) // Fermer la fenêtre si l'utilisateur clique sur le bouton "Fermer"
                window.close();

            // Gestion des clics sur les boutons
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Bouton "Commencer"
                if (startButton.getGlobalBounds().contains(mousePos)) {
                    timerStarted = true;
                    timer.restart();
                }

                // Bouton "Difficulté"
                if (difficultyButton.getGlobalBounds().contains(mousePos)) {
                    // Logique pour changer la difficulté
                    std::cout << "Changer la difficulté\n";
                }

                // Bouton "Thème"
                if (themeButton.getGlobalBounds().contains(mousePos)) {
                    // Logique pour changer le thème
                    std::cout << "Changer le thème\n";
                }

                // Bouton "Valider"
                if (validateButton.getGlobalBounds().contains(mousePos)) {
                    if (currentWord.length() >= 3 && trie.search(currentWord)) {
                        score += calculateScore(currentWord, selectedIndices.size());
                        scoreText.setString("Score: " + std::to_string(score));

                        // Marquer les cases sélectionnées en vert
                        for (size_t i : selectedIndices) {
                            cells[i].setFillColor(validWordColor);
                        }

                        // Réinitialiser les variables pour le prochain mot
                        currentWord.clear();
                        selectedIndices.clear();
                        selectedLettersText.setString("Lettres sélectionnées: ");
                    }
                    else {
                        // Si le mot n'est pas valide, réinitialiser la sélection
                        for (size_t i : selectedIndices) {
                            isSelected[i] = false;
                            cells[i].setFillColor(cellColor);
                        }
                        currentWord.clear();
                        selectedIndices.clear();
                        selectedLettersText.setString("Lettres sélectionnées: ");
                    }
                }

                // Sélection des cases de la grille
                for (size_t i = 0; i < cells.size(); ++i) {
                    if (cells[i].getGlobalBounds().contains(mousePos) && !isBlack[i]) {
                        if (selectedIndices.empty()) {
                            startIndex = i; // Marquer la première case sélectionnée comme début
                        }
                        endIndex = i; // Marquer la dernière case sélectionnée comme fin

                        isSelected[i] = !isSelected[i]; // Basculer la sélection
                        if (isSelected[i]) {
                            currentWord += grid.getNode(i / gridSize, i % gridSize)->letter;
                            selectedIndices.push_back(i);
                        }
                        else {
                            currentWord.pop_back();
                            selectedIndices.pop_back();
                        }
                        cells[i].setFillColor(isSelected[i] ? selectedCellColor : cellColor);
                        selectedLettersText.setString("Lettres sélectionnées: " + currentWord);
                    }
                }
            }

            // Effet de survol sur les boutons
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Bouton "Commencer"
                if (startButton.getGlobalBounds().contains(mousePos)) {
                    startButton.setFillColor(buttonHoverColor);
                }
                else {
                    startButton.setFillColor(buttonColor);
                }

                // Bouton "Difficulté"
                if (difficultyButton.getGlobalBounds().contains(mousePos)) {
                    difficultyButton.setFillColor(buttonHoverColor);
                }
                else {
                    difficultyButton.setFillColor(buttonColor);
                }

                // Bout                // Bouton "Thème"
                if (themeButton.getGlobalBounds().contains(mousePos)) {
                    themeButton.setFillColor(buttonHoverColor);
                }
                else {
                    themeButton.setFillColor(buttonColor);
                }

                // Bouton "Valider"
                if (validateButton.getGlobalBounds().contains(mousePos)) {
                    validateButton.setFillColor(buttonHoverColor);
                }
                else {
                    validateButton.setFillColor(buttonColor);
                }
            }
        }

        // Effacer l'écran avec un dégradé de couleurs
        sf::VertexArray background(sf::Quads, 4);
        background[0].position = sf::Vector2f(0, 0);
        background[1].position = sf::Vector2f(windowWidth, 0);
        background[2].position = sf::Vector2f(windowWidth, windowHeight);
        background[3].position = sf::Vector2f(0, windowHeight);
        background[0].color = backgroundColorStart;
        background[1].color = backgroundColorStart;
        background[2].color = backgroundColorEnd;
        background[3].color = backgroundColorEnd;
        window.draw(background);

        // Dessiner les cases
        for (size_t i = 0; i < cells.size(); ++i) {
            window.draw(cells[i]);

            // Afficher la lettre dans la case
            if (!isBlack[i]) {
                sf::Text letterText;
                letterText.setFont(font);
                letterText.setString(grid.getNode(i / gridSize, i % gridSize)->letter);
                letterText.setCharacterSize(24);
                letterText.setFillColor(sf::Color::Black);
                letterText.setPosition(cells[i].getPosition().x + 15, cells[i].getPosition().y + 10);
                window.draw(letterText);
            }
        }

        // Dessiner les boutons à droite
        window.draw(startButton);
        window.draw(startButtonText);
        window.draw(difficultyButton);
        window.draw(difficultyButtonText);
        window.draw(themeButton);
        window.draw(themeButtonText);

        // Dessiner le minuteur en haut à droite
        if (timerStarted) {
            int elapsedTime = static_cast<int>(timer.getElapsedTime().asSeconds());
            timerText.setString("Temps: " + std::to_string(elapsedTime) + "s");
        }
        window.draw(timerText);

        // Dessiner le bouton "Valider" et le score en bas
        window.draw(validateButton);
        window.draw(validateButtonText);
        window.draw(scoreText);

        // Afficher les lettres sélectionnées
        window.draw(selectedLettersText);

        // Afficher à l'écran
        window.display();
    }

    return 0;
}