#include <fstream>
#include <iostream>
#include <string>
#include <rapidcsv.h>
#include <vector>
#include <nlohmann/json.hpp>

/**
 * @brief   Coverts csv data to the lammps dump format to visualization using OVITO
 * @param   Filename in csv (e.g. position_data.csv -> position_data.csv.lammps)
 */

int main() {
  const nlohmann::json configs = nlohmann::json::parse(std::ifstream("../data/conf.json"));

  const uint64_t STEPS_ = configs["steps"];
  const uint64_t particle_number_ = configs["count"];
  const uint64_t log_step_ = configs["log_step"];
  const uint64_t box_size_ = configs["simulation_box_size"];
  const double dt_ = configs["dt"];
  const double timestep_ = dt_ * log_step_;

  std::string filename_;
  std::cout << "Enter filename with positions for formatting for Ovito lammps dump: ";
  std::cin >> filename_;

  rapidcsv::Document data_("../data/" + filename_);
  std::ofstream fout_("../data/" + filename_ + ".lammps");

  double time_ = timestep_;
  uint64_t index = 0;

  for (uint64_t step = 0; step != STEPS_; step += log_step_) {
    fout_ << "ITEM: TIMESTEP\n" << time_ << '\n';
    fout_ << "ITEM: NUMBER OF ATOMS\n" << particle_number_ << '\n';
    fout_ << "ITEM: BOX BOUNDS\n";
    fout_ << 0 << " " << box_size_ << '\n';
    fout_ << 0 << " " << box_size_ << '\n';
    fout_ << 0 << " " << box_size_ << '\n';
    fout_ << "ITEM: ATOMS x y z\n";

    for (uint64_t n = 0; n < particle_number_; ++n) {
      try {
        std::vector<double> position = data_.GetRow<double>(index);
        fout_ << position[0] << ' ' << position[1] << ' ' << position[2] << '\n';
        ++index;
      } catch (const std::out_of_range &) {
        std::cerr << "csv parse error: out of range on " << std::to_string(index) << " row" << '\n';
        return EXIT_FAILURE;
      }
    }

    fout_.flush();
    time_ += timestep_;
  }
}
