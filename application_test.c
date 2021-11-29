#include "efi.h"
#include "log.h"


EFI_STATUS EFIAPI ApplicationEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );
 
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;

    if (SystemTable != NULL)
		info(SystemTable, L"System table is loaded ...\r\n");
    
    if (SystemTable->BootServices != NULL)
		info(SystemTable, L"Boot service table loaded...\r\n");
    
    if (SystemTable->RuntimeServices != NULL)
		info(SystemTable, L"Run time service table is loaded...\r\n");

	ApplicationEntryPoint(ImageHandle,SystemTable);
 
	/* Empty the console input buffer to flush out any keystrokes entered before this point. */
	Status = SystemTable->ConIn->Reset(SystemTable->ConIn, false);
	if(EFI_ERROR(Status))
		return Status;
 
	/* Wait for keypress. */
	while((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY) ;
 
	return Status;
}


EFI_STATUS
EFIAPI
ApplicationEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

	EFI_STATUS Status;

	/* Print message. */
	Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Calling from test driver\n\r" MESSAGE);
	if(EFI_ERROR(Status))
		return Status;

  return EFI_SUCCESS;
}
