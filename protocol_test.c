#include "protocol_test.h"




EFI_STATUS
AbcStop (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE ControllerHandle,
  IN UINTN NumberOfChildren,
  IN EFI_HANDLE *ChildHandleBuffer OPTIONAL
)
{

  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  return Status;

}


EFI_STATUS
AbcStart (
  IN EFI_DRIVER_BINDING_PROTOCOL *This ,
  IN EFI_HANDLE ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL
)
{

  EFI_STATUS Status;

  if (gSt != NULL)
	info(gSt, L"Abc driver started...\r\n");  

  Status = EFI_SUCCESS;

  return Status;
}


EFI_STATUS
AbcSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL
)
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  return Status;
}


static EFI_DRIVER_BINDING_PROTOCOL mAbcDriverBinding = {
  AbcSupported,
  AbcStart,
  AbcStop,
  ABC_VERSION,
  NULL,
  NULL
};

EFI_STATUS
EFIAPI
AbcDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  
  EFI_STATUS Status;

  mAbcDriverBinding.ImageHandle = ImageHandle;
  mAbcDriverBinding.DriverBindingHandle = ImageHandle;

  Status = SystemTable->BootServices->InstallMultipleProtocolInterfaces (
                  &mAbcDriverBinding.DriverBindingHandle,
                  &gEfiDriverBindingProtocolGuid,
                  &mAbcDriverBinding,
                  NULL
                  );

  if(EFI_ERROR(Status))
  {
  	if (SystemTable->BootServices != NULL)
		info(SystemTable, L"Error status in installing protocol ...\r\n");
	return Status;
  }

  if (SystemTable->BootServices != NULL)
		info(SystemTable, L"Sucess in installing protocol ...\r\n");

  return EFI_SUCCESS;
}


EFI_STATUS
AbcUnload (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  EFI_STATUS Status;
  Status = SystemTable->BootServices->UninstallMultipleProtocolInterfaces (
                       ImageHandle,
                       &gEfiDriverBindingProtocolGuid,
                       &mAbcDriverBinding,
                       NULL
                        );
  return Status;
}


 
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

	gSt = SystemTable;

	AbcDriverEntryPoint(ImageHandle,SystemTable);

    if (SystemTable->RuntimeServices != NULL)
		info(SystemTable, L"Abc driver entry point is loaded...\r\n");
 
	/* Empty the console input buffer to flush out any keystrokes entered before this point. */
	Status = SystemTable->ConIn->Reset(SystemTable->ConIn, false);
	if(EFI_ERROR(Status))
		return Status;
 
	/* Wait for keypress. */
	while((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY) ;
 
	return Status;
}


