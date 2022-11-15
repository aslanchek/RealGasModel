#include "controller.h"

Controller::Controller()
    : isOpen(true),
      engine(configs),
      position_logger("../data/position_data.csv", static_cast<size_t>(configs["count"])),
      velocity_logger("../data/velocity_data.csv", static_cast<size_t>(configs["count"])),
      energy_logger("../data/energy_data.csv", std::vector<std::string>{"Kinetic Energy", "Potential Energy"}) {};

void Controller::log() {
  std::vector<double> row_pos{engine.getTime()};
  std::vector<double> row_vel{engine.getTime()};

  for (auto &particle : engine.particles) {
    for (int i = 0; i != 3; ++i) { row_pos.push_back(particle.position[i]); }
    for (int i = 0; i != 3; ++i) { row_vel.push_back(particle.velocity[i]); }
  }
  position_logger.log(row_pos);
  velocity_logger.log(row_vel);
}

void Controller::update() {
  engine.update();
}

void Controller::run() {
  for (unsigned long long i = 0; i < configs["steps"]; i++) {
    update();
    if (i % 500 == 0) {
      std::system("clear");
      printf("%d%s\n", (int) ((float) i / (float) configs["steps"] * 100), "%");
      log();
    }
  }
}
