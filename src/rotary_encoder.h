#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

typedef struct _ROTARY_ENCODER_tsData
{
    bool buttonPress;
    int16_t positionChange;
} ROTARY_ENCODER_tsData;

void ROTARY_ENCODER_vInit(uint32_t pinA, uint32_t pinB, uint32_t pinC);

ROTARY_ENCODER_tsData ROTARY_ENCODER_sGetData(void);

#endif /* ROTARY_ENCODER_H */
