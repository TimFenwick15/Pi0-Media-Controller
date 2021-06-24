#include <stdlib.h>
#include <stdbool.h>
#include <bcm2835.h> // Imports GPIO functions
#include "gpio.h"

#define NUM_OF_INPUT_PINS  (11)
#define NUM_OF_OUTPUT_PINS (1)

static bool m_bInitialiseGPIO(void);

/*
 * GPIO 2 and 3 have fixed pull up resistors.
 */
static uint32_t m_u32InputPins[NUM_OF_INPUT_PINS] = {
    RPI_BPLUS_GPIO_J8_07, // Pin 07 GPIO4, not GPIO7 in this numbering
    RPI_BPLUS_GPIO_J8_29, // Pin 29 GPIO5
    RPI_BPLUS_GPIO_J8_31, // Pin 31 GPIO6
    RPI_BPLUS_GPIO_J8_26, // Pin 26 GPIO7
    RPI_BPLUS_GPIO_J8_24, // Pin 24 GPIO8
    RPI_BPLUS_GPIO_J8_21, // Pin 21 GPIO9
    RPI_BPLUS_GPIO_J8_19, // Pin 19 GPIO10
    RPI_BPLUS_GPIO_J8_23, // Pin 23 GPIO11
    RPI_BPLUS_GPIO_J8_32, // Pin 32 GPIO12
    RPI_BPLUS_GPIO_J8_33, // Pin 33 GPIO13
    RPI_BPLUS_GPIO_J8_36, // Pin 36 GPIO16
};

static uint32_t m_u32OutputPins[NUM_OF_OUTPUT_PINS] = {
    RPI_BPLUS_GPIO_J8_11, // Pin 11 GPIO17
};

static uint8_t m_u8InputHandle = 0;
static uint8_t m_u8OutputHandle = 0;
static bool    m_bBCM2835Initialised = false;

bool GPIO_bRegisterInputPin(uint8_t* pu8Handle)
{
    bool bResult = false;

    if (m_bInitialiseGPIO() && (m_u8InputHandle < NUM_OF_INPUT_PINS))
    {
        bResult = true;
        *pu8Handle = m_u8InputHandle;
        bcm2835_gpio_fsel(m_u32InputPins[m_u8InputHandle], BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(m_u32InputPins[m_u8InputHandle], BCM2835_GPIO_PUD_DOWN);
        m_u8InputHandle++;
    }
    return bResult;
}

bool GPIO_bRegisterOutputPin(uint8_t* pu8Handle)
{
    bool bResult = false;

    if (m_bInitialiseGPIO() && (m_u8OutputHandle < NUM_OF_OUTPUT_PINS))
    {
        bResult = true;
        *pu8Handle = m_u8OutputHandle;
        bcm2835_gpio_fsel(m_u32OutputPins[m_u8OutputHandle], BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_set_pud(m_u32OutputPins[m_u8OutputHandle], BCM2835_GPIO_PUD_DOWN);
        m_u8OutputHandle++;
    }
    return bResult;
}

bool GPIO_bGetInputPinState(uint8_t u8Handle)
{
    return (bool) bcm2835_gpio_lev(m_u32InputPins[u8Handle]);
}

void GPIO_vSetOutputPinState(uint8_t u8Handle, bool bOn)
{
    if (bOn)
    {
        bcm2835_gpio_write(m_u32OutputPins[u8Handle], HIGH);
    }
    else
    {
        bcm2835_gpio_write(m_u32OutputPins[u8Handle], LOW);
    }
}

static bool m_bInitialiseGPIO(void)
{
    if (!m_bBCM2835Initialised)
    {
        m_bBCM2835Initialised = (bool)bcm2835_init();
    }
    return m_bBCM2835Initialised;
}
