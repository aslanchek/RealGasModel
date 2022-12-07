#include "controller.h"

Controller::Controller()
    : engine(configs),
      position_logger("../data/position_data.csv"),
      velocity_logger("../data/velocity_data.csv"),
      energy_logger("../data/energy_data.csv", std::vector{"Time", "Kinetic Energy", "Potential Energy"}) {};

void Controller::log() {
  std::vector<double> row_pos{};
  std::vector<double> row_vel{};
  std::vector<double> row_energy{engine.getTime()};

  row_energy.push_back(engine.getSystemKineticEnergy());
  row_energy.push_back(engine.getSystemPotentialEnergy());
  energy_logger.log(row_energy);

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
}

void Controller::update() {
  engine.update();
}

std::string Controller::to_readable_time(const uint64_t &seconds) {
  uint64_t h = seconds / 3600;
  uint64_t m = (seconds - h * 3600) / 60;
  uint64_t s = seconds - h * 3600 - m * 60;
  std::string timestr = std::to_string(h) + "h " + std::to_string(m) + "m " + std::to_string(s) + "s";
  return timestr;
}

void Controller::run() {

  uint64_t STEPS = static_cast<int64_t>(configs["steps"]);
  uint64_t log_step = static_cast<int64_t>(configs["log_step"]);

  auto start = std::chrono::steady_clock::now();
  for (int64_t i = 1; i < STEPS + 1; ++i) {
    update();
    if (i % log_step == 0) { log(); }
    auto end = std::chrono::steady_clock::now();

    if (i % log_step == 0) {
      float done = static_cast<float>(i) / static_cast<float>(STEPS) * 100;
      uint64_t elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
      uint64_t remaining = (static_cast<float>(elapsed) / static_cast<float>(i)) * static_cast<float>((STEPS - i));

      fmt::print("done: {:.1f}% elpsd: {} rem: {}\n", done, to_readable_time(elapsed), to_readable_time(remaining));
    }
  }
}
