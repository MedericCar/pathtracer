#include <random>

#include "random.hh"

namespace isim {

float random_float (float min, float max) {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(mt);
}

}