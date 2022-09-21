#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "gradient.hpp"
#include "point.hpp"

class Gui : public sf::Drawable {
public:
    Gui(sf::RenderWindow& window_, Gradient& gradient_);
    ~Gui();

    void processEvents(sf::Event event);
    void update(sf::Time deltaTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void drawMainWindow();

    void drawGradientSection();
    void drawPointsSection();

    void syncrhonizePoints();

private:
    sf::RenderWindow &window;
    Gradient& gradient;
    std::vector<std::pair<Point, int>> points;
};

#endif // GUI_HPP
