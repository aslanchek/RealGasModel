#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <nlohmann/json.hpp>
#include <Eigen/Dense>
#include <cstdlib>
#include <algorithm>
#include <omp.h>

/**
 * @brief     Engine class implementation for Lennard-Jones fluid modeling
 */
class Engine {
 public:
  /**
   * @brief     Particle(molecule) class implementation
   */
  struct Particle {
    /**
     * @brief     Mass of current molecule
     */
    double mass;

    /**
     * @brief     Position components vector of current particle [x, y, z]
     */
    Eigen::Vector3d position;

    /**
     * @brief     Velocity components vector of current particle [Vx, Vy, Vz]
     */
    Eigen::Vector3d velocity;

    /**
     * @brief     Acceleration components vector of current particle [ax, ay, az]
     */
    Eigen::Vector3d acceleration;

    /**
     * @brief       Vector of transitions through borders
     *              Necessary to logging real position of
     *              current particle(excluding periodic boundary conditions) 
     *              (mean squared displacement and self-diffusion coefficient evaluation)
     * @example     If particle x-position goes more than simulation box size,
     *              then transit get {1, 0, 0}. Similarly {-1, 0, 0} if it's less.
     */
    Eigen::Vector3d transit = {0, 0, 0};

    /**
     * @brief     Constructor
     * @param     x, y, z     coordinates of current particles
     */
    Particle(double x, double y, double z, double vx, double vy, double vz, double mass) :
        position(x, y, z), velocity(vx, vy, vz), mass(mass) {};

    bool operator==(const Engine::Particle &other) {
      return this->velocity == other.velocity &&
          this->position == other.position && this->acceleration == other.acceleration && this->mass == other.mass;
    }

    bool operator!=(const Engine::Particle &other) {
      return !(*this == other);
    }

    /**
     * @brief       Kinetic Energy logging
     * @returns     Kinetic Energy of current particle
     */
    double GetKineticEnergy() {
      return mass * std::pow(velocity.norm(), 2) / 2;
    }
  };

  /**
   * @brief     Constructor
   * @param     configs       nlohmann::json instance containing configuration options
   */
  explicit Engine(const nlohmann::json & configs);

  /**
   * @brief     std::vector of particles
   */
  std::vector<Particle> particles_;

  /**
   * @brief       Get passed simulation time
   * @returns     Passed time
   */
  [[nodiscard]] double GetTime() const;

  /**
   * @brief
   */
  Eigen::Vector3d CalcAcceleration(const Engine::Particle &particle);

  /**
   * @brief
   */
  void PeriodicBoundaryCondLimit(Engine::Particle & particle, const bool & transit_check);

  /**
   * @brief
   */
  void CalcSystemPotentialEnergy(const double&, const double&);

  /**
   * @brief
   */
  double GetSystemPotentialEnergy();

  /**
   * @brief
   */
  double GetSystemKineticEnergy();

  /**
   * @brief     Velocity Verlet integration method implementation (see Wikipedia)
   */
  void Update();
 private:
  /**
   * @brief
   */
  const nlohmann::json configs;

  /**
   * @brief
   */
  double system_potential_energy_ = 0;

  /**
   * @brief
   */
  const size_t kSeed = configs["seed"];

  /**
   * @brief
   */
  const double dt = configs["dt"];

  /**
   * @brief
   */
  const int kThreads = configs["num_threads"];

  /**
   * @brief   Number of particles
   */
  const size_t kCount = configs["count"];

  /**
   * @brief     Size of simulation box
   */
  const size_t kBoxSize = configs["simulation_box_size"];

  /**
   * @brief     Max velocity projection
   */
  const double kVelocity = configs["velocity"];
  
  /**
   * @brief   Mass of all particles
   */
  const double kMass = configs["mass"];

  /**
   * @brief LJ constant
   */
  const double kSigma = configs["sigma"];

  /**
   * @brief LJ constant
   */
  const double kEpsilon = configs["epsilon"];

  /**
   * @brief LJ constant
   */
  const double kPotentialCut = configs["potential_cut"];

  const double k1 = 4 * kEpsilon * std::pow(kSigma, 12);
  const double k2 = 4 * kEpsilon * std::pow(kSigma, 6);

  /**
   * @brief Passed simulation time
   */
  double time_ = 0;
};
#endif // ENGINE_H
