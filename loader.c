
#include "loader.h"


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	(void)ImageHandle;
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;

	info(SystemTable, L"Entry Point ...\r\n");

    if (SystemTable != NULL)
		info(SystemTable, L"System table is loaded ...\r\n");
    
    if (SystemTable->BootServices != NULL)
		info(SystemTable, L"Boot service table loaded...\r\n");
    
    if (SystemTable->RuntimeServices != NULL)
		info(SystemTable, L"Run time service table is loaded...\r\n");

	/* Print message. */
	Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, SystemTable->FirmwareVendor);
	if(EFI_ERROR(Status))
		return Status;

	Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L" \n\r" MESSAGE);
	if(EFI_ERROR(Status))
		return Status;

	info(SystemTable, L"Setting up the loader...\r\n");

	uint16_t config_path[] = u"efi\\boot\\config.txt";
	struct loader loader;
	efi_status_t status;







	/* Print message. */
	Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello World\n\r" MESSAGE);
	if(EFI_ERROR(Status))
		return Status;
 
	/* Empty the console input buffer to flush out any keystrokes entered before this point. */
	Status = SystemTable->ConIn->Reset(SystemTable->ConIn, false);
	if(EFI_ERROR(Status))
		return Status;
 
	/* Wait for keypress. */
	while((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY) ;
 
	return Status;
}
