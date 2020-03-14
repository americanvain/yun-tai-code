#include "ramp.h"

void RampInit(RampGen_t *ramp, int32_t XSCALE) {
    ramp->XSCALE = XSCALE;
}

float RampCalc(RampGen_t *ramp) {
    ramp->count++;
    if (ramp->count >= ramp->XSCALE)
        ramp->count = ramp->XSCALE;
    ramp->out = (float)ramp->count / (float)ramp->XSCALE;
    return ramp->out;
}

void RampSetCounter(struct RampGen_t *ramp, int32_t count) {
    ramp->count = count;
}

void RampResetCounter(struct RampGen_t *ramp) {
    ramp->count = 0;
}


void RampSetScale(struct RampGen_t *ramp, int32_t scale) {
    ramp->XSCALE = scale;
}

uint8_t RampIsOverflow(struct RampGen_t *ramp) {
    if (ramp->count >= ramp->XSCALE)
        return 1;
    else
        return 0;
}
