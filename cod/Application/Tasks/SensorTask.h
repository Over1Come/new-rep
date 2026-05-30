#ifndef SENSORTASK_H
#define SENSORTASK_H

#include "Interfaces.h"

void SensorTask_Start(void);
void SensorTask_Start(ISensor& sensor, IFilter& filter, ICalculator& calculator);

#endif
