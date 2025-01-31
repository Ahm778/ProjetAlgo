#include "Game.hpp"
#include <stdexcept>

Game::Game() : window(sf::VideoMode(800, 600), "Jeu de Mots"), grid(10, 10), score(0) {
    if (!font.loadFromFile("Roboto.ttf")) {
        throw std::runtime_error("Failed to load font");
    }
    bubbles = createBubbles(50, window.getSize());
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
    }
}

void Game::update() {
    float deltaTime = gameClock.restart().asSeconds();
    for (auto& bubble : bubbles) bubble.update(deltaTime, window.getSize());
}

void Game::render() {
    window.clear();
    for (const auto& bubble : bubbles) window.draw(bubble.shape);
    window.display();
}

std::vector<Bubble> Game::createBubbles(int count, const sf::Vector2u& windowSize) {
    std::vector<Bubble> bubbles;
    for (int i = 0; i < count; ++i) {
        float radius = static_cast<float>(randomInt(10, 50));
        sf::Vector2f position(static_cast<float>(randomInt(0, windowSize.x)), static_cast<float>(randomInt(0, windowSize.y)));
        float speedX = static_cast<float>(randomInt(-50, 50)) / 100.0f;
        float speedY = static_cast<float>(randomInt(-50, 50)) / 100.0f;
        float scaleSpeed = static_cast<float>(randomInt(1, 10)) / 1000.0f;
        bubbles.emplace_back(radius, position, speedX, speedY, scaleSpeed);
    }
    return bubbles;
}

int Game::randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}