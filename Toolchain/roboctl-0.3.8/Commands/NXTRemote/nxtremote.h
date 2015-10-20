/* Robot parameters */
#define STEER_MOTOR_PORT        2
#define DRIVE_MOTOR_PORT        0
#define IMPLEMENT_MOTOR_PORT    1

#ifndef ABS
#define ABS(x) ((x) > 0 ? (x) : -(x))
#endif

typedef struct
{
    char    *device;
    double  wheel_power;
    int     implement_power;
    int     button;
}   settings_t;

#include "protos.h"

