#ifndef INC_SFMLREALGASMODEL_SRC_CONTROLLER_H_
#define INC_SFMLREALGASMODEL_SRC_CONTROLLER_H_

#include "engine.h"
#include "logger.h"

#include <nlohmann/json.hpp>
#include <chrono>

class Controller {
 public:
  Controller();
  void log();
  void update();
  void run();
 private:
  const nlohmann::json configs = nlohmann::json::parse(std::ifstream("../src/conf.json"));
  Engine engine;
  Logger position_logger;
  Logger velocity_logger;
  Logger energy_logger;
};

#endif //INC_2DREALGASMODEL_SRC_CONTROLLER_H_
