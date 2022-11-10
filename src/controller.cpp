#include "controller.h"

Controller::Controller(): engine(configs), window(sf::VideoMode(configs["side_size"], configs["side_size"]), "Title") {
  window.setFramerateLimit(configs["FPS"]);

  for(size_t i = 0; i != configs["count"]; ++i) {
    images.emplace_back(sf::CircleShape(4));
  }
}

void Controller::render() {
  double current_time = clock.restart().asSeconds();
  window.setTitle(std::to_string(1.0 / current_time));

  window.clear(sf::Color::White);

  for(size_t i = 0; i != configs["count"]; ++i) {
    images[i].setPosition(engine.particles[i].position[0], engine.particles[i].position[0]);
    window.draw(images[i]);
  }

  window.display();
}

void Controller::eventHandler() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      window.close();
  }
}

void Controller::update() {
  eventHandler();
}

void Controller::run() {
  while(window.isOpen()) {
    update();
    render();
  }
}
