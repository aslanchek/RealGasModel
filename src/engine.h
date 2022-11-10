#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <eigen3/Eigen/Dense>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

class Engine {
 public:
  struct Particle {
    double mass;
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    Eigen::Vector3d acceleration;

    Particle(double x, double y, double z, double vx, double vy, double vz, double mass): position(x, y, z), velocity(vx, vy, vz), mass(mass) {};

    bool operator==(const Engine::Particle& other) {
      return this->velocity == other.velocity && this->position == other.position && this->acceleration == other.acceleration && this->mass == other.mass;
    }

    bool operator!=(const Engine::Particle& other) {
      return !(*this==other);
    }
  };
  Engine() = default;
  Engine(nlohmann::json&);

  void update();
  double potential_energy(const Particle &particle);
  Eigen::Vector3d acceleration(const Particle &particle);
  void limit(Particle&);

  std::vector<Particle> particles;

 private:
  const size_t kSeed_;
  const double dt_;
  const size_t kCount_;
  const size_t kWSide_;
  const double kVelocity_;
  const double kmass_;
  const double klim_;

  // LJ constants
  const double sigma_;
  const double epsilon_;

  const double k1_ = 4 * epsilon_ * std::pow(sigma_, 12);
  const double k2_ = 4 * epsilon_ * std::pow(sigma_, 6);
};
#endif // ENGINE_H