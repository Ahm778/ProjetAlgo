#include "Grid.hpp"
#include "Bubbles.hpp" 

Grid::Grid(int r, int c) : rows(r), cols(c), grid(r, std::vector<Node>(c)) {}

Node* Grid::getNode(int x, int y) {
    return &grid[x][y];
}

int Grid::getRows() const {
    return rows;
}

int Grid::getCols() const {
    return cols;
}

void Grid::fillRandom() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].letter = 'A' + randomInt(0, 25); // Lettre aléatoire
        }
    }
}

void Grid::fillWithTheme(const std::vector<std::string>& words, float blackCellProbability) {
    std::string allLetters;
    for (const auto& word : words) {
        allLetters += word;
    }

    for (char& c : allLetters) {
        c = toupper(c);
    }
    std::random_shuffle(allLetters.begin(), allLetters.end());

    int index = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (index < allLetters.size()) {
                grid[i][j].letter = allLetters[index];
                grid[i][j].isBlack = false; // Case utile
                index++;
            }
            else {
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

void Grid::generateContinuousPath(const std::vector<std::string>& themeWords) {
    validPath.clear();
    std::string continuousPath;
    for (const auto& word : themeWords) {
        continuousPath += word;
    }

    if (continuousPath.length() > rows * cols) {
        std::cerr << "Erreur: Le chemin est trop long pour la grille." << std::endl;
        return;
    }

    int maxAttempts = 100;
    int attempts = 0;
    while (attempts < maxAttempts) {
        int startX = randomInt(0, rows - 1);
        int startY = randomInt(0, cols - 1);
        grid[startX][startY].isStart = true;
        grid[startX][startY].letter = continuousPath[0];
        validPath.push_back(sf::Vector2i(startX, startY));

        if (dfsContinuousPath(startX, startY, continuousPath, 1)) {
            return;
        }

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

    std::random_shuffle(directions.begin(), directions.end());

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

const std::vector<sf::Vector2i>& Grid::getValidPath() const {
    return validPath;
}

int Grid::randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}
