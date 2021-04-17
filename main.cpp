
// SFML-2.4.2 folder main library files [ You can move anywhere but should change dir path in CmakeLists.txt ]
// SFML_COMPILER MingGW x64 compiler for sfml [ Add to Toolchains ]
// Other IDE's more info https://www.sfml-dev.org/ and in Youtube

#include <SFML/Graphics.hpp>
int main()
{
  sf::RenderWindow window(sf::VideoMode(300, 200), "SFML Application");

  sf::CircleShape circleShape(10);
  circleShape.setFillColor(sf::Color::Red);

  //Game loop
  while (window.isOpen())
  {
    //handle event
    sf::Event event;
    while (window.pollEvent(event))
    {
      if  (event.type == sf::Event::EventType::Closed ||
           (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
        window.close();
    }
    window.clear(sf::Color::Black);
    window.draw(circleShape);
    //Render objects
    window.display();
  }
  return 0;
}