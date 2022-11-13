#include "logger.h"

Logger::Logger(std::string filename, const size_t &values_number) : filename_(std::move(filename)) {
  data_.SetColumnName(0, "time");
  for (auto i = 0; i != values_number; ++i) {
    data_.SetColumnName(3 * i + 1, fmt::format("x{}", i));
    data_.SetColumnName(3 * i + 2, fmt::format("y{}", i));
    data_.SetColumnName(3 * i + 3, fmt::format("z{}", i));
  }
}

Logger::Logger(std::string filename, const std::vector<std::string> &values) : filename_(std::move(filename)) {
  data_.SetColumnName(0, "time");
  for (auto i = 1; i != values.size() + 1; ++i) {
    data_.SetColumnName(i, values[i - 1]);
  }
}

void Logger::log(const std::vector<double> &row) {
  data_.SetRow(log_count_, row);
  data_.Save(filename_);
  log_count_++;
}