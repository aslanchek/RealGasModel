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
   * @brief     Particle(molecula) class implementation
   */
  struct Particle {
    /**
     * @brief     Mass of current molecula
     */
    double mass;

    /**
     * @brief     Position componets vector of current partilce [x, y, z]
     */
    Eigen::Vector3d position;

    /**
     * @brief     Velocity componets vector of current partilce [Vx, Vy, Vz]
     */
    Eigen::Vector3d velocity;

    /**
     * @brief     Acceleration componets vector of current partilce [ax, ay, az]
     */
    Eigen::Vector3d acceleration;

    /**
     * @brief       Vector of transitions through borders
     *              Necessary to logging real position of
     *              current particle(excluding periodic boundary conditions) 
     *              (mean squared displacement and self-diffusion coefficient evaluation)
     * @example     If partile x-position goes more than simulation box size,
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
     * @brief       Kinetic Enegry logging 
     * @returns     Kinetic Energy of current particle
     */
    double getKineticEnegry() {
      return mass * std::pow(velocity.norm(), 2) / 2;
    }
  };

  Engine(const nlohmann::json &);

  /**
   * @brief     std::vector of particles
   */
  std::vector<Particle> particles;

  /**
   * @brief       Get passed simulation time
   * @returns     Passed time
   */
  double getTime() const;

  /**
   * @brief
   */
  Eigen::Vector3d acceleration(const Engine::Particle &particle);

  /**
   * @brief
   */
  void limit(Engine::Particle &, const bool &);

  /**
   * @brief
   */
  void calcSystemPotentialEnergy(const double&, const double&);

  /**
   * @brief
   */
  double getSystemPotentialEnergy();

  /**
   * @brief
   */
  double getSystemKineticEnergy();

  /**
   * @brief
   */
  void update();
 private:
  /**
   * @brief
   */
  const nlohmann::json configs;

  /**
   * @brief
   */
  double systemPotentialEnergy_ = 0;

  /**
   * @brief
   */
  const size_t kSeed_;

  /**
   * @brief
   */
  const double dt_;

  /**
   * @brief
   */
  const int threads_;

  /**
   * @brief   Number of particles
   */
  const size_t kCount_;

  /**
   * @brief     Size of simulation box
   */
  const size_t kWSize_;

  /**
   * @brief     Max velocity projection
   */
  const double kVelocity_;
  
  /**
   * @brief   Mass of all particles
   */
  const double kmass_; 

  /**
   * @brief LJ constant
   */
  const double sigma_;

  /**
   * @brief LJ constant
   */
  const double epsilon_;

  /**
   * @brief LJ constant
   */
  const double klim_;

  const double k1_ = 4 * epsilon_ * std::pow(sigma_, 12);
  const double k2_ = 4 * epsilon_ * std::pow(sigma_, 6);

  /**
   * @brief Passed sumulation time
   */
  double time_ = 0;
};
#endif // ENGINE_H
