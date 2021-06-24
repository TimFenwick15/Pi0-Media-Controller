#include <stdio.h>   // Imports printf
#include <stdint.h>  // Imports uint8_t like types
#include <stdbool.h> // Imports true, false, bool
#include <unistd.h>  // Imports usleep
#include "host.h"
#include "button.h"
#include "rotary_encoder.h"
#include "led.h"

static void m_vNextTrack(void);
static void m_vPrevTrack(void);
static void m_vPlayPause(void);
static void m_vMute(void);
static void m_vVolUp(void);
static void m_vVolDown(void);
static void m_vExit(void);
static bool m_bExit = false;

int main(void)
{
    if (!HOST_vInit()
        || !BUTTON_bInit(&m_vNextTrack)                  // Pin 07 GPIO4
        || !BUTTON_bInit(&m_vPrevTrack)                  // Pin 29 GPIO5
        || !BUTTON_bInit(&m_vPlayPause)                  // Pin 31 GPIO6
        || !BUTTON_bInit(&m_vMute)                       // Pin 26 GPIO7
        || !BUTTON_bInit(&m_vVolUp)                      // Pin 24 GPIO8
        || !BUTTON_bInit(&m_vVolDown)                    // Pin 21 GPIO9
        || !BUTTON_bInit(&m_vExit)                       // Pin 19 GPIO10
        || !ROTARY_ENCODER_bInit(&m_vVolUp, &m_vVolDown) // Pin 23 GPIO11 and Pin 32 GPIO12
        || !LED_bInit()                                  // Pin 11 GPIO17
    )
    {
        printf("Failed to initialise");
        return 1;
    }

    printf("Begin\n");
    LED_vSetLed(true);

    while(!m_bExit)
    {
        BUTTON_vPoll();
        ROTARY_ENCODER_vPoll();
        usleep(10 * 1000); // Debounce. sleep for 10 milliseconds
    }

    printf("Exiting\n");
    LED_vSetLed(false);
    HOST_vClose();

    system("shutdown -h now");
    return 0;
}

static void m_vNextTrack(void)
{
    HOST_vSendKey(HOST_eNEXT_TRACK);
}

static void m_vPrevTrack(void)
{
    HOST_vSendKey(HOST_ePREV_TRACK);
}

static void m_vPlayPause(void)
{
    HOST_vSendKey(HOST_ePLAY_PAUSE);
}

static void m_vMute(void)
{
    HOST_vSendKey(HOST_eMUTE);
}

static void m_vVolUp(void)
{
    HOST_vSendKey(HOST_eVOL_UP);
}

static void m_vVolDown(void)
{
    HOST_vSendKey(HOST_eVOL_DOWN);
}

static void m_vExit(void)
{
    m_bExit = true;
}
