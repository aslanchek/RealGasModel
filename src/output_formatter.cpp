#include <fstream>
#include <iostream>
#include <string>
#include <rapidcsv.h>
#include <vector>
#include <nlohmann/json.hpp>

int main() {
  const nlohmann::json configs = nlohmann::json::parse(std::ifstream("../src/conf.json"));

  const uint64_t STEPS_ = configs["steps"];
  const uint64_t particle_number_ = configs["count"];
  const uint64_t log_step_ = configs["log_step"];
  const uint64_t box_size_ = configs["simulation_side_size"];
  const double dt_ = configs["dt"];
  const double timestep_ = dt_ * log_step_;


  std::string filename_ = "position_data.csv";

  rapidcsv::Document data_("../data/" + filename_);
  std::ofstream fout_("../data/" + filename_ + ".lammps");

  double time_ = timestep_;
  uint64_t index = 0;

  for (uint64_t step = 0; step != STEPS_; step += log_step_) {
    fout_ << "ITEM: TIMESTEP\n" << time_ << '\n';
    fout_ << "ITEM: NUMBER OF ATOMS\n" << particle_number_ << '\n';
    fout_ << "ITEM: BOX BOUNDS\n";
    fout_ << 0 << " " << box_size_ << " " << 0 << '\n'; 
    fout_ << 0 << " " << box_size_ << " " << 0 << '\n'; 
    fout_ << 0 << " " << box_size_ << " " << 0 << '\n'; 
    fout_ << "ITEM: ATOMS x y z\n";

    for (uint64_t n = 0; n < particle_number_; ++n) {
      try {
        std::vector<double> position = data_.GetRow<double>(index);
        fout_ << position[0] << ' ' << position[1] << ' ' << position[2] << '\n';
      } catch(const std::out_of_range&) {
        std::cerr << "csv parse error: out of range on " << std::to_string(index) << " row" << '\n';
        return EXIT_FAILURE;
      }
      ++index;
    }

    fout_.flush();
    time_ += timestep_;
  }
}
