#ifndef LEDINDICATOR_H
#define LEDINDICATOR_H

#include "Interfaces.h"

class LEDIndicator : public ILedIndicator {
public:
    void Init(void) override;
    void Update(float speed) override;
};

#endif
