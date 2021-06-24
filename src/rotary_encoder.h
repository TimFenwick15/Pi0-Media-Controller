#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

bool ROTARY_ENCODER_bInit(void* pvUpCallback, void* pvDownCallback);
void ROTARY_ENCODER_vPoll(void);

#endif /* ROTARY_ENCODER_H */
