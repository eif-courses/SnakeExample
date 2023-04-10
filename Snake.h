//
// Created by Marius on 4/10/2023.
//

#ifndef SNAKEEXAMPLE_SNAKE_H
#define SNAKEEXAMPLE_SNAKE_H


#include <array>
#include <cstddef>

#include <SFML/Graphics.hpp>

#define FIELD_SIZE 21

struct pos {
    std::int32_t y{}, x{};
};

enum class Direction {
    kNone  = 0b0000,
    kLeft  = 0b0001,
    kRight = 0b0010,
    kUp    = 0b0100,
    kDown  = 0b1000
};

class Snake {
private:
    sf::RenderWindow window_{};

    std::size_t score_{};

    pos fruit_{};
    std::vector<pos> snake_{};

    Direction dir_{};

    bool game_over_{};

public:
    Snake();
    ~Snake();

    void Run();
    void HandleEvent(const sf::Event &e);
    void Draw();
    void Move();
    void SetTitle();
    pos CreateFruit();
};



#endif //SNAKEEXAMPLE_SNAKE_H
