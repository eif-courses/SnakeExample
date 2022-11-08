
// SFML-2.4.2 folder main library files [ You can move anywhere but should change dir path in CmakeLists.txt ]
// SFML_COMPILER MingGW x64 compiler for sfml [ Add to Toolchains ]
// Other IDE's more info https://www.sfml-dev.org/ and in Youtube

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

//See this link: https://www.sfml-dev.org/tutorials/2.4/graphics-sprite.php
//
//You're loading in your sf::texture to a variable local to the constructor. When the code block for the constructor has finished running, this variable goes out of scope. When this happens, your sf::texture is destroyed, which causes the white square. You need to store the texture in a way that won't let it go out of scope, such as allocating it to the heap.


sf::RectangleShape createBox(const string &image) {
    sf::RectangleShape rectangle(sf::Vector2f(120.f, 50.f));
    rectangle.setSize(sf::Vector2f(100.f, 100.f));
    auto *texture = new sf::Texture;
    if (!texture->loadFromFile(image)) {
        cerr << "Failed to load texture!" << endl;
    }
    rectangle.setTexture(texture);
    return rectangle;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML Application");

    // Create a new render-texture
    sf::RenderTexture texture;
    if (!texture.create(500, 500))
        return -1;

    //Game loop
    while (window.isOpen()) {

        texture.clear(sf::Color::Red);
        // Draw stuff to the texture
        //handle event
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::EventType::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
        }
        window.clear(sf::Color::Black);


        // rows cols
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                sf::RectangleShape box = createBox("box.jpg");
                box.setPosition(i * 100, j * 100);
                texture.draw(box);
                auto mouse_pos = sf::Mouse::getPosition(window); // Mouse position relative to the window
                auto translated_pos = window.mapPixelToCoords(mouse_pos); // Mouse position translated into world coordinates
                if (box.getGlobalBounds().contains(translated_pos) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

                    auto *te = new sf::Texture;
                    if (!te->loadFromFile("water.jpg")) {
                        cerr << "Failed to load texture!" << endl;
                    }
                    box.setTexture(te);
                } // Rectangle-contains-point check
                // Mouse is inside the sprite.
                //window.draw(box);

            }
        }
        texture.display();
        sf::Sprite sprite(texture.getTexture());
        window.draw(sprite);
        //Render objects
        window.display();
    }
    return 0;
}