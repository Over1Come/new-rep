#include "LowPassFilter.h"

#include <cmath>

LowPassFilter::LowPassFilter(float dt, float rc)
    : alpha_(1.0f),
      filteredValue_(0.0f),
      isFirstValue_(true) {
    if (rc > 0.0f) {
        alpha_ = 1.0f - expf(-dt / rc);
    }
}

float LowPassFilter::Update(float rawValue) {
    if (isFirstValue_) {
        filteredValue_ = rawValue;
        isFirstValue_ = false;
    } else {
        filteredValue_ += (rawValue - filteredValue_) * alpha_;
    }

    return filteredValue_;
}
