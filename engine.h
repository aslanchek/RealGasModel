#ifndef ENGINE_H
#define ENGINE_H

#include <QPainter>
#include <iostream>
#include <vector>
#include <random>
#include <eigen3/Eigen/Dense>
#include <random>
#include <vector>

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

  Engine();

  void update();
  double potential_energy(const Particle &particle);
  Eigen::Vector3d acceleration(const Particle &particle);
  void render(QPainter &);
  void limit(Eigen::Vector3d&);

 private:
  std::mt19937 gen();
  std::uniform_real_distribution<double> rand_vx;
  std::uniform_real_distribution<double> rand_vy;
  std::uniform_real_distribution<double> rand_vz;

  #include "constants.inc"

  std::vector<Particle> particles;
};


#endif // ENGINE_H
