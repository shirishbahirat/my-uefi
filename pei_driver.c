#include "efi.h"
#include "log.h"

EFI_STATUS
EFIAPI
NvmExpressPeimEntry (
  IN EFI_PEI_FILE_HANDLE    FileHandle,
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{

  EFI_BOOT_MODE  BootMode;
  //IA32_DESCRIPTOR   Idtr;

  //PeiServices = (CONST EFI_PEI_SERVICES **) (*(UINTN*)(Idtr.Base - sizeof (UINTN)));

  //PeiServices = GetPeiServicesTablePointer ();

  BootMode = (*PeiServices)->GetBootMode (PeiServices, &BootMode);

  return EFI_SUCCESS;
}


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;

	EFI_PEI_FILE_HANDLE    FileHandle = NULL;
    CONST EFI_PEI_SERVICES **PeiServices = NULL;

    if (SystemTable != NULL)
		info(SystemTable, L"System table is loaded ...\r\n");
    
    if (SystemTable->BootServices != NULL)
		info(SystemTable, L"Boot service table loaded...\r\n");
    
    if (SystemTable->RuntimeServices != NULL)
		info(SystemTable, L"Run time service table is loaded...\r\n");

	NvmExpressPeimEntry(FileHandle, PeiServices);
 
	/* Empty the console input buffer to flush out any keystrokes entered before this point. */
	Status = SystemTable->ConIn->Reset(SystemTable->ConIn, false);
	if(EFI_ERROR(Status))
		return Status;
 
	/* Wait for keypress. */
	while((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY) ;
 
	return Status;
}






