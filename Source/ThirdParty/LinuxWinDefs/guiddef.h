
#pragma once

#include <string.h> // memcmp

#define INITGUID

typedef struct _GUID {
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} GUID;

#ifdef __cplusplus
typedef const GUID& REFGUID;

inline int IsEqualGUID(REFGUID rguid1, REFGUID rguid2) {
    return !memcmp(&rguid1, &rguid2, sizeof(GUID));
}
#else
#define IsEqualGUID(rguid1, rguid2) \
    (!memcmp(&rguid1, &rguid2, sizeof(GUID)))
#endif

