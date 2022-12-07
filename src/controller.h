#ifndef INC_SFMLREALGASMODEL_SRC_CONTROLLER_H_
#define INC_SFMLREALGASMODEL_SRC_CONTROLLER_H_

#include "engine.h"
#include "logger.h"

#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <chrono>

class Controller {
 public:
  Controller();
  std::string to_readable_time(const uint64_t&);
  void log();
  void update();
  void run();
 private:
  const nlohmann::json configs = nlohmann::json::parse(std::ifstream("../data/conf.json"));
  Engine engine;
  Logger position_logger;
  Logger absolute_position_logger;
  Logger velocity_logger;
  Logger energy_logger;
};

#endif //INC_2DREALGASMODEL_SRC_CONTROLLER_H_
