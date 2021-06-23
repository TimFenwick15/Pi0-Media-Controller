//#include <string.h>
//#include <errno.h>
//#include <linux/kd.h>
//#include <termios.h>
//#include <linux/input.h>

#include <stdio.h>   // Imports printf
#include <stdlib.h>  // Imports usleep
#include <stdbool.h>
#include <unistd.h>  // Imports open, write, and close
#include <fcntl.h>   // Imports O_WRONLY
#include <bcm2835.h> // Imports GPIO functions
#include "rotary_encoder.h"

/*
 * GPIO 2 and 3 have fixed pull up resistors.
 */
#define PIN_NEXT_TRACK RPI_BPLUS_GPIO_J8_07 // Pin 07 GPIO4, not GPIO7 in this numbering
#define PIN_PREV_TRACK RPI_BPLUS_GPIO_J8_29 // Pin 29 GPIO5
#define PIN_STOP       RPI_BPLUS_GPIO_J8_31 // Pin 31 GPIO6
#define PIN_PLAY_PAUSE RPI_BPLUS_GPIO_J8_26 // Pin 27 GPIO7
#define PIN_MUTE       RPI_BPLUS_GPIO_J8_24 // Pin 24 GPIO8
#define PIN_VOL_UP     RPI_BPLUS_GPIO_J8_21 // Pin 21 GPIO9
#define PIN_VOL_DOWN   RPI_BPLUS_GPIO_J8_19 // Pin 19 GPIO10
#define PIN_EXIT       RPI_BPLUS_GPIO_J8_23 // Pin 23 GPIO11
#define PIN_LED        RPI_BPLUS_GPIO_J8_32 // Pin 32 GPIO12

#define PIN_ENCODER_A  RPI_BPLUS_GPIO_J8_33 // Pin 33 GPIO13
#define PIN_ENCODER_B  RPI_BPLUS_GPIO_J8_08 // Pin 08 GPIO14
#define PIN_ENCODER_C  RPI_BPLUS_GPIO_J8_10 // Pin 10 GPIO15

#define REPORT_LENGTH (1)

typedef enum
{
    eMEDIA_NEXT_TRACK = 0,
    eMEDIA_PREV_TRACK,
    eMEDIA_STOP,
    eMEDIA_PLAY_PAUSE,
    eMEDIA_MUTE,
    eMEDIA_VOL_UP,
    eMEDIA_VOL_DOWN,
    eMEDIA_NO_KEY
} teMedia;

bool initialiseGPIO(void);
void setLED(bool on);
teMedia getKey(void);
bool getExit(void);

int main(void)
{
    int hid = open("/dev/hidg0", O_WRONLY);
    unsigned char report[REPORT_LENGTH];
    teMedia eKey = eMEDIA_NO_KEY;

    if (!initialiseGPIO())
    {
        printf("Failed to initialise bcm2835 GPIO module\n");
        return 1;
    }
    ROTARY_ENCODER_vInit(PIN_ENCODER_A, PIN_ENCODER_B, PIN_ENCODER_C);

    printf("Begin\n");
    setLED(true);
    while(!getExit())
    {
        teMedia eNewKey = getKey();
        if (eKey != eNewKey)
        {
            eKey = eNewKey;
            if (eKey < eMEDIA_NO_KEY)
            {
                report[0] = (1 << (uint8_t)eKey) & 0xFF;
                write(hid, report, REPORT_LENGTH);
                report[0] = 0;
                write(hid, report, REPORT_LENGTH);
            }
        }
        ROTARY_ENCODER_sGetData();

        usleep(50 * 1000); // Debounce. sleep for 50 milliseconds
    }

    printf("Exiting\n");
    setLED(false);

    close(hid);

    system("shutdown -h now");
    return 0;
}

teMedia getKey(void)
{
    teMedia eResult = eMEDIA_NO_KEY;
    if (bcm2835_gpio_lev(PIN_NEXT_TRACK)) // Pulling down, so 0 means unpressed, 1 means pressed
    {
        eResult = eMEDIA_NEXT_TRACK;
    }
    else if (bcm2835_gpio_lev(PIN_PREV_TRACK))
    {
        eResult = eMEDIA_PREV_TRACK;
    }
    else if (bcm2835_gpio_lev(PIN_STOP))
    {
        eResult = eMEDIA_STOP;
    }
    else if (bcm2835_gpio_lev(PIN_PLAY_PAUSE))
    {
        eResult = eMEDIA_PLAY_PAUSE;
    }
    else if (bcm2835_gpio_lev(PIN_MUTE))
    {
        eResult = eMEDIA_MUTE;
    }
    else if (bcm2835_gpio_lev(PIN_VOL_UP))
    {
        eResult = eMEDIA_VOL_UP;
    }
    else if (bcm2835_gpio_lev(PIN_VOL_DOWN))
    {
        eResult = eMEDIA_VOL_DOWN;
    }
    else
    {
        eResult = eMEDIA_NO_KEY;
    }

    return eResult;
}

bool getExit(void)
{
    bool bResult = false;
    if (bcm2835_gpio_lev(PIN_EXIT))
    {
        bResult = true;
    }
    return bResult;
}

void setLED(bool on)
{
    if (on)
    {
        bcm2835_gpio_write(PIN_LED, HIGH);
    }
    else
    {
        bcm2835_gpio_write(PIN_LED, LOW);
    }
}

bool initialiseGPIO(void)
{
    bool bResult = false;
    if (bcm2835_init())
    {
        bcm2835_gpio_fsel(   PIN_NEXT_TRACK, BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(PIN_NEXT_TRACK, BCM2835_GPIO_PUD_DOWN); // set pull-down
        bcm2835_gpio_fsel(   PIN_PREV_TRACK, BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(PIN_PREV_TRACK, BCM2835_GPIO_PUD_DOWN);
        bcm2835_gpio_fsel(   PIN_STOP,       BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(PIN_STOP,       BCM2835_GPIO_PUD_DOWN);
        bcm2835_gpio_fsel(   PIN_PLAY_PAUSE, BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(PIN_PLAY_PAUSE, BCM2835_GPIO_PUD_DOWN);
        bcm2835_gpio_fsel(   PIN_MUTE,       BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(PIN_MUTE,       BCM2835_GPIO_PUD_DOWN);
        bcm2835_gpio_fsel(   PIN_VOL_UP,     BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(PIN_VOL_UP,     BCM2835_GPIO_PUD_DOWN);
        bcm2835_gpio_fsel(   PIN_VOL_DOWN,   BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(PIN_VOL_DOWN,   BCM2835_GPIO_PUD_DOWN);
        bcm2835_gpio_fsel(   PIN_EXIT,       BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(PIN_EXIT,       BCM2835_GPIO_PUD_DOWN);

        bcm2835_gpio_fsel(   PIN_LED,        BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_set_pud(PIN_LED,        BCM2835_GPIO_PUD_DOWN);

        bResult = true;
    }
    return bResult;
}

