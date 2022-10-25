#include "engine.h"


Engine::Engine()
{

    std::mt19937 gen(kSeed); // генератор случайных чисел, тут взят какой-то алгоритм mt19937
    std::uniform_real_distribution rand_x(0., kWidth); // uniform distribution - равномерное распределение на R, по умолчанию стоит double
    std::uniform_real_distribution rand_y(0., kHeight);
    std::uniform_real_distribution rand_vx(0., kVelocity);
    std::uniform_real_distribution rand_vy(0., kVelocity);

    for(size_t k = 0; k != kCount; ++k)
        particles.push_back(Particle(rand_x(gen), rand_y(gen), rand_vx(gen), rand_vy(gen), kmass));

    //particles.push_back(Particle(10, 10, 50, 50, kmass));
    //particles.push_back(Particle(500, 500, -50, -50, kmass));

}

QVector3D Engine::force(Particle &particle) {
    QVector3D force_sum;
    for(auto ext : particles) {
        QVector3D distance = ext.p - particle.p;
        if (particle != ext && distance.length() <= 100*sigma) {
            double force_abs = -12*k1/std::pow(distance.length(), 13) - 6*k1/std::pow(distance.length(), 7);
            //std::cout << (particles[0].v * kmass + particles[1].v * kmass).length()<< '\n';
            force_sum += distance.normalized() * force_abs;
        }
    }
    return force_sum;
}

void Engine::update() {
    std::vector<Particle> particles_tmp;
    double time = 0;

    for(auto curr : particles) {
        QVector3D force_sum = force(curr);

        QVector3D acceleration = force_sum/curr.mass;
        curr.p += curr.v * dt + acceleration * dt*dt/2;
        curr.v += acceleration * dt;

        particles_tmp.push_back(curr);
    }
    particles = std::move(particles_tmp);
}

void Engine::render(QPainter &painter) {
    for(auto curr : particles) {
        painter.drawEllipse(curr.p.x(), curr.p.y(), 5, 5);
    }
}
