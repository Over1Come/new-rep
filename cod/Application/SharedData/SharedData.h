#ifndef SHAREDDATA_H
#define SHAREDDATA_H

struct SharedData
{
    volatile float temperature;
    volatile float speed;
    volatile bool new_data;
};

extern SharedData g_data;

#endif