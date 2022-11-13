#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <nlohmann/json.hpp>
#include <eigen3/Eigen/Dense>
#include <cstdlib>
#include <algorithm>

#include <omp.h>


class Engine {
 public:
  struct Particle {
    double mass;
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    Eigen::Vector3d acceleration;

    Particle(double x, double y, double z, double vx, double vy, double vz, double mass) :
        position(x, y, z), velocity(vx, vy, vz), mass(mass) {};

    bool operator==(const Engine::Particle &other) {
      return this->velocity == other.velocity &&
          this->position == other.position && this->acceleration == other.acceleration && this->mass == other.mass;
    }

    bool operator!=(const Engine::Particle &other) {
      return !(*this == other);
    }

    double getKineticEnegry() {
      return mass * std::pow(velocity.norm(), 2) / 2;
    }
  };
  Engine() = default;
  Engine(const nlohmann::json &);

  std::vector<Particle> particles;

  double getTime();

  Eigen::Vector3d acceleration(const Engine::Particle &particle);
  void limit(Engine::Particle &);
  double getSystemPotentialEnergy();
  double getSystemKineticEnergy();
  void update();
 private:
  const nlohmann::json configs;
  const size_t kSeed_;
  const double dt_;
  const size_t kCount_;
  const size_t kWSize_;
  const double kVelocity_;
  const double kmass_;

  // LJ constants
  const double sigma_;
  const double epsilon_;
  const double klim_;

  const double k1_ = 4 * epsilon_ * std::pow(sigma_, 12);
  const double k2_ = 4 * epsilon_ * std::pow(sigma_, 6);

  double time_ = 0;
};
#endif // ENGINE_H