#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include "efi_spec.h"
 
#ifndef EFI_H
#define EFI_H
 
typedef void* EFI_PVOID;
typedef void* EFI_HANDLE;

#define TPL_APPLICATION       4
#define TPL_CALLBACK          8
#define TPL_NOTIFY            16
#define TPL_HIGH_LEVEL        31

#define EVT_TIMER                         0x80000000
#define EVT_RUNTIME                       0x40000000
#define EVT_NOTIFY_WAIT                   0x00000100
#define EVT_NOTIFY_SIGNAL                 0x00000200

#define EVT_SIGNAL_EXIT_BOOT_SERVICES     0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE 0x60000202

#define MAX_BIT      0x8000000000000000
#define RSIZE_MAX 1000000
 
#define MAX_UINT64 ((UINT64)0xFFFFFFFFFFFFFFFFULL)
#define MAX_UINT32 ((UINT32)0xFFFFFFFF)
#define MAX_UINT16  ((UINT16)0xFFFF)
#define MAX_UINT8   ((UINT8)0xFF)

#define END_DEVICE_PATH_LENGTH               (sizeof (EFI_DEVICE_PATH_PROTOCOL))
#define MAX_DEVICE_PATH_NODE_COUNT   1024
#define SIZE_64KB   0x00010000

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

typedef enum {
  EfiPeiCpuIoWidthUint8,
  EfiPeiCpuIoWidthUint16,
  EfiPeiCpuIoWidthUint32,
  EfiPeiCpuIoWidthUint64,
  EfiPeiCpuIoWidthFifoUint8,
  EfiPeiCpuIoWidthFifoUint16,
  EfiPeiCpuIoWidthFifoUint32,
  EfiPeiCpuIoWidthFifoUint64,
  EfiPeiCpuIoWidthFillUint8,
  EfiPeiCpuIoWidthFillUint16,
  EfiPeiCpuIoWidthFillUint32,
  EfiPeiCpuIoWidthFillUint64,
  EfiPeiCpuIoWidthMaximum
} EFI_PEI_CPU_IO_PPI_WIDTH;

typedef struct _EFI_PEI_SERVICES          EFI_PEI_SERVICES;
typedef struct _EFI_PEI_NOTIFY_DESCRIPTOR EFI_PEI_NOTIFY_DESCRIPTOR;
typedef struct _EFI_PEI_CPU_IO_PPI EFI_PEI_CPU_IO_PPI;
typedef struct _EFI_PEI_PCI_CFG2_PPI EFI_PEI_PCI_CFG2_PPI;
typedef UINT32  EFI_BOOT_MODE;
typedef VOID    *EFI_PEI_FV_HANDLE;
typedef UINT8 EFI_FV_FILETYPE;
typedef VOID    *EFI_PEI_FILE_HANDLE;
typedef UINT8 EFI_SECTION_TYPE;
typedef UINT32  EFI_STATUS_CODE_TYPE;
typedef UINT32 EFI_STATUS_CODE_VALUE;
typedef UINT32  EFI_FV_FILE_ATTRIBUTES;
typedef UINT32  EFI_FVB_ATTRIBUTES_2;

typedef struct {
  UINT16    HeaderSize;
  UINT16    Size;
  EFI_GUID  Type;
} EFI_STATUS_CODE_DATA;

typedef struct {
  EFI_GUID                FileName;
  EFI_FV_FILETYPE         FileType;
  EFI_FV_FILE_ATTRIBUTES  FileAttributes;
  VOID                    *Buffer;
  UINT32                  BufferSize;
} EFI_FV_FILE_INFO;


typedef struct {
  EFI_GUID                FileName;
  EFI_FV_FILETYPE         FileType;
  EFI_FV_FILE_ATTRIBUTES  FileAttributes;
  VOID                    *Buffer;
  UINT32                  BufferSize;
  UINT32                  AuthenticationStatus;
} EFI_FV_FILE_INFO2;

typedef struct {
  EFI_FVB_ATTRIBUTES_2  FvAttributes;
  EFI_GUID              FvFormat;
  EFI_GUID              FvName;
  VOID                  *FvStart;
  UINT64                FvSize;
} EFI_FV_INFO;


typedef enum {
  EfiPeiPciCfgWidthUint8  = 0,
  EfiPeiPciCfgWidthUint16 = 1,
  EfiPeiPciCfgWidthUint32 = 2,
  EfiPeiPciCfgWidthUint64 = 3,
  EfiPeiPciCfgWidthMaximum
} EFI_PEI_PCI_CFG_PPI_WIDTH;


#pragma pack (1)
typedef struct {
  UINT16  Limit;
  UINTN   Base;
} IA32_DESCRIPTOR;
#pragma pack ()

typedef EFI_STATUS (EFIAPI *EFI_PEI_INSTALL_PPI)(IN CONST EFI_PEI_SERVICES **PeiServices, IN CONST EFI_PEI_PPI_DESCRIPTOR *PpiList);

typedef EFI_STATUS (EFIAPI *EFI_PEI_REINSTALL_PPI)(IN CONST EFI_PEI_SERVICES **PeiServices, IN CONST EFI_PEI_PPI_DESCRIPTOR *OldPpi, IN CONST EFI_PEI_PPI_DESCRIPTOR *NewPpi);

typedef EFI_STATUS (EFIAPI *EFI_PEI_LOCATE_PPI)(IN CONST EFI_PEI_SERVICES **PeiServices, IN CONST EFI_GUID *Guid, IN UINTN  Instance, IN OUT EFI_PEI_PPI_DESCRIPTOR **PpiDescriptor OPTIONAL, IN OUT VOID **Ppi);
 
typedef EFI_STATUS (EFIAPI *EFI_PEI_NOTIFY_PPI)(IN CONST EFI_PEI_SERVICES **PeiServices, IN CONST EFI_PEI_NOTIFY_DESCRIPTOR *NotifyList);

typedef EFI_STATUS (EFIAPI *EFI_PEI_GET_BOOT_MODE)(IN CONST EFI_PEI_SERVICES **PeiServices, OUT EFI_BOOT_MODE *BootMode);

typedef EFI_STATUS (EFIAPI *EFI_PEI_SET_BOOT_MODE)(IN CONST EFI_PEI_SERVICES **PeiServices, IN EFI_BOOT_MODE BootMode);

typedef EFI_STATUS (EFIAPI *EFI_PEI_GET_HOB_LIST)(IN CONST EFI_PEI_SERVICES **PeiServices, OUT VOID **HobList);

typedef EFI_STATUS (EFIAPI *EFI_PEI_CREATE_HOB)(IN CONST EFI_PEI_SERVICES **PeiServices, IN UINT16 Type, IN UINT16 Length, IN OUT VOID **Hob);
 
typedef EFI_STATUS (EFIAPI *EFI_PEI_FFS_FIND_NEXT_VOLUME2)(IN CONST EFI_PEI_SERVICES **PeiServices, IN UINTN Instance, OUT EFI_PEI_FV_HANDLE *VolumeHandle);

typedef EFI_STATUS (EFIAPI *EFI_PEI_FFS_FIND_NEXT_FILE2)(IN CONST EFI_PEI_SERVICES **PeiServices, IN EFI_FV_FILETYPE SearchType, IN CONST EFI_PEI_FV_HANDLE FvHandle, IN OUT EFI_PEI_FILE_HANDLE *FileHandle);

typedef EFI_STATUS (EFIAPI *EFI_PEI_FFS_FIND_SECTION_DATA2)(IN CONST EFI_PEI_SERVICES **PeiServices, IN EFI_SECTION_TYPE SectionType, IN EFI_PEI_FILE_HANDLE FileHandle, OUT VOID **SectionData);

typedef EFI_STATUS (EFIAPI *EFI_PEI_INSTALL_PEI_MEMORY)(IN CONST EFI_PEI_SERVICES **PeiServices, IN EFI_PHYSICAL_ADDRESS MemoryBegin, IN UINT64 MemoryLength);

typedef EFI_STATUS (EFIAPI *EFI_PEI_ALLOCATE_PAGES)(IN CONST EFI_PEI_SERVICES **PeiServices, IN EFI_MEMORY_TYPE MemoryType, IN UINTN Pages, OUT EFI_PHYSICAL_ADDRESS *Memory);

typedef EFI_STATUS (EFIAPI *EFI_PEI_ALLOCATE_POOL)(IN CONST EFI_PEI_SERVICES **PeiServices, IN UINTN Size, OUT VOID **Buffer);

typedef VOID (EFIAPI *EFI_PEI_COPY_MEM)(IN VOID *Destination, IN VOID *Source, IN UINTN Length);

typedef VOID (EFIAPI *EFI_PEI_SET_MEM)(IN VOID *Buffer, IN UINTN Size, IN UINT8 Value);

typedef EFI_STATUS (EFIAPI *EFI_PEI_REPORT_STATUS_CODE)(IN CONST EFI_PEI_SERVICES **PeiServices, IN EFI_STATUS_CODE_TYPE Type, IN EFI_STATUS_CODE_VALUE Value, IN UINT32 Instance, IN CONST EFI_GUID *CallerId OPTIONAL, IN CONST EFI_STATUS_CODE_DATA *Data OPTIONAL);

typedef EFI_STATUS (EFIAPI *EFI_PEI_RESET_SYSTEM)(IN CONST EFI_PEI_SERVICES **PeiServices);

typedef EFI_STATUS (EFIAPI *EFI_PEI_FFS_FIND_BY_NAME)(IN  CONST  EFI_GUID *FileName, IN  EFI_PEI_FV_HANDLE VolumeHandle, OUT EFI_PEI_FILE_HANDLE *FileHandle);

typedef EFI_STATUS (EFIAPI *EFI_PEI_FFS_GET_FILE_INFO)(IN  EFI_PEI_FILE_HANDLE FileHandle, OUT EFI_FV_FILE_INFO *FileInfo);

typedef EFI_STATUS (EFIAPI *EFI_PEI_FFS_GET_FILE_INFO2)(IN  EFI_PEI_FILE_HANDLE FileHandle, OUT EFI_FV_FILE_INFO2 *FileInfo);

typedef EFI_STATUS (EFIAPI *EFI_PEI_FFS_GET_VOLUME_INFO)(IN  EFI_PEI_FV_HANDLE VolumeHandle, OUT EFI_FV_INFO *VolumeInfo);

typedef EFI_STATUS (EFIAPI *EFI_PEI_REGISTER_FOR_SHADOW)(IN  EFI_PEI_FILE_HANDLE FileHandle);

typedef EFI_STATUS (EFIAPI *EFI_PEI_FFS_FIND_SECTION_DATA3)(IN CONST EFI_PEI_SERVICES **PeiServices, IN EFI_SECTION_TYPE SectionType, IN UINTN SectionInstance, IN EFI_PEI_FILE_HANDLE FileHandle, OUT VOID **SectionData, OUT UINT32 *AuthenticationStatus);

typedef VOID (EFIAPI *EFI_PEI_RESET2_SYSTEM) (IN EFI_RESET_TYPE ResetType, IN EFI_STATUS ResetStatus, IN UINTN DataSize, IN VOID *ResetData OPTIONAL);

typedef EFI_STATUS (EFIAPI *EFI_PEI_FREE_PAGES) (IN CONST EFI_PEI_SERVICES **PeiServices, IN EFI_PHYSICAL_ADDRESS Memory, IN UINTN Pages);

typedef EFI_STATUS (EFIAPI *EFI_PEIM_NOTIFY_ENTRY_POINT)(IN EFI_PEI_SERVICES **PeiServices, IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor, IN VOID *Ppi);

typedef struct _EFI_PEI_SERVICES{
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

typedef
EFI_STATUS
(EFIAPI *EFI_PEI_PCI_CFG2_PPI_IO)(
  IN CONST  EFI_PEI_SERVICES          **PeiServices,
  IN CONST  EFI_PEI_PCI_CFG2_PPI      *This,
  IN        EFI_PEI_PCI_CFG_PPI_WIDTH Width,
  IN        UINT64                    Address,
  IN OUT    VOID                      *Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_PEI_PCI_CFG2_PPI_RW)(
  IN CONST  EFI_PEI_SERVICES          **PeiServices,
  IN CONST  EFI_PEI_PCI_CFG2_PPI      *This,
  IN        EFI_PEI_PCI_CFG_PPI_WIDTH Width,
  IN        UINT64                    Address,
  IN        VOID                      *SetBits,
  IN        VOID                      *ClearBits
);

typedef struct _EFI_PEI_PCI_CFG2_PPI{
  EFI_PEI_PCI_CFG2_PPI_IO  Read;
  EFI_PEI_PCI_CFG2_PPI_IO  Write;
  EFI_PEI_PCI_CFG2_PPI_RW  Modify;
  UINT16                  Segment;
}EFI_PEI_PCI_CFG2_PPI;

typedef struct _EFI_PEI_NOTIFY_DESCRIPTOR{
  UINTN                       Flags;
  EFI_GUID                    *Guid;
  EFI_PEIM_NOTIFY_ENTRY_POINT Notify;
} EFI_PEI_NOTIFY_DESCRIPTOR;

typedef union {
  EFI_PEI_NOTIFY_DESCRIPTOR   Notify;
  EFI_PEI_PPI_DESCRIPTOR      Ppi;
} EFI_PEI_DESCRIPTOR;





typedef
EFI_STATUS
(EFIAPI *EFI_PEI_CPU_IO_PPI_IO_MEM)(
  IN     CONST EFI_PEI_SERVICES    **PeiServices,
  IN     CONST EFI_PEI_CPU_IO_PPI  *This,
  IN     EFI_PEI_CPU_IO_PPI_WIDTH  Width,
  IN     UINT64                    Address,
  IN     UINTN                     Count,
  IN OUT VOID                      *Buffer
  );



typedef struct {
  EFI_PEI_CPU_IO_PPI_IO_MEM  Read;
  EFI_PEI_CPU_IO_PPI_IO_MEM  Write;
} EFI_PEI_CPU_IO_PPI_ACCESS;

typedef UINT32 EFI_PEI_CPU_IO_PPI_IO_READ8;
typedef UINT32 EFI_PEI_CPU_IO_PPI_IO_READ8;
typedef UINT32 EFI_PEI_CPU_IO_PPI_IO_READ16;
typedef UINT32 EFI_PEI_CPU_IO_PPI_IO_READ32;
typedef UINT32 EFI_PEI_CPU_IO_PPI_IO_READ64;
typedef UINT32 EFI_PEI_CPU_IO_PPI_IO_WRITE8;
typedef UINT32 EFI_PEI_CPU_IO_PPI_IO_WRITE16;
typedef UINT32 EFI_PEI_CPU_IO_PPI_IO_WRITE32;
typedef UINT32 EFI_PEI_CPU_IO_PPI_IO_WRITE64;
typedef UINT32 EFI_PEI_CPU_IO_PPI_MEM_READ8;
typedef UINT32 EFI_PEI_CPU_IO_PPI_MEM_READ16;
typedef UINT32 EFI_PEI_CPU_IO_PPI_MEM_READ32;
typedef UINT32 EFI_PEI_CPU_IO_PPI_MEM_READ64;
typedef UINT32 EFI_PEI_CPU_IO_PPI_MEM_WRITE8;
typedef UINT32 EFI_PEI_CPU_IO_PPI_MEM_WRITE16;
typedef UINT32 EFI_PEI_CPU_IO_PPI_MEM_WRITE32;
typedef UINT32 EFI_PEI_CPU_IO_PPI_MEM_WRITE64;

typedef struct  _EFI_PEI_CPU_IO_PPI{
  EFI_PEI_CPU_IO_PPI_ACCESS       Mem;
  EFI_PEI_CPU_IO_PPI_ACCESS       Io;
  EFI_PEI_CPU_IO_PPI_IO_READ8     IoRead8;
  EFI_PEI_CPU_IO_PPI_IO_READ16    IoRead16;
  EFI_PEI_CPU_IO_PPI_IO_READ32    IoRead32;
  EFI_PEI_CPU_IO_PPI_IO_READ64    IoRead64;
  EFI_PEI_CPU_IO_PPI_IO_WRITE8    IoWrite8;
  EFI_PEI_CPU_IO_PPI_IO_WRITE16   IoWrite16;
  EFI_PEI_CPU_IO_PPI_IO_WRITE32   IoWrite32;
  EFI_PEI_CPU_IO_PPI_IO_WRITE64   IoWrite64;
  EFI_PEI_CPU_IO_PPI_MEM_READ8    MemRead8;
  EFI_PEI_CPU_IO_PPI_MEM_READ16   MemRead16;
  EFI_PEI_CPU_IO_PPI_MEM_READ32   MemRead32;
  EFI_PEI_CPU_IO_PPI_MEM_READ64   MemRead64;
  EFI_PEI_CPU_IO_PPI_MEM_WRITE8   MemWrite8;
  EFI_PEI_CPU_IO_PPI_MEM_WRITE16  MemWrite16;
  EFI_PEI_CPU_IO_PPI_MEM_WRITE32  MemWrite32;
  EFI_PEI_CPU_IO_PPI_MEM_WRITE64  MemWrite64;
} EFI_PEI_CPU_IO_PPI;

 
#endif
