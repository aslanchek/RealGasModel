#include "controller.h"

Controller::Controller()
    : engine_(configs),
      position_logger_("../data/position_data.csv"),
      absolute_position_logger_("../data/absolute_position_data.csv"),
      velocity_logger_("../data/velocity_data.csv"),
      energy_logger_("../data/energy_data.csv", std::vector{"Time", "Kinetic Energy", "Potential Energy"}) {};

void Controller::Log() {
  std::vector<double> row_pos{};
  std::vector<double> row_abs_pos{};
  std::vector<double> row_vel{};
  std::vector<double> row_energy{engine_.GetTime()};

  row_energy.push_back(engine_.GetSystemKineticEnergy());
  row_energy.push_back(engine_.GetSystemPotentialEnergy());
  energy_logger_.log(row_energy);

  for (auto &particle : engine_.particles_) {
    for (int i = 0; i < 3; ++i) {
      row_vel.push_back(particle.velocity[i]);
      row_pos.push_back(particle.position[i]);
      row_abs_pos.push_back(particle.position[i] + particle.transit[i] * 5);
    }
    position_logger_.log(row_pos);
    absolute_position_logger_.log(row_abs_pos);
    velocity_logger_.log(row_vel);

    row_vel.clear();
    row_pos.clear();
    row_abs_pos.clear();
  }
}

void Controller::Update() {
  engine_.Update();
}

std::string Controller::ToReadableTime(const uint64_t &seconds) {
  uint64_t h = seconds / 3600;
  uint64_t m = (seconds - h * 3600) / 60;
  uint64_t s = seconds - h * 3600 - m * 60;
  std::string timestr = std::to_string(h) + "h " + std::to_string(m) + "m " + std::to_string(s) + "s";
  return timestr;
}

void Controller::Run() {

  uint64_t STEPS = static_cast<int64_t>(configs["steps"]);
  uint64_t log_step = static_cast<int64_t>(configs["log_step"]);

  auto start = std::chrono::steady_clock::now();
  for (int64_t i = 1; i < STEPS + 1; ++i) {
    Update();
    if (i % log_step == 0) { Log(); }
    auto end = std::chrono::steady_clock::now();

    if (i % log_step == 0) {
      float done = static_cast<float>(i) / static_cast<float>(STEPS) * 100;
      uint64_t elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
      uint64_t remaining = (static_cast<float>(elapsed) / static_cast<float>(i)) * static_cast<float>((STEPS - i));

      fmt::print("done: {:.1f}% elpsd: {} rem: {}\n", done, ToReadableTime(elapsed), ToReadableTime(remaining));
    }
  }
}
