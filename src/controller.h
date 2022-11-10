#ifndef INC_2DREALGASMODEL_SRC_CONTROLLER_H_
#define INC_2DREALGASMODEL_SRC_CONTROLLER_H_
#include "engine.h"

class Controller{
 public:
  std::ifstream file("./conf.json");
  nlohmann::json configs = nlohmann::json::parse(file);
  Engine engine(configs);
};

#endif //INC_2DREALGASMODEL_SRC_CONTROLLER_H_