#include <stdint.h>  // Imports uint8_t like types
#include <stdbool.h> // Imports true, false, bool
#include <unistd.h>  // Imports open, write, and close
#include <fcntl.h>   // Imports O_WRONLY
#include "host.h"

#define REPORT_LENGTH (1)

static int m_s32Handle;

bool HOST_vInit(void)
{
    bool bResult = false;
    m_s32Handle = open("/dev/hidg0", O_WRONLY);
    if (m_s32Handle > -1)
    {
        bResult = true;
    }
    return bResult;
}

void HOST_vSendKey(HOST_teKey eKey)
{
    uint8_t report[REPORT_LENGTH];
    report[0] = (1 << (uint8_t)eKey) & 0xFF;
    write(m_s32Handle, report, REPORT_LENGTH);
    report[0] = 0;
    write(m_s32Handle, report, REPORT_LENGTH);
}

void HOST_vClose(void)
{
    close(m_s32Handle);
}