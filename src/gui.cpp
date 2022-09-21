#include "gui.hpp"
#include <imgui.h>

Gui::Gui(sf::RenderWindow &window_, Gradient &gradient_)
    : window(window_), gradient(gradient_) {
  for (const Point &point : gradient.getPoints())
    points.push_back(std::make_pair(point, 0));

  ImGui::SFML::Init(this->window);
}

Gui::~Gui() { ImGui::SFML::Shutdown(); }

void Gui::processEvents(sf::Event event) { ImGui::SFML::ProcessEvent(event); }

void Gui::update(sf::Time deltaTime) {
  ImGui::SFML::Update(this->window, deltaTime);

  // this->syncrhonizePoints();
  this->drawMainWindow();
}

void Gui::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  ImGui::SFML::Render(target);
}

void Gui::drawMainWindow() {
  auto win_size = this->window.getSize();

  ImGui::SetNextWindowPos(ImVec2(20, 20));
  ImGui::SetNextWindowSize({400.0, win_size.y - 40.0f});
  ImGui::Begin("Main Window", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
  // 4 different tabs
  if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)) {
    if (ImGui::BeginTabItem("Gradient")) {
      this->drawGradientSection();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Points")) {
      this->drawPointsSection();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Filters")) {
      ImGui::Text("Tab 3");
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Export")) {
      ImGui::Text("Tab 4");
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::End();
}

void Gui::drawGradientSection() {
  static int values[2] = {50, 50};
  ImGui::DragInt2("Size", values, 0.1f, 50, 1000);
  if (ImGui::Button("Create")) {
    gradient.create(values[0], values[1]);
  }
}

void Gui::drawPointsSection() {

  const auto &points = this->gradient.getPoints();

  ImGui::Columns(1);
  ImGui::Separator();
  for (int i = 0; i < points.size(); i++) {
    auto &point = points[i];
    ImGui::Text("Point %d", i);
    ImGui::NextColumn();
    ImGui::PushID(i);
    if (ImGui::Button("Remove")) {
      this->gradient.removePoint(i);
      i--;
    }
    ImGui::PopID();
    ImGui::NextColumn();
    ImGui::PushID(i);
    float color[3] = {point.color.r / 255.0f, point.color.g / 255.0f,
                      point.color.b / 255.0f};
    ImGui::ColorEdit3("Color", color);
    ImGui::PopID();
    ImGui::NextColumn();
    ImGui::PushID(i);
    float position[2] = {point.position.x, point.position.y};
    ImGui::DragFloat2("Position", position, 0.1f);
    ImGui::PopID();
    ImGui::NextColumn();

    auto sfColor = sf::Color(color[0] * 255, color[1] * 255, color[2] * 255);
    auto sfPosition = sf::Vector2f(position[0], position[1]);

    this->gradient.updatePoint(i, Point{sfPosition, sfColor});
  }
  ImGui::Separator();
  ImGui::Columns(1);
}

void Gui::syncrhonizePoints() {
  // synchronize the points in the vector, with the ones within the gradient,
  // keeping the id for the current points and generating new ids for new points

  const auto &gradient_points = this->gradient.getPoints();

  // remove the points that are no longer in gradient_points from this->points
  for (int i = 0, j = 0; i < this->points.size(); i++, j++) {
    auto &point = this->points[i];
    if (point.first.color != gradient_points[j].color ||
        point.first.position != gradient_points[j].position) {
      this->points.erase(this->points.begin() + i);
      i--;
    }
  }

  // add the points that are in gradient_points but not in this->points, set
  // their id to the next available id
  for (int i = 0, j = 0; i < this->points.size(); i++, j++) {
    auto &point = this->points[i];
    if (point.first.color != gradient_points[j].color ||
        point.first.position != gradient_points[j].position) {
      this->points.push_back(std::make_pair(gradient_points[j], j));
      i++;
    }
  }
}
