#include "engine.h"

Engine::Engine(const nlohmann::json &configs) : configs(configs) {
  std::mt19937 gen(kSeed);
  std::uniform_real_distribution<double> rand_vx(-kVelocity, kVelocity);
  std::uniform_real_distribution<double> rand_vy(-kVelocity, kVelocity);
  std::uniform_real_distribution<double> rand_vz(-kVelocity, kVelocity);

  double step = kBoxSize / (std::cbrt(kCount) + 1);

  int count = static_cast<int>(std::cbrt(kCount));

  double z = step;
  for (size_t i = 0; i != count; ++i) {
    double y = step;
    for (size_t k = 0; k != count; ++k) {
      double x = step;
      for (size_t m = 0; m != count; ++m) {
        particles_.push_back(Particle(x, y, z, rand_vx(gen), rand_vy(gen),
                                         rand_vz(gen), kMass));
        x += step;
      }
      y += step;
    }
    z += step;
  }
  int a = 0;
}

double Engine::GetTime() const {
  return time_;
}

Eigen::Vector3d Engine::CalcAcceleration(const Engine::Particle &particle) {
  Eigen::Vector3d acceleration_sum(0, 0, 0);
  std::vector<Engine::Particle> particles_copy = particles_;

  Eigen::Vector3d shift =
      Eigen::Vector3d(static_cast<double>(kBoxSize) / 2,
                      static_cast<double>(kBoxSize) / 2,
                      static_cast<double>(kBoxSize) / 2) - particle.position;

  for (int i = 0; i < particles_copy.size(); ++i) {
    particles_copy[i].position += shift;
  }

  for (int i = 0; i < particles_copy.size(); ++i) {
    PeriodicBoundaryCondLimit(particles_copy[i], false);
  }

  for (auto &ext : particles_copy) {
    Eigen::Vector3d distance = ext.position - (particle.position + shift);
    if (distance.norm() < kPotentialCut && distance.norm() > 0) {

      double part1 = k1 / std::pow(distance.norm(), 12);
      double part2 = k2 / std::pow(distance.norm(), 6);

      CalcSystemPotentialEnergy(part1, part2);

      double acceleration_abs =
          -(12 * part1 / distance.norm() - 6 * part2 / distance.norm()) / particle.mass;
      distance.normalize();
      acceleration_sum += distance * acceleration_abs;
    }
  }
  return acceleration_sum;
}

void Engine::PeriodicBoundaryCondLimit(Engine::Particle &particle, const bool &transit_check) {
  for (int i = 0; i != 3; ++i) {
    if (particle.position[i] > kBoxSize) {
      particle.position[i] -= static_cast<double>(kBoxSize);
      if (transit_check) {
        particle.transit[i] += 1;
      }
    }
    if (particle.position[i] < 0) {
      particle.position[i] += static_cast<double>(kBoxSize);
      if (transit_check) {
        particle.transit[i] -= 1;
      }
    }
  }
}

void Engine::CalcSystemPotentialEnergy(const double &part1, const double &part2) {
  system_potential_energy_ += part1 - part2;
}

double Engine::GetSystemPotentialEnergy() {
  return system_potential_energy_ / 2;
}

double Engine::GetSystemKineticEnergy() {
  double sumKineticEnergy = 0;
  for (size_t i = 0; i != kCount; ++i) {
    sumKineticEnergy += particles_[i].GetKineticEnergy();
  }
  return sumKineticEnergy;
}

void Engine::Update() {
  if (time_ == 0) {
    std::cout << "init acceleration calc\n";
    for (int i = 0; i < particles_.size(); ++i) {
      particles_[i].acceleration = CalcAcceleration(particles_[i]);
    }
  }

  system_potential_energy_ = 0; // this is needed to system potential energy not to be doubled

  for (int i = 0; i < particles_.size(); ++i) {
    particles_[i].position += particles_[i].velocity * dt + particles_[i].acceleration * (dt * dt / 2); //move
  }

  for (int i = 0; i < particles_.size(); ++i) {
    PeriodicBoundaryCondLimit(particles_[i], true);
  }

  for (int i = 0; i < particles_.size(); ++i) {
    particles_[i].velocity += particles_[i].acceleration * dt / 2; //accelerate
  }

  for (int i = 0; i < particles_.size(); ++i) {
    particles_[i].acceleration = CalcAcceleration(particles_[i]);
  }

  for (int i = 0; i < particles_.size(); ++i) {
    particles_[i].velocity += particles_[i].acceleration * dt / 2; //accelerate
  }

  time_ += dt;
}
