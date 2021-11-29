#include "protocol_test.h"


EFI_GUID gEfiImageSecurityDatabaseGuid  = { 0xd719b2cb, 0x3d3a, 0x4596, {0xa3, 0xbc, 0xda, 0xd0,  0xe, 0x67, 0x65, 0x6f }};


typedef struct _TIMER_PRIVATE_DATA {
  EFI_EVENT                   Timer;

  UINT32                      TimerPeriod;
  UINT32                      RttTimerTick;
  EFI_EVENT                   RttTimer;

  EFI_STATUS                  Status;
} TIMER_PRIVATE_DATA;

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


VOID
EFIAPI
IfTimerEventTriggered (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  *((BOOLEAN *) Context) = true;

  info(gSt, L"   Event triggered ...\r\n");
}

VOID
EFIAPI
TimerTickRoutine (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  UINT32     *TimerTick;

  TimerTick = (UINT32*) Context;
  (*TimerTick)++;

   info(gSt, L"   Timer tick ...\r\n");
}

 
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;
    EFI_HANDLE ControllerHandle = NULL;
    EFI_DEVICE_PATH_PROTOCOL dpath;
    UINTN DataSize;
    EFI_EVENT  TimerEvent;
    EFI_EVENT mTimerTriggered = NULL;
    gSt = SystemTable;
    UINT32 TimerTick;


    Status = SystemTable->BootServices->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  TimerTickRoutine,
                  &TimerTick,
                  &TimerEvent
                  );

      Status = SystemTable->BootServices->SetTimer (
                  TimerEvent,
                  TimerPeriodic,
                  TICKS_PER_MS
                  );

    Status = SystemTable->BootServices->CreateEvent(EVT_TIMER, TPL_CALLBACK, IfTimerEventTriggered, NULL, &mTimerTriggered);


    Status = SystemTable->BootServices->CreateEvent(
                  EVT_NOTIFY_WAIT,
                  TPL_CALLBACK,
                  IfTimerEventTriggered,
                  NULL,
                  &mTimerTriggered
                  );


    if (SystemTable != NULL)
        info(SystemTable, L"System table is loaded ...\r\n");
    
    if (SystemTable->BootServices != NULL)
        info(SystemTable, L"Boot service table loaded...\r\n");
    
    if (SystemTable->RuntimeServices != NULL)
        info(SystemTable, L"Run time service table is loaded...\r\n");

    AbcDriverEntryPoint(ImageHandle,SystemTable);

    (&mAbcDriverBinding)->Start(&mAbcDriverBinding,ControllerHandle, &dpath);

    DataSize  = 0;
    Status =  SystemTable->RuntimeServices->GetVariable(EFI_IMAGE_SECURITY_DATABASE1, &gEfiImageSecurityDatabaseGuid, NULL, &DataSize, NULL);    

    if(EFI_ERROR(Status))
        Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Get Var Failed\r\n" MESSAGE);
    else
        Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Get Var Passed\r\n" MESSAGE);

    if (SystemTable->RuntimeServices != NULL)
        info(SystemTable, L"Abc driver entry point is loaded...\r\n");
 
    /* Empty the console input buffer to flush out any keystrokes entered before this point. */
    Status = SystemTable->ConIn->Reset(SystemTable->ConIn, false);
    if(EFI_ERROR(Status))
        return Status;
 
    /* Wait for keypress. */
    while((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY) ;

    Status = SystemTable->BootServices->CloseEvent (TimerEvent);

 
    return Status;
}


