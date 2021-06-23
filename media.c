//#include <string.h>
//#include <errno.h>
//#include <linux/kd.h>
//#include <termios.h>
//#include <linux/input.h>
//#include <stdlib.h>

#include <stdio.h>   // Imports printf
#include <unistd.h>  // Imports open, write, and close
#include <fcntl.h>   // Imports O_WRONLY
#include <bcm2835.h> // Imports GPIO functions

#define PIN RPI_GPIO_P1_15 // Pin 15 (GPIO22), NOT GPIO15
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

teMedia getKey(void);

int main(void)
{
    int hid = open("/dev/hidg0", O_WRONLY);
    unsigned char report[REPORT_LENGTH];
    teMedia eKey = eMEDIA_NO_KEY;

    if (!bcm2835_init())
    {
        printf("Failed to initialise bcm2835 GPIO module\n");
        return 1;
    }
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
    //bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_UP); // set pull-up
    bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_DOWN); // set pull-down

    while(1)
    {
        teMedia eNewKey = getKey();
        if (eKey != eNewKey && eNewKey < eMEDIA_NO_KEY)
        {
            printf("Sending mute\n");
            eKey = eNewKey;
            report[0] = 1 << eKey;
            write(hid, report, REPORT_LENGTH);
            report[0] = 0;
            write(hid, report, REPORT_LENGTH);
        }
        eKey = eNewKey;
    }

    close(hid);
    return 0;
}

teMedia getKey(void)
{
    teMedia eResult = eMEDIA_NO_KEY;
    if (bcm2835_gpio_lev(PIN)) // Pulling down, so 0 means unpressed, 1 means pressed
    {
        eResult = eMEDIA_MUTE;
    }
    return eResult;
}

