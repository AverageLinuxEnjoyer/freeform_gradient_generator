#include "gradient.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shader.hpp>

Gradient::Gradient(unsigned int width, unsigned int height)
    : body(sf::Vector2f(width, height)),
      size(width, height),
      position(0.f, 0.f),
      shader(),
      texture(),
      drawn_sprite(),
      points()
{
    this->create(width, height);
}

void Gradient::addPoint(Point point) {
    points.push_back(point);

    this->shader.setUniform("used_points", static_cast<int>(points.size()));

    int point_index = this->points.size() - 1;
    this->shader.setUniform("points[" + std::to_string(point_index) + "]", point.position);
    this->shader.setUniform("colors[" + std::to_string(point_index) + "]", sf::Glsl::Vec4(point.color));

    this->loadPointsIntoTexture();
}

void Gradient::removePoint(int index) {
    if (index < 0 || index >= this->points.size())
        throw std::runtime_error("Invalid point index.");

    this->points.erase(this->points.begin() + index);

    this->shader.setUniform("used_points", static_cast<int>(points.size()));

    for (int i = 0; i < this->points.size(); ++i)
    {
        this->shader.setUniform("points[" + std::to_string(i) + "]", this->points[i].position);
        this->shader.setUniform("colors[" + std::to_string(i) + "]", sf::Glsl::Vec4(this->points[i].color));
    }

    this->loadPointsIntoTexture();
}

void Gradient::updatePoint(int index, Point point) {
    if (index < 0 || index >= this->points.size())
        throw std::runtime_error("Invalid point index.");

    this->points[index] = point;
    
    this->shader.setUniform("points[" + std::to_string(index) + "]", point.position);
    this->shader.setUniform("colors[" + std::to_string(index) + "]", sf::Glsl::Vec4(point.color));

    this->loadPointsIntoTexture();
}

void Gradient::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (this->points.size() == 0)
    {
        
    }
    else
        target.draw(this->drawn_sprite, states);
}

sf::Vector2f Gradient::getPosition() const {
    return this->drawn_sprite.getPosition();
}

sf::Vector2f Gradient::getSize() const {
    return static_cast<sf::Vector2f>(this->drawn_sprite.getTexture()->getSize());
}

sf::Vector2f Gradient::getScale() const {
    return this->drawn_sprite.getScale();
}

const std::vector<Point>& Gradient::getPoints() const {
    return this->points;
}


void Gradient::setScale(float x, float y) {
    this->drawn_sprite.setScale(x, y);
}

bool Gradient::isCreated() const {
    return this->isCreated();
}

void Gradient::create(unsigned int width, unsigned int height) {
    if (!this->texture.create(width, height))
        throw std::runtime_error("Failed to create gradient texture");

    this->drawn_sprite.setTexture(this->texture.getTexture());
    this->drawn_sprite.setOrigin(this->size.x / 2.0f, this->size.y / 2.0f);

    if (!this->shader.loadFromFile("/home/nullptr/Documents/Learning/Programming/C++/freeform-gradient-generator/src/shader.glsl", sf::Shader::Fragment))
        throw std::runtime_error("Failed to load shader");

    this->shader.setUniform("gradient_height", static_cast<int>(height));
}

void Gradient::loadPointsIntoTexture() {
    this->texture.clear();
    this->texture.draw(this->body, &this->shader);
    this->texture.display();    
}
