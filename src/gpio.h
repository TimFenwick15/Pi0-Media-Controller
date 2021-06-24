#ifndef GPIO_H
#define GPIO_H

bool GPIO_bRegisterInputPin(uint8_t* pu8Handle);
bool GPIO_bRegisterOutputPin(uint8_t* pu8Handle);
bool GPIO_bGetInputPinState(uint8_t u8Handle);
void GPIO_vSetOutputPinState(uint8_t u8Handle, bool bOn);

#endif
