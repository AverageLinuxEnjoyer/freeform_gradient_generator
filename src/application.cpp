#include "application.hpp"
#include "point.hpp"
#include <fmt/core.h>

// function to get a random color
sf::Color getRandomColor() {
    return sf::Color(
        static_cast<sf::Uint8>(rand() % 255),
        static_cast<sf::Uint8>(rand() % 255),
        static_cast<sf::Uint8>(rand() % 255)
    );
}

Application::Application(std::string title, int width, int height)
    : window(sf::VideoMode(width, height), title),
      gradient(500,500),
      gui(window, gradient)
{
    this->view = this->window.getDefaultView();
}

Application::~Application()
{
}

void Application::run() {
    sf::Clock deltaClock;
    while(this->window.isOpen())
    {
        this->processEvents();
        this->update();
        this->draw();

        this->deltaTime = deltaClock.restart();
    }
}

void Application::processEvents() {
    sf::Event event;
    while(this->window.pollEvent(event))
    {
        gui.processEvents(event);

        if(event.type == sf::Event::Closed)
            this->close();
        else if (event.type == sf::Event::Resized)
            this->handleWindowResize(event);
        else if (event.type == sf::Event::MouseWheelScrolled)
            this->handleMouseWheelScroll(event);        
        // add a random point on mouse click
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
        {   
            auto mousePixelPos = sf::Mouse::getPosition(this->window);
            auto mouseWorldPos = this->window.mapPixelToCoords(mousePixelPos, this->view);
            auto gradientRealSize = this->gradient.getSize() * this->gradient.getScale().x;
            auto mouseGradientPos = (mouseWorldPos + gradientRealSize / 2.f) / this->gradient.getScale().x;
            
            this->gradient.addPoint(Point{mouseGradientPos, getRandomColor()});
        }

        handleGradientDrag(event);
    }
}

void Application::update() {
    gui.update(this->deltaTime);
}

void Application::draw() {
    this->window.clear();
    this->window.draw(this->gradient);

    this->window.draw(gui);
    this->window.display();
}

void Application::handleWindowResize(sf::Event event) {

    this->view.setSize(event.size.width, event.size.height);
    
    this->view.setCenter(this->gradient.getPosition());
    this->window.setView(this->view);

    // 1.0 -> no padding, 0.9 -> 10% padding
    constexpr float multiplier = 0.9;

    // scale the gradient to fit {padding_multiplier * 100}% of the window (and keep the aspect ratio)
    float scale = std::min(event.size.width / this->gradient.getSize().x, event.size.height / this->gradient.getSize().y);
    scale *= multiplier;
    this->gradient.setScale(scale, scale);    
}

void Application::handleMouseWheelScroll(sf::Event event) {
    // scale the gradient on mouse wheel scroll    
    constexpr float multiplier = 0.9;
    float scale = this->gradient.getScale().x;
    if (event.mouseWheelScroll.delta > 0)
        scale *= multiplier;
    else if (event.mouseWheelScroll.delta < 0)
        scale /= multiplier;
    this->gradient.setScale(scale, scale);
}

void Application::handleGradientDrag(sf::Event event) {
    // move the gradient on mouse drag
    return;
}

void Application::close() {
    this->window.close();
}
