#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "rotary_encoder.h"

typedef struct _m_tsEdgePattern
{
    bool bAHigh;
    bool bBHigh;
} m_tsEdgePattern;

m_tsEdgePattern m_sEdgePattern;

void (*m_pvUpCallback)(void);// = null;
void (*m_pvDownCallback)(void);// = null;
uint8_t m_u8AHandle;
uint8_t m_u8BHandle;

bool ROTARY_ENCODER_bInit(void* pvUpCallback, void* pvDownCallback)
{
    bool bResult = false;
    if (GPIO_bRegisterInputPin(&m_u8AHandle) && GPIO_bRegisterInputPin(&m_u8BHandle))
    {
        bResult = true;
    }

    m_sEdgePattern.bAHigh = GPIO_bGetInputPinState(m_u8AHandle);
    m_sEdgePattern.bBHigh = GPIO_bGetInputPinState(m_u8BHandle);

    m_pvUpCallback = pvUpCallback;
    m_pvDownCallback = pvDownCallback;

    return bResult;
}

void ROTARY_ENCODER_vPoll(void)
{
    m_tsEdgePattern sPreviousEdgePattern = {
        m_sEdgePattern.bAHigh,
        m_sEdgePattern.bBHigh
    };

    m_sEdgePattern.bAHigh = GPIO_bGetInputPinState(m_u8AHandle);
    m_sEdgePattern.bBHigh = GPIO_bGetInputPinState(m_u8BHandle);

    // Forward patterns
    if (
        (!sPreviousEdgePattern.bAHigh && m_sEdgePattern.bAHigh && !m_sEdgePattern.bBHigh) || // A 0 -> 1, B is 0
        (sPreviousEdgePattern.bAHigh && !m_sEdgePattern.bAHigh && m_sEdgePattern.bBHigh)  || // A 1 -> 0, B is 1
        (!sPreviousEdgePattern.bBHigh && m_sEdgePattern.bBHigh && m_sEdgePattern.bAHigh)  || // B 0 -> 1, A is 1
        (sPreviousEdgePattern.bBHigh && !m_sEdgePattern.bBHigh && !m_sEdgePattern.bAHigh)    // B 1 -> 0, A is 0
    )
    {
        m_pvUpCallback();
    }
    // Backwards patterns
    else if (
        (!sPreviousEdgePattern.bAHigh && m_sEdgePattern.bAHigh && m_sEdgePattern.bBHigh)  || // A 0 -> 1, B is 1
        (sPreviousEdgePattern.bAHigh && !m_sEdgePattern.bAHigh && !m_sEdgePattern.bBHigh) || // A 1 -> 0, B is 0
        (!sPreviousEdgePattern.bBHigh && m_sEdgePattern.bBHigh && !m_sEdgePattern.bAHigh) || // B 0 -> 1, A is 0
        (sPreviousEdgePattern.bBHigh && !m_sEdgePattern.bBHigh && m_sEdgePattern.bAHigh)     // B 1 -> 0, A is 1
    )
    {
        m_pvDownCallback();
    }
    // No update
    else if ((sPreviousEdgePattern.bAHigh == m_sEdgePattern.bAHigh) &&
        (sPreviousEdgePattern.bBHigh == m_sEdgePattern.bBHigh))
    {

    }
    // We might have missed an edge. May need a reset
    else
    {
        printf("Unknown edge pattern\n");
    }
    /*
    if ((sPreviousEdgePattern.bAHigh != m_sEdgePattern.bAHigh) ||
        (sPreviousEdgePattern.bBHigh != m_sEdgePattern.bBHigh))
    {
        printf("A %d to %d, B %d to %d\n",
            sPreviousEdgePattern.bAHigh,
            m_sEdgePattern.bAHigh,
            sPreviousEdgePattern.bBHigh,
            m_sEdgePattern.bBHigh);
    }
    */
}
