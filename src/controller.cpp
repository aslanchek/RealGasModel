#include "controller.h"

Controller::Controller(): engine(configs),
                          window(sf::VideoMode(configs["side_size"], configs["side_size"]), "Title", sf::Style::Close) {
  window.setFramerateLimit(configs["FPS"]);
  window.setSize(sf::Vector2u(800, 800));

  for (size_t i = 0; i != configs["count"]; ++i) {
    images.emplace_back(sf::CircleShape(configs["particle_size"]));
    images[i].setFillColor(sf::Color::Black);
  }

}

void Controller::render() {
  double current_time = clock.restart().asSeconds();
  window.setTitle("FPS: " + std::to_string(static_cast<int>(1.0 / current_time)));

  window.clear(sf::Color::White);

  for (size_t i = 0; i != configs["count"]; ++i) {
    images[i].setPosition(engine.particles[i].position[0] - static_cast<double>(configs["particle_size"]),
                          engine.particles[i].position[1] - static_cast<double>(configs["particle_size"]));

//    sf::CircleShape limit(configs["potential_limit"]);
//    limit.setPosition(engine.particles[i].position[0] - static_cast<double>(configs["potential_limit"]), 
//                      engine.particles[i].position[1] - static_cast<double>(configs["potential_limit"]));
//    limit.setFillColor(sf::Color::Transparent);
//    limit.setOutlineColor(sf::Color::Black);
//    limit.setOutlineThickness(0.1);
//    window.draw(limit);
    window.draw(images[i]);
  }

  window.display();
}

void Controller::log() {
  Eigen::Vector3d distance = engine.particles[0].position - engine.particles[1].position;
//  Logger::log("Distance", distance.norm());
//  Logger::log("Acceleration", engine.particles[0].acceleration.norm());
  Logger::log("Kinetic Energy", engine.getSystemKineticEnergy());
  std::system("clear");
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

void Controller::update() {
  eventHandler();
  engine.update();
}

void Controller::run() {
  while (window.isOpen()) {
    render();
    update();
//    log();
  }
}
