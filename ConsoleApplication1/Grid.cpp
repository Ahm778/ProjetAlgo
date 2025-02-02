#include "Grid.hpp"
#include <queue>
#include <ctime>
#include <algorithm> // Pour std::shuffle

// Structure pour représenter une arête entre deux cellules
struct Edge {
    int x1, y1, x2, y2;
};

// Constructeur de la grille
Grid::Grid(int r, int c) : rows(r), cols(c), grid(r, std::vector<Node>(c)) {}

// Récupérer un nœud à la position (x, y)
Node* Grid::getNode(int x, int y) {
    return &grid[x][y];
}

// Récupérer le nombre de lignes
int Grid::getRows() const {
    return rows;
}

// Récupérer le nombre de colonnes
int Grid::getCols() const {
    return cols;
}

// Remplir la grille avec des lettres aléatoires
void Grid::fillRandom() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].letter = 'A' + randomInt(0, 25); // Lettre aléatoire
        }
    }
}

// Remplir la grille avec un thème de mots
void Grid::fillWithTheme(const std::vector<std::string>& words, float blackCellProbability) {
    std::string allLetters;
    for (const auto& word : words) {
        allLetters += word;
    }

    for (char& c : allLetters) {
        c = toupper(c);
    }
    std::shuffle(allLetters.begin(), allLetters.end(), std::mt19937(std::random_device()()));

    int index = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!grid[i][j].isBlack) { // Ne pas remplir les cases noires
                if (index < allLetters.size()) {
                    grid[i][j].letter = allLetters[index];
                    index++;
                }
                else {
                    grid[i][j].letter = 'A' + randomInt(0, 25); // Lettre aléatoire
                }
            }
        }
    }
}

// Générer un labyrinthe avec plusieurs chemins
void Grid::generateMazeWithMultiplePaths() {
    // Initialiser toutes les cellules comme des murs
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].isBlack = true;
        }
    }

    // Créer une liste de toutes les arêtes possibles
    std::vector<Edge> edges;
    for (int i = 1; i < rows - 1; i += 2) {
        for (int j = 1; j < cols - 1; j += 2) {
            if (i < rows - 2) edges.push_back({ i, j, i + 2, j }); // Arête verticale
            if (j < cols - 2) edges.push_back({ i, j, i, j + 2 }); // Arête horizontale
        }
    }

    // Mélanger les arêtes
    std::shuffle(edges.begin(), edges.end(), std::mt19937(std::random_device()()));

    // Union-Find pour gérer les ensembles de cellules
    std::vector<int> parent(rows * cols);
    for (int i = 0; i < rows * cols; ++i) {
        parent[i] = i;
    }

    auto find = [&](int x, int y) {
        int idx = x * cols + y;
        while (parent[idx] != idx) {
            parent[idx] = parent[parent[idx]]; // Compression de chemin
            idx = parent[idx];
        }
        return idx;
        };

    auto unite = [&](int x1, int y1, int x2, int y2) {
        int root1 = find(x1, y1);
        int root2 = find(x2, y2);
        if (root1 != root2) {
            parent[root2] = root1;
            return true;
        }
        return false;
        };

    // Générer le labyrinthe
    for (const auto& edge : edges) {
        int midX = (edge.x1 + edge.x2) / 2;
        int midY = (edge.y1 + edge.y2) / 2;

        if (unite(edge.x1, edge.y1, edge.x2, edge.y2)) {
            grid[edge.x1][edge.y1].isBlack = false;
            grid[edge.x2][edge.y2].isBlack = false;
            grid[midX][midY].isBlack = false;
        }
    }
}

// Sélectionner 5 mots aléatoires du thème
std::vector<std::string> Grid::selectRandomWords(const std::vector<std::string>& themeWords, int count) {
    std::vector<std::string> selectedWords = themeWords;
    std::shuffle(selectedWords.begin(), selectedWords.end(), std::mt19937(std::random_device()()));
    if (selectedWords.size() > count) {
        selectedWords.resize(count);
    }
    return selectedWords;
}

// Générer un chemin continu avec seulement 5 mots aléatoires du thème
void Grid::generateContinuousPath(const std::vector<std::string>& themeWords) {
    validPath.clear();
    generateMazeWithMultiplePaths(); // Générer un labyrinthe avec plusieurs chemins

    // Sélectionner 5 mots aléatoires du thème
    std::vector<std::string> selectedWords = selectRandomWords(themeWords, 5);

    // Construire le chemin continu avec ces 5 mots
    std::string continuousPath;
    for (const auto& word : selectedWords) {
        continuousPath += word;
    }

    if (continuousPath.length() > rows * cols) {
        std::cerr << "Erreur: Le chemin est trop long pour la grille." << std::endl;
        return;
    }

    // Trouver un chemin valide dans le labyrinthe
    int startX = 1, startY = 1; // Commencer à une position fixe
    grid[startX][startY].isStart = true;
    grid[startX][startY].letter = continuousPath[0];
    validPath.push_back(sf::Vector2i(startX, startY));

    if (!dfsContinuousPath(startX, startY, continuousPath, 1)) {
        std::cerr << "Erreur: Impossible de générer un chemin valide." << std::endl;
    }
}

// DFS pour générer un chemin continu
bool Grid::dfsContinuousPath(int x, int y, const std::string& path, int index) {
    if (index >= path.length()) {
        grid[x][y].isEnd = true;
        return true;
    }

    std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},          {0, 1},
        {1, -1},  {1, 0}, {1, 1}
    };

    // Mélanger les directions
    std::shuffle(directions.begin(), directions.end(), std::mt19937(std::random_device()()));

    for (const auto& dir : directions) {
        int newX = x + dir.first;
        int newY = y + dir.second;

        if (newX > 0 && newX < rows - 1 && newY > 0 && newY < cols - 1 &&
            !grid[newX][newY].isBlack && !grid[newX][newY].isStart && !grid[newX][newY].isEnd) {

            bool isAlreadyInPath = false;
            for (const auto& pos : validPath) {
                if (pos.x == newX && pos.y == newY) {
                    isAlreadyInPath = true;
                    break;
                }
            }

            if (!isAlreadyInPath) {
                grid[newX][newY].letter = path[index];
                validPath.push_back(sf::Vector2i(newX, newY));
                if (dfsContinuousPath(newX, newY, path, index + 1)) {
                    return true;
                }
                validPath.pop_back();
                grid[newX][newY].letter = '\0';
            }
        }
    }

    return false;
}

// Récupérer le chemin valide
const std::vector<sf::Vector2i>& Grid::getValidPath() const {
    return validPath;
}

// Générer un nombre entier aléatoire
int Grid::randomInt(int min, int max) {
    static std::mt19937 gen(static_cast<unsigned int>(std::time(0))); // Utilisez l'heure actuelle comme graine
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}