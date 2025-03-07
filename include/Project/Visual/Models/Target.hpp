#pragma once

class Target
{
    // "detection_point": [0, 0],
    // "root-mean-square_error": 99999,
    // "courses": [360, 360],
    double currentVelocity;
    //   "max": 99.99,
    //   "min_noise-reduced": 99.99,
    //   "max_noise-reduced": 99.99
    // "obsolescence_time": 999999,
    // "avoidance_distance": 999999

public:
    void setCurrentVelocity(const double velocity)
    {
        currentVelocity = velocity;
    }

    double getCurrentVelocity() const
    {
        return currentVelocity;
    }
};
