#include "Grid.hpp"
#include <queue>
#include <ctime>
#include <algorithm> // Pour std::shuffle

// Constructeur de la grille
Grid::Grid(int r, int c) : rows(r), cols(c), grid(r, std::vector<Node>(c)) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].letter = 'A' + randomInt(0, 25); // Remplir chaque case avec une lettre al�atoire
            grid[i][j].isBlack = false; // Par d�faut, la case n'est pas noire
            grid[i][j].isSelected = false;
            grid[i][j].isStart = false;
            grid[i][j].isEnd = false;
            grid[i][j].isHint = false;
        }
    }
}

// R�cup�rer un n�ud � la position (x, y)
Node* Grid::getNode(int x, int y) {
    return &grid[x][y];
}

// R�cup�rer le nombre de lignes
int Grid::getRows() const {
    return rows;
}

// R�cup�rer le nombre de colonnes
int Grid::getCols() const {
    return cols;
}

// Remplir la grille avec des lettres al�atoires
void Grid::fillRandom() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!grid[i][j].isBlack) { // Ne pas remplir les cases noires
                grid[i][j].letter = 'A' + randomInt(0, 25); // Lettre al�atoire
            }
            else {
                grid[i][j].letter = ' '; // Remplir les cases noires avec un espace
            }
        }
    }
}

// Remplir la grille avec un th�me de mots
void Grid::fillWithTheme(const std::vector<std::string>& words, float blackCellProbability) {
    std::string allLetters;
    for (const auto& word : words) {
        allLetters += word;
    }
    // Convertir toutes les lettres en majuscules
    for (char& c : allLetters) {
        c = toupper(c);
    }
    // M�langer les lettres
    std::shuffle(allLetters.begin(), allLetters.end(), std::mt19937(std::random_device()()));
    // Remplir la grille avec ces lettres
    int index = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!grid[i][j].isBlack) { // Ne pas remplir les cases noires
                if (index < allLetters.size()) {
                    grid[i][j].letter = allLetters[index];
                    index++;
                }
                else {
                    grid[i][j].letter = 'A' + randomInt(0, 25); // Lettre al�atoire
                }
            }
        }
    }
}

void Grid::generateMazeWithMultiplePaths() {
    // Initialiser toutes les cellules comme des murs
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].isBlack = true;
            grid[i][j].letter = ' '; // Remplir les cases noires avec un espace
        }
    }

    // Cr�er une liste de toutes les ar�tes possibles
    std::vector<Edge> edges;
    for (int i = 1; i < rows - 1; i += 2) {
        for (int j = 1; j < cols - 1; j += 2) {
            if (i < rows - 2) edges.push_back({ i, j, i + 2, j }); // Ar�te verticale
            if (j < cols - 2) edges.push_back({ i, j, i, j + 2 }); // Ar�te horizontale
        }
    }

    // M�langer les ar�tes
    std::shuffle(edges.begin(), edges.end(), std::mt19937(std::random_device()()));

    // Union-Find pour g�rer les ensembles de cellules
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

    // G�n�rer le labyrinthe
    for (const auto& edge : edges) {
        int midX = (edge.x1 + edge.x2) / 2;
        int midY = (edge.y1 + edge.y2) / 2;

        if (unite(edge.x1, edge.y1, edge.x2, edge.y2)) {
            grid[edge.x1][edge.y1].isBlack = false;
            grid[edge.x2][edge.y2].isBlack = false;
            grid[midX][midY].isBlack = false;

            // Remplir les cases ouvertes avec des lettres al�atoires
            grid[edge.x1][edge.y1].letter = 'A' + randomInt(0, 25);
            grid[edge.x2][edge.y2].letter = 'A' + randomInt(0, 25);
            grid[midX][midY].letter = 'A' + randomInt(0, 25);

        }
    }
}

// S�lectionner 5 mots al�atoires du th�me
std::vector<std::string> Grid::selectRandomWords(const std::vector<std::string>& themeWords, int count) {
    std::vector<std::string> selectedWords = themeWords;
    std::shuffle(selectedWords.begin(), selectedWords.end(), std::mt19937(std::random_device()()));
    if (selectedWords.size() > count) {
        selectedWords.resize(count);
    }
    return selectedWords;
}

// G�n�rer un chemin continu avec seulement 5 mots al�atoires du th�me
void Grid::generateContinuousPath(const std::vector<std::string>& themeWords, int nbWords) {
    validPath.clear();
    int hintIndex = 0; // R�initialiser hintIndex
    generateMazeWithMultiplePaths(); // G�n�rer un labyrinthe avec plusieurs chemins

    // S�lectionner 5 mots al�atoires du th�me
    std::vector<std::string> selectedWords = selectRandomWords(themeWords, nbWords);

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
    int startX = 1, startY = 1; // Commencer � une position fixe
    grid[startX][startY].isStart = true;
    grid[startX][startY].letter = continuousPath[0];
    validPath.push_back(sf::Vector2i(startX, startY));

    if (!dfsContinuousPath(startX, startY, continuousPath, 1)) {
        std::cerr << "Erreur: Impossible de g�n�rer un chemin valide." << std::endl;
    }
}

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

    // M�langer les directions
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
                grid[newX][newY].letter = 'A' + randomInt(0, 25); // Remplir avec une lettre al�atoire si le chemin n'est pas valide
            }
        }
    }

    return false;
}

// R�cup�rer le chemin valide
const std::vector<sf::Vector2i>& Grid::getValidPath() const {
    return validPath;
}

// G�n�rer un nombre entier al�atoire
int Grid::randomInt(int min, int max) {
    static std::mt19937 gen(static_cast<unsigned int>(std::time(0))); // Utilisez l'heure actuelle comme graine
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void Grid::display() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j].isBlack) {
                std::cout << "# "; // Afficher un caract�re sp�cial pour les cases noires
            }
            else {
                std::cout << grid[i][j].letter << " "; // Afficher la lettre
            }
        }
        std::cout << std::endl;
    }
}

// Fonction Dijkstra pour trouver le plus court chemin
std::vector<sf::Vector2i> Grid::dijkstra(int startX, int startY, int endX, int endY) {
    std::vector<std::vector<int>> distances(rows, std::vector<int>(cols, INT_MAX));
    std::vector<std::vector<sf::Vector2i>> previous(rows, std::vector<sf::Vector2i>(cols, sf::Vector2i(-1, -1)));
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, std::greater<>> pq;

    distances[startX][startY] = 0;
    pq.emplace(startX, startY, 0);

    std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},          {0, 1},
        {1, -1},  {1, 0}, {1, 1}
    };

    while (!pq.empty()) {
        DijkstraNode current = pq.top();
        pq.pop();

        if (current.x == endX && current.y == endY) {
            break;
        }

        for (const auto& dir : directions) {
            int newX = current.x + dir.first;
            int newY = current.y + dir.second;

            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && !grid[newX][newY].isBlack) {
                int newDistance = current.distance + 1;
                if (newDistance < distances[newX][newY]) {
                    distances[newX][newY] = newDistance;
                    previous[newX][newY] = sf::Vector2i(current.x, current.y);
                    pq.emplace(newX, newY, newDistance);
                }
            }
        }
    }

    std::vector<sf::Vector2i> path;
    for (sf::Vector2i at(endX, endY); at != sf::Vector2i(-1, -1); at = previous[at.x][at.y]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}