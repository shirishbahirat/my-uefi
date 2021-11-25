#ifndef __LOG_H__
#define __LOG_H__

#include "efi.h"
#include "debug.h"
void info(EFI_SYSTEM_TABLE *SystemTable, const wchar_t *fmt, ...);
void err(EFI_SYSTEM_TABLE *SystemTable, const wchar_t *fmt, ...);

#endif  // __LOG_H__
