//
// Created by Marius on 4/10/2023.
//

#include "Snake.h"
#include <cmath>
#include <random>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <sysinfoapi.h>
#include <windows.h>
// XOR
// snake can't reverse direction
#define REVERSED_DIRECTION_X (0b0001 ^ 0b0010)
#define REVERSED_DIRECTION_Y (0b0100 ^ 0b1000)

#define BACKGROUND_COLOR { 25, 25, 25, 255 }

sf::Clock timer;
int startAlpha = 0;
int endAlpha = 255;
int targetTime = 3000;
sf::RectangleShape fade;
auto snake_color = sf::Color::Yellow;
std::vector<pos> collected_fruits;

sf::Music background_music;
sf::Music music;
sf::Texture animatedTexture;
sf::IntRect rectSourceSprite;
sf::Clock clockForAnimation;
sf::Sprite animatedFruit;

Snake::Snake()
        : score_{1},
          window_(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SNAKE | SCORE: 0",
                  sf::Style::Titlebar | sf::Style::Close),
          dir_{Direction::kNone},
          speed{7},
          game_over_{false} {




    window_.setFramerateLimit(speed);

    int start_pos_x{FIELD_SIZE / 2};
    int start_pos_y{FIELD_SIZE / 2};
    snake_.reserve(FIELD_SIZE * FIELD_SIZE);
    snake_.push_back({start_pos_x, start_pos_y});

    fruit_ = CreateFruit();

    SetTitle();
}

Snake::~Snake() {
    window_.close();
}

void Snake::Run() {


    rectSourceSprite = sf::IntRect(0, 0, 68, 70);
    animatedFruit = sf::Sprite(animatedTexture,rectSourceSprite);


    if (!background_music.openFromFile("background.flac")){
        std::cout << "No sound!";
    }
    background_music.play();

    if (!music.openFromFile("crunch.wav")){
        std::cout << "No sound!";
    }




    while (window_.isOpen()) {


        sf::Event event{};
        while (window_.pollEvent(event)) {
            HandleEvent(event);
        }

        if (dir_ != Direction::kNone && !game_over_) {
            Move();
        }


        Draw(dir_);

        //window_.clear();

        //window_.display();

    }
}
COLORREF GetRandomColor(void)
{
#define COLORREF_BASE		(256)
    srand(GetTickCount());
    BYTE byRed, byGreen, byBlue;

    byRed = rand() % COLORREF_BASE;
    byGreen = rand() % COLORREF_BASE;
    byBlue = rand() % COLORREF_BASE;
    return RGB(byRed, byGreen, byBlue);
}

void Snake::HandleEvent(const sf::Event &e) {
    if (e.type == sf::Event::Closed) {
        window_.close();
    } else if (e.type == sf::Event::KeyPressed) {

        if(e.key.code == sf::Keyboard::B){
            snake_color = sf::Color(GetRandomColor());
            std::cout << "BUY BUTTON PRESSED";
        }

        if (e.key.code >= 71 && e.key.code <= 74) {
            auto new_dir{static_cast<Direction>(std::pow(2, e.key.code - 71))};

            if (dir_ != Direction::kNone) {
                auto xor_val{
                        static_cast<std::int32_t>(new_dir) ^ static_cast<std::int32_t>(dir_)
                };

                if (xor_val == REVERSED_DIRECTION_X || xor_val == REVERSED_DIRECTION_Y) {
                    return;
                }
            }

            dir_ = new_dir;
        }
    }
}

void Snake::Draw(Direction &direction) {

    window_.clear(BACKGROUND_COLOR);

    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cout << "Failed";
        // error...
    }
    sf::Text text;
    sf::String score;
    text.setFont(font); // font is a sf::Font

    score = std::to_string(score_);

    text.setString("Score: " + score);
    text.setCharacterSize(48); // in pixels, not points!
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setPosition(2, 2);

    window_.draw(text);


    sf::RectangleShape rect{};
    int count = 0;
    sf::Texture textureFront;


    for (auto it: snake_) {
        rect.setSize(sf::Vector2f(SHAPE_SIZE - 1, SHAPE_SIZE - 1));
        rect.setPosition(it.x * PIXEL_SIZE + 1, it.y * PIXEL_SIZE + 1);


        if (snake_.size() == count) {
            if (!textureFront.loadFromFile("box.jpg", sf::IntRect(0, 24, 24, 24))) {
                // error...`
            }
            rect.setTexture(&textureFront);
        }
//        }else{
//            rect.setTexture(nullptr);
//            rect.setFillColor(sf::Color::Green);
//            rect.setOutlineColor(BACKGROUND_COLOR);
//            rect.setOutlineThickness(1);
//        }




        // TODO need tidy up code
        if (count == 0) {

//            rect.setFillColor(sf::Color::White);
//            rect.setOutlineColor(BACKGROUND_COLOR);
//            rect.setOutlineThickness(1);

            if (direction == Direction::kLeft) {
                std::cout << "LEFT" << std::endl;
                if (!textureFront.loadFromFile("head_sheet.png", sf::IntRect(0, 24, 24, 24))) {
                    // error...
                }
                rect.setTexture(&textureFront);
            } else if (direction == Direction::kRight) {

                if (!textureFront.loadFromFile("head_sheet.png", sf::IntRect(24, 0, 24, 24))) {
                    // error...
                }
                rect.setTexture(&textureFront);
            } else if (direction == Direction::kDown) {
                if (!textureFront.loadFromFile("head_sheet.png", sf::IntRect(24, 24, 24, 24))) {
                    // error...
                }
                rect.setTexture(&textureFront);
            } else if (direction == Direction::kUp) {
                if (!textureFront.loadFromFile("head_sheet.png", sf::IntRect(0, 0, 24, 24))) {
                    // error...
                }
                rect.setTexture(&textureFront);
            }
        } else if (snake_.size() == (count + 1)) {
            if (!textureFront.loadFromFile("box.jpg", sf::IntRect(0, 24, 24, 24))) {
                // error...
            }
            rect.setTexture(&textureFront);
        } else {
            rect.setTexture(nullptr);
            rect.setFillColor(snake_color);
            rect.setOutlineColor(BACKGROUND_COLOR);
            rect.setOutlineThickness(1);
        }
        count++;
        window_.draw(rect);
    }

    //rect.setFillColor(sf::Color::Red);
    rect.setPosition(fruit_.x * PIXEL_SIZE + 1, fruit_.y * PIXEL_SIZE + 1);



    // ANIMATION FOR FRUITS
    if (!animatedTexture.loadFromFile("fruits.png")) {
        // Handle error loading texture
    }
    // END OF ANIMATION FRUITS

    if (clockForAnimation.getElapsedTime().asMilliseconds() > 50){
        if (rectSourceSprite.left == 280) {
            rectSourceSprite.left = 0;
        }
        else {
            rectSourceSprite.left += 70;
        }
        animatedFruit.setTextureRect(rectSourceSprite);
        clockForAnimation.restart();
    }
    rect.setTexture(animatedFruit.getTexture());
    rect.setTextureRect(rectSourceSprite);


    if(score_ == 5) {
        sf::Text achievment;
        achievment.setFont(font);
        achievment.setString("You get 5 fruits!");

        auto text_length = achievment.getString().getSize();
        auto fontSize = 40;

        achievment.setCharacterSize(fontSize);
        achievment.setFillColor(sf::Color::Yellow);

        achievment.setPosition(300, 0);
        fade.setPosition(300, 0);

        auto x = float(text_length) * 20;
        auto sizeOfX = x;
        auto y = float(fontSize) + 40;
        auto sizeOfY = y;


        fade.setSize(sf::Vector2f(sizeOfX, sizeOfY));
        int currentTime = timer.getElapsedTime().asMilliseconds();
        int currentAlpha = endAlpha;
        if (currentTime >= targetTime) {

            //you are done
        } else {
            currentAlpha = startAlpha + (endAlpha - startAlpha) * currentTime / targetTime;
        }
        fade.setFillColor(sf::Color(255, 0, 0, currentAlpha));
        achievment.setFillColor(sf::Color(0, 255, 255, currentAlpha));

        window_.draw(fade);
        window_.draw(achievment);
    }


    window_.draw(animatedFruit);
    window_.draw(rect);
    window_.display();
}

void Snake::Move() {
    pos new_head{.y = snake_[0].y, .x = snake_[0].x};


    switch (dir_) {
        case Direction::kLeft: {
            if (new_head.x == 0) {
                new_head.x = FIELD_SIZE - 1;
            } else {
                --new_head.x;
            }
            break;
        }
        case Direction::kRight: {
            if (new_head.x == FIELD_SIZE - 1) {
                new_head.x = 0;
            } else {
                ++new_head.x;
            }
            break;
        }
        case Direction::kUp: {
            if (new_head.y == 0) {
                new_head.y = FIELD_SIZE - 1;
            } else {
                --new_head.y;
            }
            break;
        }
        case Direction::kDown: {
            if (new_head.y == FIELD_SIZE - 1) {
                new_head.y = 0;
            } else {
                ++new_head.y;
            }
            break;
        }
    }

    // Game over state
    for (auto it: snake_) {
        game_over_ = (new_head.x == it.x && new_head.y == it.y);

        if (game_over_) {
            SetTitle();
            return;
        }
    }

    // Filling map with snake rectangle each step
    snake_.insert(snake_.begin(), new_head);
    // Collision for fruit
    if (new_head.x == fruit_.x && new_head.y == fruit_.y) {
        score_++;
        collected_fruits.emplace_back(fruit_);
        std::cout << "You picked: " << fruit_.image << std::endl;




        music.play();

//        sf::SoundBuffer buffer;
//        if (!buffer.loadFromFile("crunch.wav")){
//            std::cout << "sound not loaded!";
//        }
//
//        sf::Sound sound;
//        sound.setBuffer(buffer);
//        sound.play();

        fruit_ = CreateFruit();

        for (const auto &item:collected_fruits) {
            std::cout << item.image << " ";
        }

        SetTitle();
    } else {
        // Destroying each previous rendering Rectangle
        snake_.pop_back();

    }
}

void Snake::SetTitle() {


    std::stringstream stream{};
    if (!game_over_) {
        stream << "SNAKE | SCORE: " << score_;
    } else {
        stream << "SNAKE | YOU " << (score_ == 21 * 21 ? "WON" : "LOST");
    }
    window_.setTitle(stream.str());


}

pos Snake::CreateFruit() {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<> next(0, FIELD_SIZE - 1);

    const int NUM_IMAGES = 5;
    std::srand(std::time(0));
    std::string images[NUM_IMAGES] = {
            "melon.jpg",
            "pineapple.png",
            "apple.jpg",
            "cherry.png",
            "banana.png",
    };
    int randomIndex = std::rand() % NUM_IMAGES;




    //pos koordinate{1, 2};

    pos new_fruit{next(gen), next(gen), images[randomIndex]};

    auto f = [&new_fruit](const pos &val) {
        return new_fruit.x == val.x && new_fruit.y == val.y;
    };

    while (std::find_if(snake_.begin(), snake_.end(), f) != snake_.end()) {
        new_fruit.x = next(gen);
        new_fruit.y = next(gen);
    }
    fruit_ = new_fruit;
    return new_fruit;
}

#undef REVERSED_DIRECTION_X
#undef REVERSED_DIRECTION_Y
#undef BACKGROUND_COLOR
