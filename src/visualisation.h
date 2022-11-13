#ifndef SFMLREALGASMODEL_SRC_VISUALISATION_H_
#define SFMLREALGASMODEL_SRC_VISUALISATION_H_

#include "rapidcsv.h"

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

class Visualizer {
 public:
  Visualizer(std::string, const nlohmann::json &);

  void render();
  void run();
 private:

  const nlohmann::json configs_;

  uint fps_;
  size_t window_side_size_;
  double particle_size_;

  std::string filename_;
  rapidcsv::Document data_;
  sf::RenderWindow window_;
  sf::Clock clock_;
  std::vector<sf::CircleShape> images_; // vector of images for each partile
};

#endif //SFMLREALGASMODEL_SRC_VISUALISATION_H_