#ifndef CONGRATULATIONWINDOW_HPP
#define CONGRATULATIONWINDOW_HPP

#include <SFML/Graphics.hpp>

class CongratulationWindow {
private:
    sf::RenderWindow window;
    sf::Font font;
    int score;
    int timeElapsed;

public:
    CongratulationWindow(const sf::Font& font, int score, int timeElapsed);
    void run();

private:
    void render();
};

#endif // CONGRATULATIONWINDOW_HPP
