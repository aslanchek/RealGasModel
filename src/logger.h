#include <fstream>
#include <string>
#include <vector>

/**
 * @brief   Logger class implementation
 *          Needed to save passed data to .csv file
 */
class Logger {
 public:
  /**
   * @brief     Constructor
   * @param     filename      std::string of filename of .csv file to be logged into 
   */
  Logger(std::string filename) : fout_(std::move(filename), std::fstream::out) {};

  /**
   * @brief     Constructor
   * @param     filename      std::string of filename of .csv file to be logged into 
   * @param     values        std::vector of columns names
   */
  Logger(std::string filename, const std::vector<const char *> &values) :
      fout_(std::move(filename), std::fstream::out) {
    fout_ << getCsvLine(values);
  }

  /**
   * @brief     Write value into corresponding file
   * @tparam    T           the type of value to be logged into .csv file
   * @param     values      std::vector of logging values
   */
  template<typename T>
  void log(const std::vector<T> &values) {
    fout_ << getCsvLine(values);
    fout_.flush();
  }

 private:
  std::ofstream fout_;

  /**
   * @brief     to_string addition
   * @returns   passed value converted to std::string
   */
  std::string get_str(const std::string &value) {
    return value;
  }

  /**
   * @brief     to_string addition  (overloaded)
   * @returns   passed value converted to std::string
   */
  std::string get_str(const double &value) {
    return std::to_string(value);
  }

  /**
   * @brief       Returns csv line by vector of values provided
   * @tparam      T           the type of data to be converted into std::string and then into csv format
   * @param       values      std::vector of values to be logged
   * @returns     std::string of passed values converted into .csv format
   */
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
