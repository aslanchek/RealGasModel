#include <fstream>
#include <string>
#include <vector>

class Logger {
 public:
  Logger(std::string filename) : fout_(std::move(filename), std::fstream::out) {};

  Logger(std::string filename, const std::vector<const char *> &values) :
      fout_(std::move(filename), std::fstream::out) {
    fout_ << getCsvLine(values);
  }

  template<typename T>
  void log(const std::vector<T> &values) {
    fout_ << getCsvLine(values);
    fout_.flush();
  }

 private:
  std::ofstream fout_;

  std::string get_str(const std::string &value) {
    return value;
  }

  std::string get_str(const double &value) {
    return std::to_string(value);
  }

  template<typename T>
  std::string getCsvLine(const std::vector<T> &values) {
    std::string str;
    for (size_t i = 0; i < values.size(); ++i) {
      str.append(get_str(values[i]));
      if (i != values.size() - 1) {
        str.push_back(',');
      }
    }
    str.append("\n");
    return str;
  }
};
