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
  double acc_sum_x = 0;
  double acc_sum_y = 0;
  double acc_sum_z = 0;

  std::vector<Engine::Particle> particles_copy = particles_;

  Eigen::Vector3d shift =
      Eigen::Vector3d(static_cast<double>(kBoxSize) / 2,
                      static_cast<double>(kBoxSize) / 2,
                      static_cast<double>(kBoxSize) / 2) - particle.position;

  #pragma omp parallel for num_threads(kThreads) shared(particles_copy, shift)
  for (int i = 0; i < particles_copy.size(); ++i) {
    particles_copy[i].position += shift;
  }

  #pragma omp parallel for num_threads(kThreads) shared(particles_copy)
  for (int i = 0; i < particles_copy.size(); ++i) {
    PeriodicBoundaryCondLimit(particles_copy[i], false);
  }

  #pragma omp parallel for num_threads(kThreads) shared(particles_copy, particle) reduction(+: acc_sum_x, acc_sum_y, acc_sum_z) 
  for (int i = 0; i < particles_copy.size(); ++i) {
    Eigen::Vector3d distance = particles_copy[i].position - (particle.position + shift);                      // расстояние между частицами
    if (distance.norm() < kPotentialCut && distance.norm() > 0) {                                             // проверка обрезания потенциала

      double acceleration_abs =
          -(12 * k1 / std::pow(distance.norm(), 13) - 6 * k2 / std::pow(distance.norm(), 7)) / particle.mass; // вычисление ускорения 

      distance.normalize();
      Eigen::Vector3d acc_sum_vec = distance * acceleration_abs;
      acc_sum_x += acc_sum_vec[0];
      acc_sum_y += acc_sum_vec[1];
      acc_sum_z += acc_sum_vec[2];
    }
  }

  Eigen::Vector3d acceleration_sum(acc_sum_x, acc_sum_y, acc_sum_z);
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
  //TODO: сделать пробег по всем частицам и посчитать суммарную энергию взаимодействия
  //      можно использовать OpenMP pragma, но нужно не забыть про data races
  return 0;
}

double Engine::GetSystemKineticEnergy() {
  double sumKineticEnergy = 0;
  for (size_t i = 0; i != kCount; ++i) {
    sumKineticEnergy += particles_[i].GetKineticEnergy();
  }
  return sumKineticEnergy;
}

void Engine::Update() {
  // calculates acceleration for each particle at the first time
  if (time_ == 0) {
    std::cout << "init acceleration calc\n";
    for (int i = 0; i < particles_.size(); ++i) {
      particles_[i].acceleration = CalcAcceleration(particles_[i]);
    }
  }

  // updates position for each particle
  for (int i = 0; i < particles_.size(); ++i) {
    particles_[i].position += particles_[i].velocity * dt + particles_[i].acceleration * (dt * dt / 2); //move
  }

  // checks if partile has not gone beyond the boundaries
  for (int i = 0; i < particles_.size(); ++i) {
    PeriodicBoundaryCondLimit(particles_[i], true);
  }

  // updates velocity for each particle
  for (int i = 0; i < particles_.size(); ++i) {
    particles_[i].velocity += particles_[i].acceleration * dt / 2;
  }

  // updates acceleration for each particle
  for (int i = 0; i < particles_.size(); ++i) {
    particles_[i].acceleration = CalcAcceleration(particles_[i]);
  }

  // updates velocity for each particle
  for (int i = 0; i < particles_.size(); ++i) {
    particles_[i].velocity += particles_[i].acceleration * dt / 2;
  }

  time_ += dt;
}
