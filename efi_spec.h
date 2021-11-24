#ifndef EFI_SPEC_H
#define EFI_SPEC_H


#define VOID void
#define IN
#define OUT
#define OPTIONAL
#define EFIAPI

#define MESSAGE L"x86_64\r\n"
typedef uint64_t UINTN;
 
//typedef UINTN EFI_STATUS;

typedef uint8_t   BOOLEAN;
typedef int8_t    INT8;
typedef uint8_t   UINT8;
typedef int16_t   INT16;
typedef uint16_t  UINT16;
typedef int32_t   INT32;
typedef uint32_t  UINT32;
typedef int64_t   INT64;
typedef uint64_t  UINT64;
typedef char      CHAR8;
typedef uint16_t  CHAR16;

typedef struct {
  UINT32  Data1;
  UINT16  Data2;
  UINT16  Data3;
  UINT8   Data4[8];
} GUID;

typedef UINTN RETURN_STATUS;

typedef GUID                      EFI_GUID;
typedef RETURN_STATUS             EFI_STATUS;
typedef VOID                      *EFI_HANDLE;
typedef VOID                      *EFI_EVENT;
typedef UINTN                     EFI_TPL;
typedef UINT64                    EFI_LBA;
typedef UINT64                    EFI_PHYSICAL_ADDRESS;
typedef UINT64                    EFI_VIRTUAL_ADDRESS;


typedef enum {
  EfiResetCold,
  EfiResetWarm,
  EfiResetShutdown,
  EfiResetPlatformSpecific
} EFI_RESET_TYPE;


typedef struct {
  UINT16  Year;
  UINT8   Month;
  UINT8   Day;
  UINT8   Hour;
  UINT8   Minute;
  UINT8   Second;
  UINT8   Pad1;
  UINT32  Nanosecond;
  INT16   TimeZone;
  UINT8   Daylight;
  UINT8   Pad2;
} EFI_TIME;

typedef struct {
  EFI_GUID          CapsuleGuid;
  UINT32            HeaderSize;
  UINT32            Flags;
  UINT32            CapsuleImageSize;
} EFI_CAPSULE_HEADER;

typedef struct {
  UINT32    Resolution;
  UINT32    Accuracy;
  BOOLEAN   SetsToZero;
} EFI_TIME_CAPABILITIES;

typedef struct {
  UINT32                Type;
  EFI_PHYSICAL_ADDRESS  PhysicalStart;
  EFI_VIRTUAL_ADDRESS   VirtualStart;
  UINT64                NumberOfPages;
  UINT64                Attribute;
} EFI_MEMORY_DESCRIPTOR;

typedef enum {
  AllocateAnyPages,
  AllocateMaxAddress,
  AllocateAddress,
  MaxAllocateType
} EFI_ALLOCATE_TYPE;

typedef enum {
  EfiReservedMemoryType,
  EfiLoaderCode,
  EfiLoaderData,
  EfiBootServicesCode,
  EfiBootServicesData,
  EfiRuntimeServicesCode,
  EfiRuntimeServicesData,
  EfiConventionalMemory,
  EfiUnusableMemory,
  EfiACPIReclaimMemory,
  EfiACPIMemoryNVS,
  EfiMemoryMappedIO,
  EfiMemoryMappedIOPortSpace,
  EfiPalCode,
  EfiPersistentMemory,
  EfiMaxMemoryType
} EFI_MEMORY_TYPE;

typedef enum {
  TimerCancel,
  TimerPeriodic,
  TimerRelative
} EFI_TIMER_DELAY;

typedef VOID (EFIAPI *EFI_RESET_SYSTEM)(IN EFI_RESET_TYPE ResetType,IN EFI_STATUS ResetStatus,IN UINTN DataSize,IN VOID  *ResetData OPTIONAL);

typedef EFI_STATUS (EFIAPI *EFI_GET_TIME)(OUT  EFI_TIME *Time, OUT EFI_TIME_CAPABILITIES *Capabilities OPTIONAL);

typedef EFI_STATUS (EFIAPI *EFI_SET_TIME)(IN EFI_TIME *Time);

typedef EFI_STATUS (EFIAPI *EFI_GET_WAKEUP_TIME)(OUT BOOLEAN *Enabled, OUT BOOLEAN *Pending, OUT EFI_TIME *Time);

typedef EFI_STATUS (EFIAPI *EFI_SET_WAKEUP_TIME)(IN BOOLEAN Enable, IN EFI_TIME *Time OPTIONAL);

typedef EFI_STATUS (EFIAPI *EFI_SET_VIRTUAL_ADDRESS_MAP)(IN UINTN MemoryMapSize, IN UINTN DescriptorSize, IN UINT32 DescriptorVersion, IN EFI_MEMORY_DESCRIPTOR *VirtualMap);

typedef EFI_STATUS (EFIAPI *EFI_CONVERT_POINTER)(IN UINTN DebugDisposition, IN OUT VOID **Address);

typedef EFI_STATUS (EFIAPI *EFI_GET_VARIABLE)(IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid, OUT UINT32 *Attributes, OPTIONAL IN OUT UINTN *DataSize, OUT VOID *Data OPTIONAL);

typedef EFI_STATUS (EFIAPI *EFI_GET_NEXT_VARIABLE_NAME)(IN OUT UINTN *VariableNameSize, IN OUT CHAR16 *VariableName, IN OUT EFI_GUID *VendorGuid);

typedef EFI_STATUS (EFIAPI *EFI_SET_VARIABLE)(IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid, IN UINT32 Attributes, IN UINTN DataSize, IN VOID *Data);

typedef EFI_STATUS (EFIAPI *EFI_GET_NEXT_HIGH_MONO_COUNT)(OUT UINT32 *HighCount);

typedef VOID (EFIAPI *EFI_RESET_SYSTEM)(IN EFI_RESET_TYPE ResetType, IN EFI_STATUS ResetStatus, IN UINTN DataSize, IN VOID *ResetData OPTIONAL);

typedef EFI_STATUS (EFIAPI *EFI_UPDATE_CAPSULE)(IN EFI_CAPSULE_HEADER **CapsuleHeaderArray, IN UINTN CapsuleCount, IN EFI_PHYSICAL_ADDRESS ScatterGatherList OPTIONAL);

typedef EFI_STATUS (EFIAPI *EFI_QUERY_CAPSULE_CAPABILITIES)(IN EFI_CAPSULE_HEADER **CapsuleHeaderArray, IN UINTN CapsuleCount, OUT UINT64 *MaximumCapsuleSize, OUT EFI_RESET_TYPE *ResetType);

typedef EFI_STATUS (EFIAPI *EFI_QUERY_VARIABLE_INFO)(IN UINT32 Attributes, OUT UINT64 *MaximumVariableStorageSize, OUT UINT64 *RemainingVariableStorageSize, OUT UINT64 *MaximumVariableSize);

typedef EFI_TPL (EFIAPI *EFI_RAISE_TPL)(IN EFI_TPL NewTpl);

typedef VOID (EFIAPI *EFI_RESTORE_TPL)(IN EFI_TPL OldTpl);

typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_PAGES)(IN EFI_ALLOCATE_TYPE Type, IN EFI_MEMORY_TYPE MemoryType, IN UINTN Pages, IN OUT EFI_PHYSICAL_ADDRESS *Memory);

typedef EFI_STATUS (EFIAPI *EFI_FREE_PAGES)(IN EFI_PHYSICAL_ADDRESS Memory, IN UINTN Pages);

typedef EFI_STATUS (EFIAPI *EFI_GET_MEMORY_MAP)(IN OUT UINTN *MemoryMapSize, OUT EFI_MEMORY_DESCRIPTOR *MemoryMap, OUT UINTN *MapKey, OUT UINTN *DescriptorSize, OUT UINT32 *DescriptorVersion);

typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_POOL)(IN EFI_MEMORY_TYPE PoolType, IN UINTN Size, OUT VOID **Buffer);

typedef EFI_STATUS (EFIAPI *EFI_FREE_POOL)(IN VOID *Buffer);

typedef VOID (EFIAPI *EFI_EVENT_NOTIFY)(IN  EFI_EVENT Event, IN VOID *Context);

typedef EFI_STATUS (EFIAPI *EFI_CREATE_EVENT)(IN UINT32 Type, IN EFI_TPL NotifyTpl,IN EFI_EVENT_NOTIFY NotifyFunction, IN VOID *NotifyContext, OUT EFI_EVENT *Event);

typedef EFI_STATUS (EFIAPI *EFI_SET_TIMER)(IN EFI_EVENT Event, IN EFI_TIMER_DELAY Type, IN UINT64 TriggerTime);

typedef EFI_STATUS (EFIAPI *EFI_WAIT_FOR_EVENT)(IN UINTN NumberOfEvents, IN EFI_EVENT *Event, OUT UINTN *Index);

typedef EFI_STATUS (EFIAPI *EFI_SIGNAL_EVENT)(IN EFI_EVENT Event);

typedef EFI_STATUS (EFIAPI *EFI_CLOSE_EVENT)(IN EFI_EVENT Event);

typedef EFI_STATUS (EFIAPI *EFI_CHECK_EVENT)(IN EFI_EVENT Event);

typedef EFI_STATUS (EFIAPI *EFI_INSTALL_PROTOCOL_INTERFACE)(IN OUT EFI_HANDLE *Handle, IN EFI_GUID *Protocol, IN EFI_INTERFACE_TYPE InterfaceType, IN VOID *Interface);


#endif
