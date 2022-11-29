#include "controller.h"

Controller::Controller()
    : engine(configs),
      position_logger("../data/position_data.csv", std::vector<std::string>{"x", "y", "z"}),
      velocity_logger("../data/velocity_data.csv", std::vector<std::string>{"Vx", "Vy", "Vz"}),
      energy_logger("../data/energy_data.csv",
                    std::vector<std::string>{"Time", "Kinetic Energy", "Potential Energy"}) {};

void Controller::log() {
  std::vector<double> row_pos{};
  std::vector<double> row_vel{};
  std::vector<double> row_energy{engine.getTime()};

  for (auto &particle : engine.particles) {
    for (int i = 0; i < 3; ++i) {
      row_vel.push_back(particle.velocity[i]);
      row_pos.push_back(particle.position[i]);
    }
    position_logger.log(row_pos);
    velocity_logger.log(row_vel);
    row_vel.clear();
    row_pos.clear();
  }
  row_energy.push_back(engine.getSystemKineticEnergy());
  row_energy.push_back(engine.getSystemPotentialEnergy());

  energy_logger.log(row_energy);
}

void Controller::update() {
  engine.update();
}

void Controller::run() {

  auto start = std::chrono::high_resolution_clock::now();
  for (uint64_t i = 0; i < static_cast<uint64_t>(configs["steps"]); ++i) {
    update();
    auto end = std::chrono::high_resolution_clock::now();

    if (i % (size_t) configs["log_step"] == 0) {
      double done = (double) i / (double) configs["steps"] * 100;
      double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
      size_t remaining = ((size_t) configs["steps"] - i) * (elapsed / i);
      size_t h = remaining / 3600000;
      size_t m = (size_t) (remaining - h * 3600000) / 60000;
      size_t s = (size_t) (remaining - h * 3600000 - m * 60000) / 1000;

      printf("done %.1f%s rem %zuh %zum %zus\n", done, "%", h, m, s);

      log();
    }
  }
}