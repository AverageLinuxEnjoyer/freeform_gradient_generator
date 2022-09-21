#ifndef GRADIENT_HPP
#define GRADIENT_HPP

#include "point.hpp"

class Gradient : public sf::Drawable {
public:
  Gradient(unsigned int width, unsigned int height);
  ~Gradient() = default;

  void addPoint(Point point);
  void removePoint(int index);
  void updatePoint(int index, Point point);

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  sf::Vector2f getPosition() const;
  sf::Vector2f getSize() const;
  sf::Vector2f getScale() const;
  const std::vector<Point> &getPoints() const;

  void setScale(float x, float y);

  bool isCreated() const;
  void create(unsigned int width, unsigned int height);

private:
  void loadPointsIntoTexture();

private:
  sf::RectangleShape body;
  const sf::Vector2u size;
  sf::Vector2f position;

  bool created = false;

  sf::Shader shader;
  sf::RenderTexture texture;
  sf::Sprite drawn_sprite;

  std::vector<Point> points;
};

#endif // GRADIENT_HPP
