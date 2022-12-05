#include "engine.h"

Engine::Engine(const nlohmann::json &configs) : configs(configs),
                                                kSeed_(configs["seed"]),
                                                dt_(configs["dt"]),
                                                threads_(configs["num_threads"]),
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

  double step = kWSize_ / (std::cbrt(kCount_) + 1);

  int count = static_cast<int>(std::cbrt(kCount_));

  double z = step;
  for (size_t i = 0; i != count; ++i) {
    double y = step;
    for (size_t k = 0; k != count; ++k) {
      double x = step;
      for (size_t m = 0; m != count; ++m) {
        particles.emplace_back(Particle(x, y, z, rand_vx(gen), rand_vy(gen),
                                     rand_vz(gen), kmass_));
        x += step;
      }
      y += step;
    }
    z += step;
  }
}

double Engine::getTime() const {
  return time_;
}

Eigen::Vector3d Engine::acceleration(const Engine::Particle &particle) {
  Eigen::Vector3d acceleration_sum(0, 0, 0);
  std::vector<Engine::Particle> particles_copy = particles;

  Eigen::Vector3d shift =
      Eigen::Vector3d(static_cast<double>(kWSize_) / 2,
                      static_cast<double>(kWSize_) / 2,
                      static_cast<double>(kWSize_) / 2) - particle.position;

#pragma omp parallel for num_threads(threads_)
  for (int i = 0; i < particles_copy.size(); ++i) {
    particles_copy[i].position += shift;
  }

#pragma omp parallel for num_threads(threads_)
  for (int i = 0; i < particles_copy.size(); ++i) {
    limit(particles_copy[i]);
  }

  for (auto &ext : particles_copy) {
    Eigen::Vector3d distance = ext.position - (particle.position + shift);
    if (distance.norm() < klim_ && distance.norm() > 0) {

      double part1 = k1_ / std::pow(distance.norm(), 12);
      double part2 = k2_ / std::pow(distance.norm(), 6);

      calcSystemPotentialEnergy(part1, part2);

      double acceleration_abs =
          -(12 * part1 / distance.norm() - 6 * part2 / distance.norm()) / particle.mass;
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

void Engine::calcSystemPotentialEnergy(const double &part1, const double &part2) {
  systemPotentialEnergy_ += part1 - part2;
}

double Engine::getSystemPotentialEnergy() {
  return systemPotentialEnergy_ / 2;
}

double Engine::getSystemKineticEnergy() {
  double sumKineticEnergy = 0;
  for (size_t i = 0; i != kCount_; ++i) {
    sumKineticEnergy += particles[i].getKineticEnegry();
  }
  return sumKineticEnergy;
}

void Engine::update() {
#pragma omp parallel for num_threads(threads_)
  for (int i = 0; i < particles.size(); i++) {
    particles[i].acceleration = acceleration(particles[i]);
  }
  systemPotentialEnergy_ = 0; // this is needed to systemPotentialEnergy_ not to be doubled
#pragma omp parallel for num_threads(threads_)
  for (auto &current : particles) {
    current.position += current.velocity * dt_ + current.acceleration * (dt_ * dt_ / 2); //move
  }
#pragma omp parallel for num_threads(threads_)
  for (auto &current : particles) {
    limit(current);
  }
#pragma omp parallel for num_threads(threads_)
  for (auto &current : particles) {
    current.velocity += current.acceleration * dt_ / 2; //accelerate
  }
#pragma omp parallel for num_threads(threads_)
  for (int i = 0; i < particles.size(); i++) {
    particles[i].acceleration = acceleration(particles[i]);
  }
#pragma omp parallel for num_threads(threads_)
  for (auto &current : particles) {
    current.velocity += current.acceleration * dt_ / 2; //accelerate
  }
  time_ += dt_;
}
