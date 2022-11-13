#include "visualisation.h"


//void Controller::eventHandler() {
//  sf::Event event;
//  while (window.pollEvent(event)) {
//    if (event.type == sf::Event::Closed) {
//      window.close();
//    } else if (event.type == sf::Event::KeyPressed) {
//      if (event.key.code == sf::Keyboard::L) {
//        log();
//      }
//    }
//  }
//}


Visualizer::Visualizer(std::string filename, const nlohmann::json &configs)
    : configs_(configs),
      fps_(configs["FPS"]),
      window_side_size_(configs["window_side_size"]),
      particle_size_(configs["particle_size"]),
      filename_(std::move(filename)),
      data_(filename_),
      window_(sf::VideoMode((size_t) configs["simulation_side_size"], (size_t) configs["simulation_side_size"]),
              "Lennard Jones Potential",
              sf::Style::Close) {

  window_.setFramerateLimit(fps_);
  window_.setSize(sf::Vector2u(window_side_size_, window_side_size_));

  size_t particles_count = (data_.GetColumnCount() - 1) / 3;
  for (size_t i = 0; i != particles_count; ++i) {
    images_.emplace_back(sf::CircleShape(particle_size_));
    images_[i].setFillColor(sf::Color::Black);
  }
}

void Visualizer::render() {
  for (size_t i = 0; i != data_.GetRowCount(); ++i) {
    double current_time = clock_.restart().asSeconds();
    window_.setTitle("FPS: " + std::to_string(static_cast<int>(1.0 / current_time)) + "        "
                         + std::to_string((double)i / (double)data_.GetRowCount() * 100) + "%");
    window_.clear(sf::Color::White);

    std::vector<double> row = data_.GetRow<double>(i);

    for (size_t j = 0; j != configs_["count"]; ++j) {
      images_[j].setPosition(row[3 * j + 1] - particle_size_, row[3 * j + 2] - particle_size_);
      window_.draw(images_[j]);
    }

    window_.display();
  }

}