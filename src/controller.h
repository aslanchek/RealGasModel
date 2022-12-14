#ifndef INC_REALGASMODEL_SRC_CONTROLLER_H_
#define INC_REALGASMODEL_SRC_CONTROLLER_H_

#include "engine.h"
#include "logger.h"

#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <chrono>

/**
 * @brief     Necessary to connect all program modules together.
 *            Has Engine instance providing all physics calculations.
 *            Has Logger instances for several types of logging values.
 */
class Controller {
/// #smth
 public:
  Controller();
  
  /**
   * @brief     Converts passed time (in seconds) to human-readable format(hh:mm:ss)
   *            (used in remaining time estimating)
   * @param     seconds     Time in seconds 
   * @returns   Human-readable time in std::string
   */
  std::string ToReadableTime(const uint64_t& seconds);

  /**
   * @brief     Provides logger-members work
   */
  void Log();

  /**
   * @brief     Move the steppable instances to the next step (e.g. Engine)
   */
  void Update();

  /**
   * @brief     Convenient run whole program (see main.cpp)
   */
  void Run();
 private:
  /**
   * @brief     Configuration file instance (see data/conf.json)
   */
  const nlohmann::json configs = nlohmann::json::parse(std::ifstream("../data/conf.json"));

  /**
   * @brief     Molecular Engine instance, providing all calculating process (see engine.h)
   */
  Engine engine_;

  /**
   * @brief     Members that save specified valuables in corresponding file (see logger.h)
   */
  Logger position_logger_;

  /**
   * @brief     Members that save specified valuables in corresponding file (see logger.h)
   */
  Logger absolute_position_logger_;

   /**
   * @brief     Members that save specified valuables in corresponding file (see logger.h)
   */
  Logger velocity_logger_;

  /**
   * @brief     Members that save specified valuables in corresponding file (see logger.h)
   */
  Logger energy_logger_;
};

#endif //INC_REALGASMODEL_SRC_CONTROLLER_H_
