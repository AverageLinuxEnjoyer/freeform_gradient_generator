#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>
#include "gui.hpp"
#include "gradient.hpp"

class Application {
public:
    Application(std::string title, int width, int height);

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    ~Application();

    void run();

private:
    void processEvents();
    void update();
    void draw();

    // event processing related
    void handleWindowResize(sf::Event event);
    void handleMouseWheelScroll(sf::Event event);
    void handleGradientDrag(sf::Event event);

    void close();

private:
    sf::RenderWindow window;    
    sf::View view;

    Gradient gradient;
    Gui gui;

    sf::Time deltaTime;
};

#endif // APPLICATION_HPP
