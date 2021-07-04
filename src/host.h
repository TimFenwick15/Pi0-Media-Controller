#ifndef HOST_H
#define HOST_H

typedef enum
{
    HOST_eNEXT_TRACK = 0,
    HOST_ePREV_TRACK,
    HOST_ePLAY_PAUSE,
    HOST_eMUTE,
    HOST_eVOL_UP,
    HOST_eVOL_DOWN,
    HOST_eNO_KEY
} HOST_teKey;

bool HOST_vInit(void);
void HOST_vSendKey(HOST_teKey eKey);
void HOST_vClose(void);

#endif
