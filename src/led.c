#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "led.h"

static uint8_t m_u8AHandle;

bool LED_bInit(void)
{
    return GPIO_bRegisterOutputPin(&m_u8AHandle);
}

void LED_vSetLed(bool bOn)
{
    GPIO_vSetOutputPinState(m_u8AHandle, bOn);
}
