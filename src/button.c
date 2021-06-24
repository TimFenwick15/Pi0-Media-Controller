#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "button.h"

#define NUM_OF_BUTTONS_LIMIT 9 // This is a hack - get this from GPIO?

typedef struct _m_tsData
{
    bool bPressed;
    uint8_t u8Handle;
    void (*pvCallback)(void);
} m_tsData;

static m_tsData m_sData[NUM_OF_BUTTONS_LIMIT];
static uint8_t m_u8NumberOfButtons = 0;

bool BUTTON_bInit(void* pvPressedCallback)
{
    bool bResult = false;
    if (m_u8NumberOfButtons < NUM_OF_BUTTONS_LIMIT)
    {
        if (GPIO_bRegisterInputPin(&m_sData[m_u8NumberOfButtons].u8Handle))
        {
            bResult = true;
            m_sData[m_u8NumberOfButtons].pvCallback = pvPressedCallback;
            m_u8NumberOfButtons++;
        }
    }
    return bResult;
}

void BUTTON_vPoll(void)
{
    uint8_t u8Index = 0;
    bool bPreviouslyPressed;
    for (u8Index = 0; u8Index < m_u8NumberOfButtons; u8Index++)
    {
        bPreviouslyPressed = m_sData[u8Index].bPressed;
        m_sData[u8Index].bPressed = GPIO_bGetInputPinState(m_sData[u8Index].u8Handle);
        if (!bPreviouslyPressed && m_sData[u8Index].bPressed)
        {
            m_sData[u8Index].pvCallback();
        }
    }
}
