#ifndef GAME_HPP
#define GAME_HPP
#

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <vector>
#include "Grid.hpp"
#include "Bulle.hpp"

class Game {
private:
    sf::RenderWindow window;
    sf::Font font;
    Grid grid;
    std::vector<Bubble> bubbles;
    int score;
    sf::Clock gameClock;
    std::unordered_set<std::string> foundWords;

public:
    Game();
    void run();

private:
    void handleEvents();
    void update();
    void render();
    std::vector<Bubble> createBubbles(int count, const sf::Vector2u& windowSize);
    int randomInt(int min, int max);
};

#endif // GAME_HPP