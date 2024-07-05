#include "random.h"

float rng(float min, float max) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> dist(min, max);

    return dist(generator);
}
