#ifndef INC_SFMLREALGASMODEL_SRC_CONTROLLER_H_
#define INC_SFMLREALGASMODEL_SRC_CONTROLLER_H_

#include "engine.h"
#include "visualisation.h"
#include "logger.h"

#include <nlohmann/json.hpp>

class Controller {
 public:
  Controller();
  void log();
  void update();
  void run();
 private:
  bool isOpen;
  const nlohmann::json configs = nlohmann::json::parse(std::ifstream("../src/conf.json"));
  Engine engine;
  Logger position_logger;
  //Visualizer visualizer;

};

#endif //INC_2DREALGASMODEL_SRC_CONTROLLER_H_