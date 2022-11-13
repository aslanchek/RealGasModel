#ifndef SFMLREALGASMODEL_SRC_LOGGER_H_
#define SFMLREALGASMODEL_SRC_LOGGER_H_

#include <vector>
#include <rapidcsv.h>
#include <fmt/core.h>

class Logger {
 public:
  Logger(std::string, const size_t &);
  Logger(std::string, const std::vector<std::string> &);
  void log(const std::vector<double> &);
 private:
  rapidcsv::Document data_;
  std::string filename_;
  size_t log_count_ = 0;
};

#endif //SFMLREALGASMODEL_SRC_LOGGER_H_
