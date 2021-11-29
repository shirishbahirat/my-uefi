#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include "efi_spec.h"
 
#ifndef EFI_H
#define EFI_H
 
typedef void* EFI_PVOID;
typedef void* EFI_HANDLE;
 
#define EFIERR(a) (a | ~(((EFI_STATUS)-1) >> 1))
#define EFI_ERROR(a) (a & ~(((EFI_STATUS)-1) >> 1))
#define EFI_SUCCESS 0

#define SIGNATURE_16(A, B)        ((A) | (B << 8))
#define SIGNATURE_32(A, B, C, D)  (SIGNATURE_16 (A, B) | (SIGNATURE_16 (C, D) << 16))
 
#define EFI_NOT_READY EFIERR(6)
 
typedef struct {
	uint64_t Signature;
	uint32_t Revision;
	uint32_t HeaderSize;
	uint32_t CRC32;
	uint32_t Reserved;
} EFI_TABLE_HEADER;
 
typedef struct {
	uint32_t MaxMode;
	uint32_t Mode;
	uint32_t Attribute;
	uint32_t CursorColumn;
	uint32_t CursorRow;
	uint8_t  CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;
 
typedef EFI_STATUS (*EFI_TEXT_CLEAR_SCREEN)(void *This);
typedef EFI_STATUS (*EFI_TEXT_ENABLE_CURSOR)(void *This, uint8_t Visible);
typedef EFI_STATUS (*EFI_TEXT_SET_ATTRIBUTE)(void *This, UINTN Attribute);
typedef EFI_STATUS (*EFI_TEXT_STRING)(void *This, const wchar_t *String);
 
typedef EFI_STATUS (*EFI_TEXT_QUERY_MODE)(
	void  *This,
	UINTN ModeNumber,
	UINTN *Columns,
	UINTN *Rows);
 
typedef EFI_STATUS (*EFI_TEXT_SET_CURSOR_POSITION)(
	void  *This,
	UINTN Column,
	UINTN Row);
 
typedef struct {
	EFI_PVOID                    Reset;
	EFI_TEXT_STRING              OutputString;
	EFI_PVOID                    TestString;
	EFI_TEXT_QUERY_MODE          QueryMode;
	EFI_PVOID                    SetMode;
	EFI_TEXT_SET_ATTRIBUTE       SetAttribute;
	EFI_TEXT_CLEAR_SCREEN        ClearScreen;
	EFI_TEXT_SET_CURSOR_POSITION SetCursorPosition;
	EFI_TEXT_ENABLE_CURSOR       EnableCursor;
	SIMPLE_TEXT_OUTPUT_MODE      *Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
 
typedef struct {
	wchar_t ScanCode;
	wchar_t UnicodeChar;
} EFI_INPUT_KEY;
 
typedef EFI_STATUS (*EFI_INPUT_RESET)(void *This, bool ExtendedVerification);
typedef EFI_STATUS (*EFI_INPUT_READ_KEY)(void *This, EFI_INPUT_KEY *Key);
 
typedef struct {
	EFI_INPUT_RESET    Reset;
	EFI_INPUT_READ_KEY ReadKeyStroke;
	EFI_PVOID          WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;


typedef struct {
  EFI_TABLE_HEADER                Hdr;
  EFI_PEI_INSTALL_PPI             InstallPpi;
  EFI_PEI_REINSTALL_PPI           ReInstallPpi;
  EFI_PEI_LOCATE_PPI              LocatePpi;
  EFI_PEI_NOTIFY_PPI              NotifyPpi;
  EFI_PEI_GET_BOOT_MODE           GetBootMode;
  EFI_PEI_SET_BOOT_MODE           SetBootMode;
  EFI_PEI_GET_HOB_LIST            GetHobList;
  EFI_PEI_CREATE_HOB              CreateHob;
  EFI_PEI_FFS_FIND_NEXT_VOLUME2   FfsFindNextVolume;
  EFI_PEI_FFS_FIND_NEXT_FILE2     FfsFindNextFile;
  EFI_PEI_FFS_FIND_SECTION_DATA2  FfsFindSectionData;
  EFI_PEI_INSTALL_PEI_MEMORY      InstallPeiMemory;
  EFI_PEI_ALLOCATE_PAGES          AllocatePages;
  EFI_PEI_ALLOCATE_POOL           AllocatePool;
  EFI_PEI_COPY_MEM                CopyMem;
  EFI_PEI_SET_MEM                 SetMem;
  EFI_PEI_REPORT_STATUS_CODE      ReportStatusCode;
  EFI_PEI_RESET_SYSTEM            ResetSystem;
  EFI_PEI_CPU_IO_PPI              *CpuIo;
  EFI_PEI_PCI_CFG2_PPI            *PciCfg;
  EFI_PEI_FFS_FIND_BY_NAME        FfsFindFileByName;
  EFI_PEI_FFS_GET_FILE_INFO       FfsGetFileInfo;
  EFI_PEI_FFS_GET_VOLUME_INFO     FfsGetVolumeInfo;
  EFI_PEI_REGISTER_FOR_SHADOW     RegisterForShadow;
  EFI_PEI_FFS_FIND_SECTION_DATA3  FindSectionData3;
  EFI_PEI_FFS_GET_FILE_INFO2      FfsGetFileInfo2;
  EFI_PEI_RESET2_SYSTEM           ResetSystem2;
  EFI_PEI_FREE_PAGES              FreePages;
} EFI_PEI_SERVICES;



typedef struct {
  EFI_TABLE_HEADER                  Hdr;
  EFI_RAISE_TPL                     RaiseTPL;
  EFI_RESTORE_TPL                   RestoreTPL;
  EFI_ALLOCATE_PAGES                AllocatePages;
  EFI_FREE_PAGES                    FreePages;
  EFI_GET_MEMORY_MAP                GetMemoryMap;
  EFI_ALLOCATE_POOL                 AllocatePool;
  EFI_FREE_POOL                     FreePool;
  EFI_CREATE_EVENT                  CreateEvent;
  EFI_SET_TIMER                     SetTimer;
  EFI_WAIT_FOR_EVENT                WaitForEvent;
  EFI_SIGNAL_EVENT                  SignalEvent;
  EFI_CLOSE_EVENT                   CloseEvent;
  EFI_CHECK_EVENT                   CheckEvent;
  EFI_INSTALL_PROTOCOL_INTERFACE    InstallProtocolInterface;
  EFI_REINSTALL_PROTOCOL_INTERFACE  ReinstallProtocolInterface;
  EFI_UNINSTALL_PROTOCOL_INTERFACE  UninstallProtocolInterface;
  EFI_HANDLE_PROTOCOL               HandleProtocol;
  VOID                              *Reserved;
  EFI_REGISTER_PROTOCOL_NOTIFY      RegisterProtocolNotify;
  EFI_LOCATE_HANDLE                 LocateHandle;
  EFI_LOCATE_DEVICE_PATH            LocateDevicePath;
  EFI_INSTALL_CONFIGURATION_TABLE   InstallConfigurationTable;
  EFI_IMAGE_LOAD                    LoadImage;
  EFI_IMAGE_START                   StartImage;
  EFI_EXIT                          Exit;
  EFI_IMAGE_UNLOAD                  UnloadImage;
  EFI_EXIT_BOOT_SERVICES            ExitBootServices;
  EFI_GET_NEXT_MONOTONIC_COUNT      GetNextMonotonicCount;
  EFI_STALL                         Stall;
  EFI_SET_WATCHDOG_TIMER            SetWatchdogTimer;
  EFI_CONNECT_CONTROLLER            ConnectController;
  EFI_DISCONNECT_CONTROLLER         DisconnectController;
  EFI_OPEN_PROTOCOL                 OpenProtocol;
  EFI_CLOSE_PROTOCOL                CloseProtocol;
  EFI_OPEN_PROTOCOL_INFORMATION     OpenProtocolInformation;
  EFI_PROTOCOLS_PER_HANDLE          ProtocolsPerHandle;
  EFI_LOCATE_HANDLE_BUFFER          LocateHandleBuffer;
  EFI_LOCATE_PROTOCOL               LocateProtocol;
  EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES    InstallMultipleProtocolInterfaces;
  EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES  UninstallMultipleProtocolInterfaces;
  EFI_CALCULATE_CRC32               CalculateCrc32;
  EFI_COPY_MEM                      CopyMem;
  EFI_SET_MEM                       SetMem;
  EFI_CREATE_EVENT_EX               CreateEventEx;
} EFI_BOOT_SERVICES;




typedef struct {
  EFI_TABLE_HEADER                Hdr;
  EFI_GET_TIME                    GetTime;
  EFI_SET_TIME                    SetTime;
  EFI_GET_WAKEUP_TIME             GetWakeupTime;
  EFI_SET_WAKEUP_TIME             SetWakeupTime;
  EFI_SET_VIRTUAL_ADDRESS_MAP     SetVirtualAddressMap;
  EFI_CONVERT_POINTER             ConvertPointer;
  EFI_GET_VARIABLE                GetVariable;
  EFI_GET_NEXT_VARIABLE_NAME      GetNextVariableName;
  EFI_SET_VARIABLE                SetVariable;
  EFI_GET_NEXT_HIGH_MONO_COUNT    GetNextHighMonotonicCount;
  EFI_RESET_SYSTEM                ResetSystem;
  EFI_UPDATE_CAPSULE              UpdateCapsule;
  EFI_QUERY_CAPSULE_CAPABILITIES  QueryCapsuleCapabilities;
  EFI_QUERY_VARIABLE_INFO         QueryVariableInfo;
} EFI_RUNTIME_SERVICES;


typedef struct {
	EFI_TABLE_HEADER                Hdr;
	EFI_PVOID                       FirmwareVendor;
	INT32                           FirmwareRevision;
	EFI_PVOID                       ConsoleInHandle;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *ConIn;
	EFI_PVOID                       ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
	EFI_PVOID                       StandardErrorHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
	EFI_RUNTIME_SERVICES            *RuntimeServices;
	EFI_BOOT_SERVICES               *BootServices;
	UINTN                           NumberOfTableEntries;
	void                            *ConfigurationTable;
} EFI_SYSTEM_TABLE;
 
#endif
