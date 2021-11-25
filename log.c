#include "log.h"

#include "efi.h"

static void debug(
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *out,
	const wchar_t *fmt,
	va_list args)
{

	wchar_t msg[512];

	vsnprintf(msg, sizeof(msg), (char*)fmt, args);

	out->OutputString(out, fmt);
}

void info(EFI_SYSTEM_TABLE *SystemTable, const wchar_t *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	debug(SystemTable->ConOut, fmt, args);
	va_end(args);
}

void err(EFI_SYSTEM_TABLE *SystemTable, const wchar_t *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	debug(SystemTable->StdErr, fmt, args);
	va_end(args);
}
