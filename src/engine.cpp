#include "engine.h"

Engine::Engine(nlohmann::json& configs): kSeed_(configs["seed"]), dt_(configs["dt"]), kCount_(configs["count"]),
                                          kWSide_(configs["side_size"]), kVelocity_(configs["velocity"]),
                                          kmass_(configs["mass"]), sigma_(configs["sigma"]), epsilon_(configs["epsilon"]), klim_(configs["potential_limit"])
{
  std::mt19937 gen(kSeed_); // генератор случайных чисел, тут взят какой-то алгоритм mt19937
  std::uniform_real_distribution<double> rand_vx(-kVelocity_, kVelocity_); // uniform distribution - равномерное распределение на R, по умолчанию стоит double
  std::uniform_real_distribution<double> rand_vy(-kVelocity_, kVelocity_);
  std::uniform_real_distribution<double> rand_vz(-kVelocity_, kVelocity_);

  double step = kWSide_ / (std::sqrt(kCount_)+1);

  int count = static_cast<int>(std::sqrt(kCount_));

  double y = step;
  for (size_t k = 0; k != count; ++k) {
    double x = step;
    for (size_t m = 0; m != count; ++m) {
      particles.emplace_back(Particle(x, y, 0, rand_vx(gen), rand_vy(gen),
                                     0, kmass_));
      x += step;
    }
    y += step;
  }
}

void Engine::limit(Particle& particle) {
  if (particle.position[0] > kWSide_ || particle.position[0] < 0)
    particle.velocity[0] *= -1;
  if (particle.position[1] > kWSide_ || particle.position[1] < 0)
    particle.velocity[1] *= -1;
}

Eigen::Vector3d Engine::acceleration(const Particle& particle) {
  Eigen::Vector3d acceleration_sum(0,0,0);
  for(auto ext : particles) {
    if (ext != particle) {
      Eigen::Vector3d distance = ext.position - particle.position;
      if (distance.norm() < klim_) {
        double acceleration_abs = (- 12*k1_/std::pow(distance.norm(), 13) - 6*k1_/std::pow(distance.norm(), 7)) / particle.mass;
        distance.normalize();
        acceleration_sum += distance * acceleration_abs;
      }
    }
  }
  return acceleration_sum;
}

void Engine::update() {
  for(auto& current : particles) {
    current.acceleration = acceleration(current);
  }
  for(auto& current : particles) {
    current.position += current.velocity * dt_/2; //move
  }
  for(auto& current : particles) {
    limit(current);
  }
  for(auto& current : particles) {
    current.velocity += current.acceleration * dt_/2; //accelerate
  }
  for(auto& current : particles) {
    current.acceleration = acceleration(current); //reestimate acceleration
  }
  for(auto& current : particles) {
    current.velocity += current.acceleration * dt_/2; //accelerate
  }
  for(auto& current : particles) {
    current.position += current.velocity * dt_/2; //move
  }
  for(auto& current : particles) {
    limit(current);
  }
}