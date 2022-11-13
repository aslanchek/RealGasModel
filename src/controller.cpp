#include "controller.h"

Controller::Controller()
    : isOpen(true),
      engine(configs),
      position_logger("position_data.csv", static_cast<size_t>(configs["count"]))
      /*visualizer("position_data.csv", configs)*/ {};

void Controller::log() {
  std::vector<double> row{engine.getTime()};
  for (auto &particle : engine.particles) {
    for (int i = 0; i != 3; ++i) { row.push_back(particle.position[i]); }
  }
  position_logger.log(row);
}


void Controller::update() {
  engine.update();
}

void Controller::run() {
  for(unsigned long long i = 0; i < 10000t ; i ++) {
    printf("%d\n", i);
    update();
    log();
  }
//  visualizer.render();
}
