#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include "Interfaces.h"

class LowPassFilter : public IFilter {
public:
    LowPassFilter(float dt, float rc);

    float Update(float rawValue) override;

private:
    float alpha_;
    float filteredValue_;
    bool isFirstValue_;
};

#endif
