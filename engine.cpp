#include "engine.h"

Engine::Engine() {
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
      particles.push_back(Particle(x, y, 0, rand_vx(gen), rand_vy(gen),
                                     0 /*rand_vz(gen)*/, kmass_));
      x += step;
    }
    y += step;
  }
  //particles.push_back(Particle(10, 10, 0, 40, 40, 0, kmass_));
  //particles.push_back(Particle(300, 300, 0, 0, -40, 0, kmass_));
}

void Engine::limit(Eigen::Vector3d& v) {
    v[0] = std::fmod(v[0] + kWSide_, kWSide_);
    v[1] = std::fmod(v[1] + kWSide_, kWSide_);
    v[2] = std::fmod(v[2] + kWSide_, kWSide_);
}

Eigen::Vector3d Engine::acceleration(const Particle& particle) {
  Eigen::Vector3d acceleration_sum(0,0,0);
  for(auto ext : particles) {
      if (ext != particle) {
        Eigen::Vector3d distance = ext.position - particle.position;
        if (distance.norm() ) {
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
    limit(current.position);
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
    limit(current.position);
  }
}

void Engine::render(QPainter &painter) {
    for(auto curr : particles) {
        double radius = 5;
        double lim = sigma_*2.5;
        painter.drawEllipse(curr.position[0] - radius/2, curr.position[1] - radius/2, 5, 5);
        painter.drawEllipse(curr.position[0] - lim/2, curr.position[1] - lim/2, lim, lim);
    }
}
