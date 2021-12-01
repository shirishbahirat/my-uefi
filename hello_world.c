#include "efi.h"
#include "log.h"


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{

 	UINT32 Index;
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;
	(void)ImageHandle;

  	Index = 0;

  	for (Index = 0; Index < 5; Index++)
  	{
		Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello World!\n\r" MESSAGE);
		if(EFI_ERROR(Status))
			return Status;
	}


	/* Empty the console input buffer to flush out any keystrokes entered before this point. */
	Status = SystemTable->ConIn->Reset(SystemTable->ConIn, false);
	if(EFI_ERROR(Status))
		return Status;
 
	/* Wait for keypress. */
	while((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY) ;
 

	return EFI_SUCCESS;

}
