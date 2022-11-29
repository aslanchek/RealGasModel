#include "logger.h"

Logger::Logger(std::string filename, const std::vector<std::string> &values) : filename_(std::move(filename)) {
  for (auto i = 0; i != values.size(); ++i) {
    data_.SetColumnName(i, values[i]);
  }
}

void Logger::log(const std::vector<double> &row) {
  data_.SetRow(log_count_, row);

  log_count_++;
  if(log_count_ / 100) {
    data_.Save(filename_);
  }

}