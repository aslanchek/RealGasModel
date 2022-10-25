#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <QVector2D>
#include <QVector3D>
#include <QPainter>

#include <random>
#include <vector>


class Engine
{
public:
    struct Particle {
      double mass;
      QVector3D v; // velocity of particle
      QVector3D p;

      Particle(double x, double y, double vx, double vy, double mass): p(x, y, 0), v(vx, vy, 0), mass(mass) {}

      bool operator==(const Particle& other) {
        return this->v == other.v && this->p == other.p && this->mass == other.mass;
      }

      bool operator!=(const Particle& other) {
        return !(*this==other);
      }
    };

    Engine();

    void update();
    void render(QPainter &);
    QVector3D force(Particle &particle);

private:
    const size_t kSeed = 120000000;
    const double dt = 0.01;
    const size_t kCount = 100;
    const double kWidth = 1000.;
    const double kHeight = 1000.;
    const double kVelocity = 50.;

    const double kmass = 0.0000001;
    const double sigma = 1.5;
    const double epsilon = 5;

    const double k1 = 4*epsilon*std::pow(sigma, 12);
    const double k2 = 4*epsilon*std::pow(sigma, 6);

    std::vector<Particle> particles;

};

#endif // ENGINE_H
