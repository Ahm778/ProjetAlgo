#include "CongratulationWindow.hpp"

CongratulationWindow::CongratulationWindow(const sf::Font& font, int score, int timeElapsed)
    : window(sf::VideoMode(800, 600), "Félicitations !"), font(font), score(score), timeElapsed(timeElapsed) {}

void CongratulationWindow::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        render();
    }
}

void CongratulationWindow::render() {
    window.clear();
    // Dessiner les textes et les boutons...
    window.display();
}
