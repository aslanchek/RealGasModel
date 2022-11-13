#include "engine.h"

Engine::Engine(const nlohmann::json &configs) : configs(configs),
                                                kSeed_(configs["seed"]),
                                                dt_(configs["dt"]),
                                                kCount_(configs["count"]),
                                                kWSize_(configs["simulation_side_size"]),
                                                kVelocity_(configs["velocity"]),
                                                kmass_(configs["mass"]),
                                                sigma_(configs["sigma"]),
                                                epsilon_(configs["epsilon"]),
                                                klim_(configs["potential_limit"]) {
  std::mt19937 gen(kSeed_);
  std::uniform_real_distribution<double> rand_vx(-kVelocity_, kVelocity_);
  std::uniform_real_distribution<double> rand_vy(-kVelocity_, kVelocity_);
  std::uniform_real_distribution<double> rand_vz(-kVelocity_, kVelocity_);

  double step = static_cast<double>(kWSize_) / (std::sqrt(kCount_) + 1);

  int count = static_cast<int>(std::sqrt(kCount_));

  double y = step;
  for (size_t k = 0; k != count; ++k) {
    double x = step;
    for (size_t m = 0; m != count; ++m) {
      particles.emplace_back(Particle(x, y, 0, rand_vx(gen), rand_vy(gen), 0, kmass_));
      x += step;
    }
    y += step;
  }
}

double Engine::getTime() {
  return time_;
}

Eigen::Vector3d Engine::acceleration(const Engine::Particle &particle) {
  Eigen::Vector3d acceleration_sum(0, 0, 0);
  std::vector<Engine::Particle> particles_copy = particles;

  Eigen::Vector3d shift =
      Eigen::Vector3d(static_cast<double>(kWSize_) / 2, static_cast<double>(kWSize_) / 2, 0) - particle.position;

#pragma omp parallel for
  for (int i = 0; i < particles_copy.size(); i++) {
    particles_copy[i].position += shift;
  }

#pragma omp parallel for
  for (int i = 0; i < particles_copy.size(); i++) {
    limit(particles_copy[i]);
  }

  for (auto &ext : particles_copy) {
    Eigen::Vector3d distance = ext.position - (particle.position + shift);
    if (distance.norm() < klim_ && distance.norm() > 0) {
      double acceleration_abs =
          -(12 * k1_ / std::pow(distance.norm(), 13) - 6 * k2_ / std::pow(distance.norm(), 7)) / particle.mass;
      distance.normalize();
      acceleration_sum += distance * acceleration_abs;
    }
  }
  return acceleration_sum;
}

void Engine::limit(Engine::Particle &particle) {
  for (int i = 0; i != 3; ++i) {
    particle.position[i] = std::abs(std::fmod(particle.position[i] + static_cast<double>(kWSize_), kWSize_));
  }
}

double Engine::getSystemKineticEnergy() {
  double sumKineticEnergy = 0;
  for (size_t i = 0; i != kCount_; ++i) {
    sumKineticEnergy += particles[i].getKineticEnegry();
  }
  return sumKineticEnergy;
}

void Engine::update() {

#pragma omp parallel for
  for (int i = 0; i < particles.size(); i++) {
    particles[i].acceleration = acceleration(particles[i]);
  }
  for (auto &current : particles) {
    current.position += current.velocity * dt_ + current.acceleration * (dt_ * dt_ / 2); //move
  }
  for (auto &current : particles) {
    limit(current);
  }
  for (auto &current : particles) {
    current.velocity += current.acceleration * dt_ / 2; //accelerate
  }
#pragma omp parallel for
  for (int i = 0; i < particles.size(); i++) {
    particles[i].acceleration = acceleration(particles[i]);
  }
  for (auto &current : particles) {
    current.velocity += current.acceleration * dt_ / 2; //accelerate
  }
  time_ += dt_;
}
