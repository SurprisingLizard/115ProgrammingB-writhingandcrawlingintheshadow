#include <SFML/Graphics.hpp>
#include <optional>
#include "Types.h"

namespace Tetris {

InputAction mapKeyToAction(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Key::Left:
        case sf::Keyboard::Key::A:
            return InputAction::MoveLeft;
        case sf::Keyboard::Key::Right:
        case sf::Keyboard::Key::D:
            return InputAction::MoveRight;
        case sf::Keyboard::Key::Down:
        case sf::Keyboard::Key::S:
            return InputAction::SoftDrop;
        case sf::Keyboard::Key::Space:
            return InputAction::HardDrop;
        case sf::Keyboard::Key::Up:
        case sf::Keyboard::Key::X:
            return InputAction::RotateCW;
        case sf::Keyboard::Key::Z:
            return InputAction::RotateCCW;
        default:
            return InputAction::MoveLeft; // 可依實際列舉調整預設值
    }
}

} // namespace Tetris

int main() {
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;

    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(WINDOW_WIDTH),
                       static_cast<unsigned int>(WINDOW_HEIGHT)}),
        "Tetris"
    );
    window.setFramerateLimit(60);

    bool isPlaying = true;
    bool isPaused = false;
    bool isGameOver = false;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (isPlaying && !isPaused && !isGameOver) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape ||
                        keyPressed->code == sf::Keyboard::Key::P) {
                        isPaused = true;
                    } else {
                        Tetris::InputAction action = Tetris::mapKeyToAction(keyPressed->code);
                        (void)action; // 交由遊戲狀態處理
                    }
                } else if (isPaused) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape ||
                        keyPressed->code == sf::Keyboard::Key::P) {
                        isPaused = false;
                    }
                } else if (isGameOver) {
                    if (keyPressed->code == sf::Keyboard::Key::R ||
                        keyPressed->code == sf::Keyboard::Key::Enter) {
                        isGameOver = false;
                        isPaused = false;
                        isPlaying = true;
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));
        window.display();
    }

    return 0;
}