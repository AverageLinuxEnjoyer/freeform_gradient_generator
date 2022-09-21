add_executable(imgui-sfml-example
  application.cpp
  gradient.cpp
  gui.cpp
  main.cpp
)

target_link_libraries(imgui-sfml-example
  PUBLIC
    ImGui-SFML::ImGui-SFML
)

include(Install.cmake)