#ifndef INC_2DREALGASMODEL_SRC_CONTROLLER_H_
#define INC_2DREALGASMODEL_SRC_CONTROLLER_H_
#include "engine.h"

class Controller {
 public:
  Controller();
  void render();
  void log();
  void eventHandler();
  void update();
  void run();
 private:
  double time = 0;
  const nlohmann::json configs = nlohmann::json::parse(std::ifstream("../src/conf.json"));
  Engine engine;
  sf::RenderWindow window;
  sf::Clock clock;
  std::vector<sf::CircleShape> images; // vector of images for each partile
};

#endif //INC_2DREALGASMODEL_SRC_CONTROLLER_H_