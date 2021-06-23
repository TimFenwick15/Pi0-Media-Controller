#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <stdbool.h>
#include "rotary_encoder.h"

typedef struct _m_tsEdgePattern
{
    bool bAHigh;
    bool bBHigh;
    bool bCHigh;
} m_tsEdgePattern;

ROTARY_ENCODER_tsData m_sData;
m_tsEdgePattern m_sEdgePattern;
uint32_t m_u32PinA;
uint32_t m_u32PinB;
uint32_t m_u32PinC;

void ROTARY_ENCODER_vInit(uint32_t pinA, uint32_t pinB, uint32_t pinC)
{
    m_sData.buttonPress = false;
    m_sData.positionChange = 0;
    m_sEdgePattern.bAHigh = false;
    m_sEdgePattern.bBHigh = false;
    m_sEdgePattern.bCHigh = false;

    m_u32PinA = pinA;
    m_u32PinB = pinB;
    m_u32PinC = pinC;

    bcm2835_gpio_fsel(pinA, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(pinA, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(pinB, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(pinB, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(pinC, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(pinC, BCM2835_GPIO_PUD_DOWN);
}

ROTARY_ENCODER_tsData ROTARY_ENCODER_sGetData(void)
{
    m_tsEdgePattern sPreviousEdgePattern = {
        m_sEdgePattern.bAHigh,
        m_sEdgePattern.bBHigh,
        m_sEdgePattern.bCHigh,
    };

    m_sEdgePattern.bAHigh = (bool)bcm2835_gpio_lev(m_u32PinA);
    m_sEdgePattern.bBHigh = (bool)bcm2835_gpio_lev(m_u32PinB);
    m_sEdgePattern.bCHigh = (bool)bcm2835_gpio_lev(m_u32PinC);

    if (m_sEdgePattern.bAHigh != sPreviousEdgePattern.bAHigh)
    {
        printf("A edge %d\n", m_sEdgePattern.bAHigh);
    }
    if (m_sEdgePattern.bBHigh != sPreviousEdgePattern.bBHigh)
    {
        printf("B edge %d\n", m_sEdgePattern.bAHigh);
    }
    if (m_sEdgePattern.bCHigh != sPreviousEdgePattern.bCHigh)
    {
        printf("C edge %d\n", m_sEdgePattern.bAHigh);
    }
    return m_sData;
}

