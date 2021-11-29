#ifndef __PROTOCOL_TEST_H__
#define __PROTOCOL_TEST_H__

#include "efi.h"
#include "log.h"


#define ABC_VERSION 10

#define PROTOCOL_TEST_DRIVER_PRIVATE_DATA_SIGNATURE SIGNATURE_32 ('A','B','C','D')

EFI_GUID       gEfiDriverBindingProtocolGuid;

EFI_SYSTEM_TABLE* gSt;

#endif
