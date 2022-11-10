#include "controller.h"

Controller::Controller(): engine(configs), window(sf::VideoMode(configs["side_size"], configs["side_size"]), "Title") {
  window.setFramerateLimit(configs["FPS"]);

  for(size_t i = 0; i != configs["count"]; ++i) {
    images.emplace_back(sf::CircleShape(configs["particle_size"]));
    images[i].setFillColor(sf::Color::Black);
  }

}

void Controller::render() {
  double current_time = clock.restart().asSeconds();
  window.setTitle("FPS: " + std::to_string(static_cast<int>(1.0 / current_time)));

  window.clear(sf::Color::White);

  for(size_t i = 0; i != configs["count"]; ++i) {
    images[i].setPosition(engine.particles[i].position[0] - static_cast<double>(configs["particle_size"]),
                          engine.particles[i].position[1] - static_cast<double>(configs["particle_size"]));
    window.draw(images[i]);
  }

  window.display();
}

void Controller::eventHandler() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    } else if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::L) {
        log();
      }
    }
  }
}


void Controller::log() {
  Eigen::Vector3d distance = engine.particles[0].position - engine.particles[1].position;
  Logger::log("Distance", distance.norm());
  Logger::log("Acceleration", engine.particles[0].acceleration.norm());
  Logger::log("Kinetic Energy", engine.getSystemKineticEnergy());
  std::system("clear");
}

void Controller::update() {
  eventHandler();
  engine.update();
}

void Controller::run() {
  while (window.isOpen()) {
    render();
    update();
    //log();
  }
}
