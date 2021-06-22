//#include <string.h>
//#include <errno.h>
//#include <linux/kd.h>
//#include <termios.h>
//#include <linux/input.h>
//#include <stdlib.h>

#include <stdio.h>  // Imports printf
#include <unistd.h> // Imports open, write, and close
#include <fcntl.h>  // Imports O_WRONLY

#define REPORT_LENGTH (1)

typedef enum
{
    eMEDIA_NEXT_TRACK = 0,
    eMEDIA_PREV_TRACK,
    eMEDIA_STOP,
    eMEDIA_PLAY_PAUSE,
    eMEDIA_MUTE,
    eMEDIA_VOL_UP,
    eMEDIA_VOL_DOWN
} teMedia;

int main(void)
{
    int hid = open("/dev/hidg0", O_WRONLY);
    unsigned char report[REPORT_LENGTH];
    teMedia eKey = eMEDIA_MUTE;

    report[0] = 1 << eKey;
    write(hid, report, REPORT_LENGTH);
    report[0] = 0;
    write(hid, report, REPORT_LENGTH);

    close(hid);
    return 0;
}

