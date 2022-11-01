#ifndef CONSTANTS_H
#define CONSTANTS_H

const size_t kSeed_ = 130000001;
const double dt_ = 0.001;
const size_t kCount_ = 100;
const size_t kWSide_ = 500;
const double kVelocity_ = 500.;

const double kmass_ = 0.0000001;

// LJ constants
const double sigma_ = 1.3;
const double epsilon_ = 5;

const double klim_ = 50;

const double k1_ = 4*epsilon_*std::pow(sigma_, 12);
const double k2_ = 4*epsilon_*std::pow(sigma_, 6);

#endif // CONSTANTS_H
