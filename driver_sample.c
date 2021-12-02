#include "efi.h"
#include "log.h"
#include "driver_sample.h"






VOID
EFIAPI
CpuPause (
  VOID
  )
{
  __asm__ __volatile__ ("pause");
}

#define ASSERT(Expression)      \
//  do {                          \
//    if (!(Expression)) {        \
//      CpuPause ();           \
//    }                           \
//  } while (FALSE)


#define ASSERT_EFI_ERROR(Expression)



#define DISPLAY_ONLY_MY_ITEM  0x0002



#define DRIVER_SAMPLE_FORMSET_GUID \
  { \
    0xA04A27f4, 0xDF00, 0x4D42, {0xB5, 0x52, 0x39, 0x51, 0x13, 0x02, 0x11, 0x3D} \
  }

#define DRIVER_SAMPLE_INVENTORY_GUID \
  { \
    0xb3f56470, 0x6141, 0x4621, {0x8f, 0x19, 0x70, 0x4e, 0x57, 0x7a, 0xa9, 0xe8} \
  }

#define EFI_IFR_REFRESH_ID_OP_GUID \
  { \
    0xF5E655D9, 0x02A6, 0x46f2, {0x9E, 0x76, 0xB8, 0xBE, 0x8E, 0x60, 0xAB, 0x22} \
  }

EFI_GUID gDriverSampleFormSetGuid;
EFI_GUID gDriverSampleInventoryGuid;
EFI_GUID gEfiIfrRefreshIdOpGuid;

EFI_GUID gEfiSimpleTextInputExProtocolGuid = {0xdd9e7534, 0x7762, 0x4698, { 0x8c, 0x14, 0xf5, 0x85, 0x17, 0xa6, 0x25, 0xaa } };
EFI_GUID gEfiDevicePathProtocolGuid = { 0x09576E91, 0x6D3F, 0x11D2, { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B }};

EFI_GUID gEfiPrint2SProtocolGuid = { 0xcc252d2, 0xc106, 0x4661, { 0xb5, 0xbd, 0x31, 0x47, 0xa4, 0xf8, 0x1f, 0x92 } };

EFI_GUID gEfiPrint2ProtocolGuid = EFI_PRINT2S_PROTOCOL_GUID;

typedef struct _EFI_PRINT2S_PROTOCOL  EFI_PRINT2S_PROTOCOL;

UINTN
EFIAPI
StrLen (
  IN      CONST CHAR16              *String
  )
{
  UINTN                             Length;

  ASSERT (String != NULL);
  ASSERT (((UINTN) String & BIT0) == 0);

  for (Length = 0; *String != L'\0'; String++, Length++) {
    //
    // If PcdMaximumUnicodeStringLength is not zero,
    // length should not more than PcdMaximumUnicodeStringLength
    //
    //if (PcdGet32 (PcdMaximumUnicodeStringLength) != 0) {
    //  ASSERT (Length < PcdGet32 (PcdMaximumUnicodeStringLength));
    //}
  }
  return Length;
}

UINTN
EFIAPI
StrSize (
  IN      CONST CHAR16              *String
  )
{
  return (StrLen (String) + 1) * sizeof (*String);
}



CHAR16 *
EFIAPI
StrStr (
  IN      CONST CHAR16              *String,
  IN      CONST CHAR16              *SearchString
  )
{
  CONST CHAR16 *FirstMatch;
  CONST CHAR16 *SearchStringTmp;

  //
  // ASSERT both strings are less long than PcdMaximumUnicodeStringLength.
  // Length tests are performed inside StrLen().
  //
  ASSERT (StrSize (String) != 0);
  ASSERT (StrSize (SearchString) != 0);

  if (*SearchString == L'\0') {
    return (CHAR16 *) String;
  }

  while (*String != L'\0') {
    SearchStringTmp = SearchString;
    FirstMatch = String;

    while ((*String == *SearchStringTmp)
            && (*String != L'\0')) {
      String++;
      SearchStringTmp++;
    }

    if (*SearchStringTmp == L'\0') {
      return (CHAR16 *) FirstMatch;
    }

    if (*String == L'\0') {
      return NULL;
    }

    String = FirstMatch + 1;
  }

  return NULL;
}


INTN
EFIAPI
StrnCmp (
  IN      CONST CHAR16              *FirstString,
  IN      CONST CHAR16              *SecondString,
  IN      UINTN                     Length
  )
{
  if (Length == 0) {
    return 0;
  }

  //
  // ASSERT both strings are less long than PcdMaximumUnicodeStringLength.
  // Length tests are performed inside StrLen().
  //
  ASSERT (StrSize (FirstString) != 0);
  ASSERT (StrSize (SecondString) != 0);

  //if (PcdGet32 (PcdMaximumUnicodeStringLength) != 0) {
  //  ASSERT (Length <= PcdGet32 (PcdMaximumUnicodeStringLength));
  //}

  while ((*FirstString != L'\0') &&
         (*SecondString != L'\0') &&
         (*FirstString == *SecondString) &&
         (Length > 1)) {
    FirstString++;
    SecondString++;
    Length--;
  }

  return *FirstString - *SecondString;
}



VOID
EFIAPI
FreePool (
  IN VOID   *Buffer
  )
{
  // Not implemented yet
}

UINTN
EFIAPI
UnicodeSPrint (
  OUT CHAR16        *StartOfBuffer,
  IN  UINTN         BufferSize,
  IN  CONST CHAR16  *FormatString,
  ...
  )
{
 // Not implemented yet
	return 0;
}




UINTN
EFIAPI
StrnLenS (
  IN CONST CHAR16              *String,
  IN UINTN                     MaxSize
  )
{
  UINTN                             Length;

  ASSERT (((UINTN) String & BIT0) == 0);

  //
  // If String is a null pointer or MaxSize is 0, then the StrnLenS function returns zero.
  //
  if ((String == NULL) || (MaxSize == 0)) {
    return 0;
  }

  //
  // Otherwise, the StrnLenS function returns the number of characters that precede the
  // terminating null character. If there is no null character in the first MaxSize characters of
  // String then StrnLenS returns MaxSize. At most the first MaxSize characters of String shall
  // be accessed by StrnLenS.
  //
  Length = 0;
  while (String[Length] != 0) {
    if (Length >= MaxSize - 1) {
      return MaxSize;
    }
    Length++;
  }
  return Length;
}

RETURN_STATUS
EFIAPI
StrCpyS (
  OUT CHAR16       *Destination,
  IN  UINTN        DestMax,
  IN  CONST CHAR16 *Source
  )
{
  UINTN            SourceLen;

  ASSERT (((UINTN) Destination & BIT0) == 0);
  ASSERT (((UINTN) Source & BIT0) == 0);

  //
  // 1. Neither Destination nor Source shall be a null pointer.
  //
  //SAFE_STRING_CONSTRAINT_CHECK ((Destination != NULL), RETURN_INVALID_PARAMETER);
  //SAFE_STRING_CONSTRAINT_CHECK ((Source != NULL), RETURN_INVALID_PARAMETER);

  //
  // 2. DestMax shall not be greater than RSIZE_MAX.
  //
  if (RSIZE_MAX != 0) {
    //SAFE_STRING_CONSTRAINT_CHECK ((DestMax <= RSIZE_MAX), RETURN_INVALID_PARAMETER);
  }

  //
  // 3. DestMax shall not equal zero.
  //
  //SAFE_STRING_CONSTRAINT_CHECK ((DestMax != 0), RETURN_INVALID_PARAMETER);

  //
  // 4. DestMax shall be greater than StrnLenS(Source, DestMax).
  //
  SourceLen = StrnLenS (Source, DestMax);
  //SAFE_STRING_CONSTRAINT_CHECK ((DestMax > SourceLen), RETURN_BUFFER_TOO_SMALL);

  //
  // 5. Copying shall not take place between objects that overlap.
  //
  //SAFE_STRING_CONSTRAINT_CHECK (InternalSafeStringNoStrOverlap (Destination, DestMax, (CHAR16 *)Source, SourceLen + 1), RETURN_ACCESS_DENIED);

  //
  // The StrCpyS function copies the string pointed to by Source (including the terminating
  // null character) into the array pointed to by Destination.
  //
  while (*Source != 0) {
    *(Destination++) = *(Source++);
  }
  *Destination = 0;

  return RETURN_SUCCESS;
}




CHAR16
EFIAPI
CharToUpper (
  IN      CHAR16                    Char
  )
{
  if (Char >= L'a' && Char <= L'z') {
    return (CHAR16) (Char - (L'a' - L'A'));
  }

  return Char;
}


BOOLEAN
InternalIsDecimalDigitCharacter (
  CHAR16                    Char
  )
{
  return (BOOLEAN) (Char >= L'0' && Char <= L'9');
}

BOOLEAN
InternalIsHexaDecimalDigitCharacter (
  CHAR16                    Char
  )
{

  return (BOOLEAN) (InternalIsDecimalDigitCharacter (Char) ||
    (Char >= L'A' && Char <= L'F') ||
    (Char >= L'a' && Char <= L'f'));
}

UINTN
EFIAPI
InternalHexCharToUintn (
  IN      CHAR16                    Char
  )
{
  if (InternalIsDecimalDigitCharacter (Char)) {
    return Char - L'0';
  }

  return (10 + CharToUpper (Char) - L'A');
}



CHAR16
InternalCharToUpper (
        CHAR16                    Char
  )
{
  if (Char >= L'a' && Char <= L'z') {
    return (CHAR16) (Char - (L'a' - L'A'));
  }

  return Char;
}


RETURN_STATUS
StrHexToUint64S (
    CONST CHAR16             *String,
         CHAR16             **EndPointer,  OPTIONAL
         UINT64             *Data
  )
{
  ASSERT (((UINTN) String & BIT0) == 0);

  //
  // 1. Neither String nor Data shall be a null pointer.
  //
  //SAFE_STRING_CONSTRAINT_CHECK ((String != NULL), RETURN_INVALID_PARAMETER);
  //SAFE_STRING_CONSTRAINT_CHECK ((Data != NULL), RETURN_INVALID_PARAMETER);

  //
  // 2. The length of String shall not be greater than RSIZE_MAX.
  //
  if (RSIZE_MAX != 0) {
    //SAFE_STRING_CONSTRAINT_CHECK ((StrnLenS (String, RSIZE_MAX + 1) <= RSIZE_MAX), RETURN_INVALID_PARAMETER);
  }

  if (EndPointer != NULL) {
    *EndPointer = (CHAR16 *) String;
  }

  //
  // Ignore the pad spaces (space or tab)
  //
  while ((*String == L' ') || (*String == L'\t')) {
    String++;
  }

  //
  // Ignore leading Zeros after the spaces
  //
  while (*String == L'0') {
    String++;
  }

  if (InternalCharToUpper (*String) == L'X') {
    if (*(String - 1) != L'0') {
      *Data = 0;
      return RETURN_SUCCESS;
    }
    //
    // Skip the 'X'
    //
    String++;
  }

  *Data = 0;

  while (InternalIsHexaDecimalDigitCharacter (*String)) {
    //
    // If the number represented by String overflows according to the range
    // defined by UINT64, then MAX_UINT64 is stored in *Data and
    // RETURN_UNSUPPORTED is returned.
    //
    if (*Data > ((MAX_UINT64 - InternalHexCharToUintn (*String))>>4)) {
      *Data = MAX_UINT64;
      if (EndPointer != NULL) {
        *EndPointer = (CHAR16 *) String;
      }
      return RETURN_UNSUPPORTED;
    }

    *Data =  ((*Data) << 4) + InternalHexCharToUintn (*String);
    String++;
  }

  if (EndPointer != NULL) {
    *EndPointer = (CHAR16 *) String;
  }
  return RETURN_SUCCESS;
}



UINT64
StrHexToUint64 (
  CONST CHAR16             *String
  )
{
  UINT64    Result;

  StrHexToUint64S (String, (CHAR16 **) NULL, &Result);
  return Result;
}


VOID
ZeroMem (
  IN VOID   *Buffer,
  IN UINTN  Size
  )
{
  INT8  *Ptr;

  Ptr = Buffer;
  while (Size--) {
    *(Ptr++) = 0;
  }
}

VOID*
CopyMem (
  IN VOID   *Destination,
  IN CONST VOID   *Source,
  IN UINTN  Length
  )
{
  CHAR8 *Destination8;
  CONST CHAR8 *Source8;

  Destination8  = Destination;
  Source8       = Source;
  while (Length--) {
    *(Destination8++) = *(Source8++);
  }

  return Destination8;
}

UINT64
EFIAPI
LShiftU64 (
  IN      UINT64                    Operand,
  IN      UINTN                     Count
  )
{
  return Operand << Count;
}

VOID *
EFIAPI
InternalMemSetMem (
  OUT     VOID                      *Buffer,
  IN      UINTN                     Length,
  IN      UINT8                     Value
  )
{
  //
  // Declare the local variables that actually move the data elements as
  // volatile to prevent the optimizer from replacing this function with
  // the intrinsic memset()
  //
  volatile UINT8                    *Pointer8;
  volatile UINT32                   *Pointer32;
  volatile UINT64                   *Pointer64;
  UINT32                            Value32;
  UINT64                            Value64;

  if ((((UINTN)Buffer & 0x7) == 0) && (Length >= 8)) {
    // Generate the 64bit value
    Value32 = (Value << 24) | (Value << 16) | (Value << 8) | Value;
    Value64 = LShiftU64 (Value32, 32) | Value32;

    Pointer64 = (UINT64*)Buffer;
    while (Length >= 8) {
      *(Pointer64++) = Value64;
      Length -= 8;
    }

    // Finish with bytes if needed
    Pointer8 = (UINT8*)Pointer64;
  } else if ((((UINTN)Buffer & 0x3) == 0) && (Length >= 4)) {
    // Generate the 32bit value
    Value32 = (Value << 24) | (Value << 16) | (Value << 8) | Value;

    Pointer32 = (UINT32*)Buffer;
    while (Length >= 4) {
      *(Pointer32++) = Value32;
      Length -= 4;
    }

    // Finish with bytes if needed
    Pointer8 = (UINT8*)Pointer32;
  } else {
    Pointer8 = (UINT8*)Buffer;
  }
  while (Length-- > 0) {
    *(Pointer8++) = Value;
  }
  return Buffer;
}


VOID *
EFIAPI
SetMem (
  OUT VOID  *Buffer,
  IN UINTN  Length,
  IN UINT8  Value
  )
{
  if (Length == 0) {
    return Buffer;
  }

  ASSERT ((Length - 1) <= (MAX_ADDRESS - (UINTN)Buffer));

  return InternalMemSetMem (Buffer, Length, Value);
}

void* memcpy (void *dest, const void *src, unsigned int count)
{
  return CopyMem (dest, src, (UINTN)count);
}

void* memset (void *dest, char ch, unsigned int count)
{
  return SetMem (dest, count, ch);
}


VOID *
AllocateZeroPool (
  UINTN  AllocationSize
  );



VOID *
AllocateZeroPool (
  UINTN  AllocationSize
  )
{
  VOID * Memory;
  Memory = malloc(AllocationSize);
  ASSERT (Memory != NULL);
  if (Memory == NULL) {
   // shirish fprintf(stderr, "Not memory for malloc\n");
  }
  memset(Memory, 0, AllocationSize);
  return Memory;
}


CHAR16     VariableName[] = L"MyIfrNVData";
CHAR16     MyEfiVar[] = L"MyEfiVar";
CHAR16     MyEfiBitVar[] = L"MyEfiBitVar";
CHAR16     MyEfiUnionVar[] = L"MyEfiUnionVar";

EFI_HANDLE                      DriverHandle[2] = {NULL, NULL};
DRIVER_SAMPLE_PRIVATE_DATA      *mPrivateData = NULL;
EFI_EVENT                       mEvent;

HII_VENDOR_DEVICE_PATH  mHiiVendorDevicePath0 = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    DRIVER_SAMPLE_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

HII_VENDOR_DEVICE_PATH  mHiiVendorDevicePath1 = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    DRIVER_SAMPLE_INVENTORY_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

/**
  Set value of a data element in an Array by its Index.

  @param  Array                  The data array.
  @param  Type                   Type of the data in this array.
  @param  Index                  Zero based index for data in this array.
  @param  Value                  The value to be set.

**/
VOID
SetArrayData (
  IN VOID                     *Array,
  IN UINT8                    Type,
  IN UINTN                    Index,
  IN UINT64                   Value
  )
{

  ASSERT (Array != NULL);

  switch (Type) {
  case EFI_IFR_TYPE_NUM_SIZE_8:
    *(((UINT8 *) Array) + Index) = (UINT8) Value;
    break;

  case EFI_IFR_TYPE_NUM_SIZE_16:
    *(((UINT16 *) Array) + Index) = (UINT16) Value;
    break;

  case EFI_IFR_TYPE_NUM_SIZE_32:
    *(((UINT32 *) Array) + Index) = (UINT32) Value;
    break;

  case EFI_IFR_TYPE_NUM_SIZE_64:
    *(((UINT64 *) Array) + Index) = (UINT64) Value;
    break;

  default:
    break;
  }
}

EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
DevicePathFromHandle (
  IN EFI_HANDLE                      Handle
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  EFI_STATUS                Status;

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID *) &DevicePath
                  );
  if (EFI_ERROR (Status)) {
    DevicePath = NULL;
  }
  return DevicePath;
}


UINT8
DevicePathType (
   CONST VOID  *Node
  )
{
  ASSERT (Node != NULL);
  return ((EFI_DEVICE_PATH_PROTOCOL *)(Node))->Type;
}

BOOLEAN
IsDevicePathEndType (
   CONST VOID  *Node
  )
{
  ASSERT (Node != NULL);
  return (BOOLEAN) (DevicePathType (Node) == END_DEVICE_PATH_TYPE);
}

UINT8
DevicePathSubType (
   CONST VOID  *Node
  )
{
  ASSERT (Node != NULL);
  return ((EFI_DEVICE_PATH_PROTOCOL *)(Node))->SubType;
}

BOOLEAN
IsDevicePathEnd (
   CONST VOID  *Node
  )
{
  ASSERT (Node != NULL);
  return (BOOLEAN) (IsDevicePathEndType (Node) && DevicePathSubType(Node) == END_ENTIRE_DEVICE_PATH_SUBTYPE);
}

UINT16
ReadUnaligned16 (
  CONST UINT16              *Buffer
  )
{
  ASSERT (Buffer != NULL);

  return *Buffer;
}

UINTN
DevicePathNodeLength (
   CONST VOID  *Node
  )
{
  ASSERT (Node != NULL);
  return ReadUnaligned16 ((UINT16 *)&((EFI_DEVICE_PATH_PROTOCOL *)(Node))->Length[0]);
}

EFI_DEVICE_PATH_PROTOCOL *
NextDevicePathNode (
   CONST VOID  *Node
  )
{
  ASSERT (Node != NULL);
  return (EFI_DEVICE_PATH_PROTOCOL *)((UINT8 *)(Node) + DevicePathNodeLength(Node));
}

BOOLEAN
IsDevicePathValid (
   CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
   UINTN                    MaxSize
  )
{
  UINTN Count;
  UINTN Size;
  UINTN NodeLength;

  //
  //  Validate the input whether exists and its size big enough to touch the first node
  //
  if (DevicePath == NULL || (MaxSize > 0 && MaxSize < END_DEVICE_PATH_LENGTH)) {
    return FALSE;
  }

  if (MaxSize == 0) {
    MaxSize = MAX_UINT32;
  }

  for (Count = 0, Size = 0; !IsDevicePathEnd (DevicePath); DevicePath = NextDevicePathNode (DevicePath)) {
    NodeLength = DevicePathNodeLength (DevicePath);
    if (NodeLength < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
      return FALSE;
    }

    if (NodeLength > MAX_UINT32 - Size) {
      return FALSE;
    }
    Size += NodeLength;

    //
    // Validate next node before touch it.
    //
    if (Size > MaxSize - END_DEVICE_PATH_LENGTH ) {
      return FALSE;
    }

    Count++;
    if (Count >= MAX_DEVICE_PATH_NODE_COUNT) {
      return FALSE;
      }

  }

  //
  // Only return TRUE when the End Device Path node is valid.
  //
  return (BOOLEAN) (DevicePathNodeLength (DevicePath) == END_DEVICE_PATH_LENGTH);
}


UINTN
UefiDevicePathLibGetDevicePathSize (
   CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  CONST EFI_DEVICE_PATH_PROTOCOL  *Start;

  if (DevicePath == NULL) {
    return 0;
  }

  if (!IsDevicePathValid (DevicePath, 0)) {
    return 0;
  }

  //
  // Search for the end of the device path structure
  //
  Start = DevicePath;
  while (!IsDevicePathEnd (DevicePath)) {
    DevicePath = NextDevicePathNode (DevicePath);
  }

  //
  // Compute the size and add back in the size of the end device path structure
  //
  return ((UINTN) DevicePath - (UINTN) Start) + DevicePathNodeLength (DevicePath);
}



CONST EFI_PRINT2S_PROTOCOL mPrint2SProtocol = {
  UnicodeBSPrint,
  UnicodeSPrint,
  UnicodeBSPrintAsciiFormat,
  UnicodeSPrintAsciiFormat,
  UnicodeValueToStringS,
  AsciiBSPrint,
  AsciiSPrint,
  AsciiBSPrintUnicodeFormat,
  AsciiSPrintUnicodeFormat,
  AsciiValueToStringS
};

UINTN
EFIAPI
UnicodeBSPrint (
  OUT CHAR16        *StartOfBuffer,
  IN  UINTN         BufferSize,
  IN  CONST CHAR16  *FormatString,
  IN  BASE_LIST     Marker
  )
{
  ASSERT_UNICODE_BUFFER (StartOfBuffer);
  ASSERT_UNICODE_BUFFER (FormatString);
  return mPrint2SProtocol->UnicodeBSPrint (StartOfBuffer, BufferSize, FormatString, Marker);
}


RETURN_STATUS
EFIAPI
UnicodeValueToStringS (
  IN OUT CHAR16  *Buffer,
  IN UINTN       BufferSize,
  IN UINTN       Flags,
  IN INT64       Value,
  IN UINTN       Width
  )
{
  return mPrint2SProtocol->UnicodeValueToStringS (Buffer, BufferSize, Flags, Value, Width);
}

UINTN
EFIAPI
UnicodeVSPrintAsciiFormat (
  OUT CHAR16       *StartOfBuffer,
  IN  UINTN        BufferSize,
  IN  CONST CHAR8  *FormatString,
  IN  VA_LIST      Marker
  )
{
  UINT64   BaseListMarker[256 / sizeof (UINT64)];
  BOOLEAN  Converted;

  ASSERT_UNICODE_BUFFER (StartOfBuffer);

  Converted = DxePrintLibPrint2ProtocolVaListToBaseList (
                TRUE,
                FormatString,
                Marker,
                (BASE_LIST)BaseListMarker,
                sizeof (BaseListMarker) - 8
                );
  if (!Converted) {
    return 0;
  }

  return UnicodeBSPrintAsciiFormat (StartOfBuffer, BufferSize, FormatString, (BASE_LIST)BaseListMarker);
}

UINTN
EFIAPI
UnicodeSPrintAsciiFormat (
  OUT CHAR16       *StartOfBuffer,
  IN  UINTN        BufferSize,
  IN  CONST CHAR8  *FormatString,
  ...
  )
{
  VA_LIST Marker;
  UINTN   NumberOfPrinted;

  VA_START (Marker, FormatString);
  NumberOfPrinted = UnicodeVSPrintAsciiFormat (StartOfBuffer, BufferSize, FormatString, Marker);
  VA_END (Marker);
  return NumberOfPrinted;
}

UINTN
EFIAPI
UnicodeBSPrintAsciiFormat (
  OUT CHAR16       *StartOfBuffer,
  IN  UINTN        BufferSize,
  IN  CONST CHAR8  *FormatString,
  IN  BASE_LIST    Marker
  )
{
  ASSERT_UNICODE_BUFFER (StartOfBuffer);
  return mPrint2SProtocol->UnicodeBSPrintAsciiFormat (StartOfBuffer, BufferSize, FormatString, Marker);
}



UINTN
GetDevicePathSize (
   CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  return UefiDevicePathLibGetDevicePathSize (DevicePath);
}


EFI_STATUS
EFIAPI
PrintLibConstructor (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS                   Status;

  Status = SystemTable->BootServices->LocateProtocol (
                                        &gEfiPrint2SProtocolGuid,
                                        NULL,
                                        (VOID**) &mPrint2SProtocol
                                        );
  ASSERT_EFI_ERROR (Status);
  ASSERT (mPrint2SProtocol != NULL);

  return Status;
}


EFI_STRING
EFIAPI
HiiConstructConfigHdr (
  IN CONST EFI_GUID  *Guid,  OPTIONAL
  IN CONST CHAR16    *Name,  OPTIONAL
  IN EFI_HANDLE      DriverHandle
  )
{
  UINTN                     NameLength;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  UINTN                     DevicePathSize;
  CHAR16                    *String;
  CHAR16                    *ReturnString;
  UINTN                     Index;
  UINT8                     *Buffer;
  UINTN                     MaxLen;

  //
  // Compute the length of Name in Unicode characters.
  // If Name is NULL, then the length is 0.
  //
  NameLength = 0;
  if (Name != NULL) {
    NameLength = StrLen (Name);
  }

  DevicePath = NULL;
  DevicePathSize = 0;
  //
  // Retrieve DevicePath Protocol associated with DriverHandle
  //
  if (DriverHandle != NULL) {
    DevicePath = DevicePathFromHandle (DriverHandle);
    if (DevicePath == NULL) {
      return NULL;
    }
    //
    // Compute the size of the device path in bytes
    //
    DevicePathSize = GetDevicePathSize (DevicePath);
  }

  //
  // GUID=<HexCh>32&NAME=<Char>NameLength&PATH=<HexChar>DevicePathSize <Null>
  // | 5 | sizeof (EFI_GUID) * 2 | 6 | NameStrLen*4 | 6 | DevicePathSize * 2 | 1 |
  //
  MaxLen = 5 + sizeof (EFI_GUID) * 2 + 6 + NameLength * 4 + 6 + DevicePathSize * 2 + 1;
  String = AllocateZeroPool (MaxLen * sizeof (CHAR16));
  if (String == NULL) {
    return NULL;
  }

  //
  // Start with L"GUID="
  //
  StrCpyS (String, MaxLen, L"GUID=");
  ReturnString = String;
  String += StrLen (String);

  if (Guid != NULL) {
    //
    // Append Guid converted to <HexCh>32
    //
    for (Index = 0, Buffer = (UINT8 *)Guid; Index < sizeof (EFI_GUID); Index++) {
      UnicodeValueToStringS (
        String,
        MaxLen * sizeof (CHAR16) - ((UINTN)String - (UINTN)ReturnString),
        PREFIX_ZERO | RADIX_HEX,
        *(Buffer++),
        2
        );
      String += StrnLenS (String, MaxLen - ((UINTN)String - (UINTN)ReturnString) / sizeof (CHAR16));
    }
  }

  //
  // Append L"&NAME="
  //
  StrCatS (ReturnString, MaxLen, L"&NAME=");
  String += StrLen (String);

  if (Name != NULL) {
    //
    // Append Name converted to <Char>NameLength
    //
    for (; *Name != L'\0'; Name++) {
      UnicodeValueToStringS (
        String,
        sizeof (CHAR16) * MaxLen - ((UINTN)String - (UINTN)ReturnString),
        PREFIX_ZERO | RADIX_HEX,
        *Name,
        4
        );
      String += StrnLenS (String, MaxLen - ((UINTN)String - (UINTN)ReturnString) / sizeof (CHAR16));
    }
  }

  //
  // Append L"&PATH="
  //
  StrCatS (ReturnString, MaxLen, L"&PATH=");
  String += StrLen (String);

  //
  // Append the device path associated with DriverHandle converted to <HexChar>DevicePathSize
  //
  for (Index = 0, Buffer = (UINT8 *)DevicePath; Index < DevicePathSize; Index++) {
    UnicodeValueToStringS (
      String,
      sizeof (CHAR16) * MaxLen - ((UINTN)String - (UINTN)ReturnString),
      PREFIX_ZERO | RADIX_HEX,
      *(Buffer++),
      2
      );
    String += StrnLenS (String, MaxLen - ((UINTN)String - (UINTN)ReturnString) / sizeof (CHAR16));
  }

  //
  // Null terminate the Unicode string
  //
  *String = L'\0';

  //
  // Convert all hex digits in range [A-F] in the configuration header to [a-f]
  //
  return InternalHiiLowerConfigString (ReturnString);
}

BOOLEAN
EFIAPI
InternalHiiCompareSubString (
  IN CHAR16  *FirstString,
  IN CHAR16  *SecondString,
  IN CHAR16  *StartSearchString,
  IN CHAR16  *StopSearchString
  )
{
  CHAR16  *EndFirstString;
  CHAR16  *EndSecondString;

  ASSERT (FirstString != NULL);
  ASSERT (SecondString != NULL);
  ASSERT (StartSearchString != NULL);
  ASSERT (StopSearchString != NULL);

  FirstString = StrStr (FirstString, StartSearchString);
  if (FirstString == NULL) {
    return FALSE;
  }

  SecondString = StrStr (SecondString, StartSearchString);
  if (SecondString == NULL) {
    return FALSE;
  }

  EndFirstString = StrStr (FirstString, StopSearchString);
  if (EndFirstString == NULL) {
    return FALSE;
  }

  EndSecondString = StrStr (SecondString, StopSearchString);
  if (EndSecondString == NULL) {
    return FALSE;
  }

  if ((EndFirstString - FirstString) != (EndSecondString - SecondString)) {
    return FALSE;
  }

  return (BOOLEAN)(StrnCmp (FirstString, SecondString, EndFirstString - FirstString) == 0);
}

BOOLEAN
EFIAPI
HiiIsConfigHdrMatch (
  IN CONST EFI_STRING  ConfigHdr,
  IN CONST EFI_GUID    *Guid,     OPTIONAL
  IN CONST CHAR16      *Name      OPTIONAL
  )
{
  EFI_STRING  CompareConfigHdr;
  BOOLEAN     Result;

  ASSERT (ConfigHdr != NULL);

  //
  // Use Guid and Name to generate a <ConfigHdr> string
  //
  CompareConfigHdr = HiiConstructConfigHdr (Guid, Name, NULL);
  if (CompareConfigHdr == NULL) {
    return FALSE;
  }

  Result = TRUE;
  if (Guid != NULL) {
    //
    // Compare GUID value strings
    //
    Result = InternalHiiCompareSubString (ConfigHdr, CompareConfigHdr, L"GUID=", L"&NAME=");
  }

  if (Result && Name != NULL) {
    //
    // Compare NAME value strings
    //
    Result = InternalHiiCompareSubString (ConfigHdr, CompareConfigHdr, L"&NAME=", L"&PATH=");
  }

  //
  // Free the <ConfigHdr> string
  //
  FreePool (CompareConfigHdr);

  return Result;
}

/**
  Notification function for keystrokes.

  @param[in] KeyData    The key that was pressed.

  @retval EFI_SUCCESS   The operation was successful.
**/
EFI_STATUS
EFIAPI
NotificationFunction(
  IN EFI_KEY_DATA *KeyData
  )
{
  gBS->SignalEvent (mEvent);

  return EFI_SUCCESS;
}

/**
  Function to start monitoring for CTRL-C using SimpleTextInputEx.

  @retval EFI_SUCCESS           The feature is enabled.
  @retval EFI_OUT_OF_RESOURCES  There is not enough mnemory available.
**/
EFI_STATUS
EFIAPI
InternalStartMonitor(
  VOID
  )
{
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx;
  EFI_KEY_DATA                      KeyData;
  EFI_STATUS                        Status;
  EFI_HANDLE                        *Handles;
  UINTN                             HandleCount;
  UINTN                             HandleIndex;
  VOID                              *NotifyHandle;

  Status = gBS->LocateHandleBuffer (
              ByProtocol,
              &gEfiSimpleTextInputExProtocolGuid,
              NULL,
              &HandleCount,
              &Handles
              );
  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (Handles[HandleIndex], &gEfiSimpleTextInputExProtocolGuid, (VOID **) &SimpleEx);
    ASSERT_EFI_ERROR (Status);

    KeyData.KeyState.KeyToggleState = 0;
    KeyData.Key.ScanCode            = 0;
    KeyData.KeyState.KeyShiftState  = EFI_SHIFT_STATE_VALID|EFI_LEFT_CONTROL_PRESSED;
    KeyData.Key.UnicodeChar         = L'c';

    Status = SimpleEx->RegisterKeyNotify(
      SimpleEx,
      &KeyData,
      NotificationFunction,
      &NotifyHandle);
    if (EFI_ERROR (Status)) {
      break;
    }

    KeyData.KeyState.KeyShiftState  = EFI_SHIFT_STATE_VALID|EFI_RIGHT_CONTROL_PRESSED;
    Status = SimpleEx->RegisterKeyNotify(
      SimpleEx,
      &KeyData,
      NotificationFunction,
      &NotifyHandle);
    if (EFI_ERROR (Status)) {
      break;
    }
  }

  return EFI_SUCCESS;
}

/**
  Function to stop monitoring for CTRL-C using SimpleTextInputEx.

  @retval EFI_SUCCESS           The feature is enabled.
  @retval EFI_OUT_OF_RESOURCES  There is not enough mnemory available.
**/
EFI_STATUS
EFIAPI
InternalStopMonitor(
  VOID
  )
{
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx;
  EFI_STATUS                        Status;
  EFI_HANDLE                        *Handles;
  EFI_KEY_DATA                      KeyData;
  UINTN                             HandleCount;
  UINTN                             HandleIndex;
  VOID                              *NotifyHandle;

  Status = gBS->LocateHandleBuffer (
                ByProtocol,
                &gEfiSimpleTextInputExProtocolGuid,
                NULL,
                &HandleCount,
                &Handles
                );
  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (Handles[HandleIndex], &gEfiSimpleTextInputExProtocolGuid, (VOID **) &SimpleEx);
    ASSERT_EFI_ERROR (Status);

    KeyData.KeyState.KeyToggleState = 0;
    KeyData.Key.ScanCode            = 0;
    KeyData.KeyState.KeyShiftState  = EFI_SHIFT_STATE_VALID|EFI_LEFT_CONTROL_PRESSED;
    KeyData.Key.UnicodeChar         = L'c';

    Status = SimpleEx->RegisterKeyNotify(
      SimpleEx,
      &KeyData,
      NotificationFunction,
      &NotifyHandle);
    if (!EFI_ERROR (Status)) {
      Status = SimpleEx->UnregisterKeyNotify (SimpleEx, NotifyHandle);
    }

    KeyData.KeyState.KeyShiftState  = EFI_SHIFT_STATE_VALID|EFI_RIGHT_CONTROL_PRESSED;
    Status = SimpleEx->RegisterKeyNotify(
      SimpleEx,
      &KeyData,
      NotificationFunction,
      &NotifyHandle);
    if (!EFI_ERROR (Status)) {
      Status = SimpleEx->UnregisterKeyNotify (SimpleEx, NotifyHandle);
    }
  }
  return EFI_SUCCESS;
}

/**
 Update names of Name/Value storage to current language.

 @param PrivateData   Points to the driver private data.

 @retval EFI_SUCCESS   All names are successfully updated.
 @retval EFI_NOT_FOUND Failed to get Name from HII database.

**/
EFI_STATUS
LoadNameValueNames (
  IN DRIVER_SAMPLE_PRIVATE_DATA      *PrivateData
  )
{
  UINTN      Index;

  //
  // Get Name/Value name string of current language
  //
  for (Index = 0; Index < NAME_VALUE_NAME_NUMBER; Index++) {
    PrivateData->NameValueName[Index] = HiiGetString (
                                         PrivateData->HiiHandle[0],
                                         PrivateData->NameStringId[Index],
                                         NULL
                                         );
    if (PrivateData->NameValueName[Index] == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  return EFI_SUCCESS;
}


/**
  Get the value of <Number> in <BlockConfig> format, i.e. the value of OFFSET
  or WIDTH or VALUE.
  <BlockConfig> ::= 'OFFSET='<Number>&'WIDTH='<Number>&'VALUE'=<Number>

  This is a internal function.

  @param  StringPtr              String in <BlockConfig> format and points to the
                                 first character of <Number>.
  @param  Number                 The output value. Caller takes the responsibility
                                 to free memory.
  @param  Len                    Length of the <Number>, in characters.

  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to store neccessary
                                 structures.
  @retval EFI_SUCCESS            Value of <Number> is outputted in Number
                                 successfully.

**/
EFI_STATUS
GetValueOfNumber (
  IN EFI_STRING                    StringPtr,
  OUT UINT8                        **Number,
  OUT UINTN                        *Len
  )
{
  EFI_STRING               TmpPtr;
  UINTN                    Length;
  EFI_STRING               Str;
  UINT8                    *Buf;
  EFI_STATUS               Status;
  UINT8                    DigitUint8;
  UINTN                    Index;
  CHAR16                   TemStr[2];

  if (StringPtr == NULL || *StringPtr == L'\0' || Number == NULL || Len == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Buf = NULL;

  TmpPtr = StringPtr;
  while (*StringPtr != L'\0' && *StringPtr != L'&') {
    StringPtr++;
  }
  *Len   = StringPtr - TmpPtr;
  Length = *Len + 1;

  Str = (EFI_STRING) AllocateZeroPool (Length * sizeof (CHAR16));
  if (Str == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  CopyMem (Str, TmpPtr, *Len * sizeof (CHAR16));
  *(Str + *Len) = L'\0';

  Length = (Length + 1) / 2;
  Buf = (UINT8 *) AllocateZeroPool (Length);
  if (Buf == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Length = *Len;
  ZeroMem (TemStr, sizeof (TemStr));
  for (Index = 0; Index < Length; Index ++) {
    TemStr[0] = Str[Length - Index - 1];
    DigitUint8 = (UINT8) StrHexToUint64 (TemStr);
    if ((Index & 1) == 0) {
      Buf [Index/2] = DigitUint8;
    } else {
      Buf [Index/2] = (UINT8) ((DigitUint8 << 4) + Buf [Index/2]);
    }
  }

  *Number = Buf;
  Status  = EFI_SUCCESS;

Exit:
  if (Str != NULL) {
    FreePool (Str);
  }

  return Status;
}

/**
  Create altcfg string.

  @param  Result               The request result string.
  @param  ConfigHdr            The request head info. <ConfigHdr> format.
  @param  Offset               The offset of the parameter int he structure.
  @param  Width                The width of the parameter.


  @retval  The string with altcfg info append at the end.
**/
EFI_STRING
CreateAltCfgString (
  IN     EFI_STRING     Result,
  IN     EFI_STRING     ConfigHdr,
  IN     UINTN          Offset,
  IN     UINTN          Width
  )
{
  EFI_STRING StringPtr;
  EFI_STRING TmpStr;
  UINTN      NewLen;

  NewLen = StrLen (Result);
  //
  // String Len = ConfigResp + AltConfig + AltConfig + 1("\0")
  //
  NewLen = (NewLen + ((1 + StrLen (ConfigHdr) + 8 + 4) + (8 + 4 + 7 + 4 + 7 + 4)) * 2 + 1) * sizeof (CHAR16);
  StringPtr = AllocateZeroPool (NewLen);
  if (StringPtr == NULL) {
    return NULL;
  }

  TmpStr = StringPtr;
  if (Result != NULL) {
    StrCpyS (StringPtr, NewLen / sizeof (CHAR16), Result);
    StringPtr += StrLen (Result);
    FreePool (Result);
  }

  UnicodeSPrint (
  StringPtr,
  (1 + StrLen (ConfigHdr) + 8 + 4 + 1) * sizeof (CHAR16),
  L"&%s&ALTCFG=%04x",
  ConfigHdr,
  EFI_HII_DEFAULT_CLASS_STANDARD
  );
  StringPtr += StrLen (StringPtr);

  UnicodeSPrint (
    StringPtr,
    (8 + 4 + 7 + 4 + 7 + 4 + 1) * sizeof (CHAR16),
    L"&OFFSET=%04x&WIDTH=%04x&VALUE=%04x",
    Offset,
    Width,
    DEFAULT_CLASS_STANDARD_VALUE
    );
  StringPtr += StrLen (StringPtr);

  UnicodeSPrint (
  StringPtr,
  (1 + StrLen (ConfigHdr) + 8 + 4 + 1) * sizeof (CHAR16),
  L"&%s&ALTCFG=%04x",
  ConfigHdr,
  EFI_HII_DEFAULT_CLASS_MANUFACTURING
  );
  StringPtr += StrLen (StringPtr);

  UnicodeSPrint (
    StringPtr,
    (8 + 4 + 7 + 4 + 7 + 4 + 1) * sizeof (CHAR16),
    L"&OFFSET=%04x&WIDTH=%04x&VALUE=%04x",
    Offset,
    Width,
    DEFAULT_CLASS_MANUFACTURING_VALUE
    );
  StringPtr += StrLen (StringPtr);

  return TmpStr;
}

/**
  Check whether need to add the altcfg string. if need to add, add the altcfg
  string.

  @param  RequestResult              The request result string.
  @param  ConfigRequestHdr           The request head info. <ConfigHdr> format.

**/
VOID
AppendAltCfgString (
  IN OUT EFI_STRING                       *RequestResult,
  IN     EFI_STRING                       ConfigRequestHdr
  )
{
  EFI_STRING                          StringPtr;
  UINTN                               Length;
  UINT8                               *TmpBuffer;
  UINTN                               Offset;
  UINTN                               Width;
  UINTN                               BlockSize;
  UINTN                               ValueOffset;
  UINTN                               ValueWidth;
  EFI_STATUS                          Status;

  TmpBuffer = NULL;
  StringPtr = *RequestResult;
  StringPtr = StrStr (StringPtr, L"OFFSET");
  BlockSize = sizeof (DRIVER_SAMPLE_CONFIGURATION);
  ValueOffset = OFFSET_OF (DRIVER_SAMPLE_CONFIGURATION, GetDefaultValueFromAccess);
  ValueWidth  = sizeof (((DRIVER_SAMPLE_CONFIGURATION *)0)->GetDefaultValueFromAccess);

  if (StringPtr == NULL) {
    return;
  }

  while (*StringPtr != 0 && StrnCmp (StringPtr, L"OFFSET=", StrLen (L"OFFSET=")) == 0) {
    StringPtr += StrLen (L"OFFSET=");
    //
    // Get Offset
    //
    Status = GetValueOfNumber (StringPtr, &TmpBuffer, &Length);
    if (EFI_ERROR (Status)) {
      return;
    }
    Offset = 0;
    CopyMem (
     &Offset,
     TmpBuffer,
     (((Length + 1) / 2) < sizeof (UINTN)) ? ((Length + 1) / 2) : sizeof (UINTN)
     );
    FreePool (TmpBuffer);

    StringPtr += Length;
    if (StrnCmp (StringPtr, L"&WIDTH=", StrLen (L"&WIDTH=")) != 0) {
      return;
    }
    StringPtr += StrLen (L"&WIDTH=");

    //
    // Get Width
    //
    Status = GetValueOfNumber (StringPtr, &TmpBuffer, &Length);
    if (EFI_ERROR (Status)) {
      return;
    }
    Width = 0;
    CopyMem (
     &Width,
     TmpBuffer,
     (((Length + 1) / 2) < sizeof (UINTN)) ? ((Length + 1) / 2) : sizeof (UINTN)
     );
    FreePool (TmpBuffer);

    StringPtr += Length;
    if (StrnCmp (StringPtr, L"&VALUE=", StrLen (L"&VALUE=")) != 0) {
      return;
    }
    StringPtr += StrLen (L"&VALUE=");

    //
    // Get Value
    //
    Status = GetValueOfNumber (StringPtr, &TmpBuffer, &Length);
    if (EFI_ERROR (Status)) {
      return;
    }
    StringPtr += Length;

    //
    // Skip the character "&" before "OFFSET".
    //
    StringPtr ++;

    //
    // Calculate Value and convert it to hex string.
    //
    if (Offset + Width > BlockSize) {
      return;
    }

    if (Offset <= ValueOffset && Offset + Width >= ValueOffset + ValueWidth) {
      *RequestResult = CreateAltCfgString(*RequestResult, ConfigRequestHdr, ValueOffset, ValueWidth);
      return;
    }
  }
}

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Request                A null-terminated Unicode string in
                                 <ConfigRequest> format.
  @param  Progress               On return, points to a character in the Request
                                 string. Points to the string's null terminator if
                                 request was successful. Points to the most recent
                                 '&' before the first failing name/value pair (or
                                 the beginning of the string if the failure is in
                                 the first name/value pair) if the request was not
                                 successful.
  @param  Results                A null-terminated Unicode string in
                                 <ConfigAltResp> format which has all values filled
                                 in for the names in the Request string. String to
                                 be allocated by the called function.

  @retval EFI_SUCCESS            The Results is filled with the requested values.
  @retval EFI_OUT_OF_RESOURCES   Not enough memory to store the results.
  @retval EFI_INVALID_PARAMETER  Request is illegal syntax, or unknown name.
  @retval EFI_NOT_FOUND          Routing data doesn't match any storage in this
                                 driver.

**/
EFI_STATUS
EFIAPI
ExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  EFI_STATUS                       Status;
  UINTN                            BufferSize;
  DRIVER_SAMPLE_PRIVATE_DATA       *PrivateData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;
  EFI_STRING                       ConfigRequest;
  EFI_STRING                       ConfigRequestHdr;
  UINTN                            Size;
  EFI_STRING                       Value;
  UINTN                            ValueStrLen;
  CHAR16                           BackupChar;
  CHAR16                           *StrPointer;
  BOOLEAN                          AllocatedRequest;

  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Initialize the local variables.
  //
  ConfigRequestHdr  = NULL;
  ConfigRequest     = NULL;
  Size              = 0;
  *Progress         = Request;
  AllocatedRequest  = FALSE;

  PrivateData = DRIVER_SAMPLE_PRIVATE_FROM_THIS (This);
  HiiConfigRouting = PrivateData->HiiConfigRouting;

  //
  // Get Buffer Storage data from EFI variable.
  // Try to get the current setting from variable.
  //
  BufferSize = sizeof (DRIVER_SAMPLE_CONFIGURATION);
  Status = gRT->GetVariable (
            VariableName,
            &gDriverSampleFormSetGuid,
            NULL,
            &BufferSize,
            &PrivateData->Configuration
            );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  if (Request == NULL) {
    //
    // Request is set to NULL, construct full request string.
    //

    //
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&gDriverSampleFormSetGuid, VariableName, PrivateData->DriverHandle[0]);
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    ASSERT (ConfigRequest != NULL);
    AllocatedRequest = TRUE;
    UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64)BufferSize);
    FreePool (ConfigRequestHdr);
    ConfigRequestHdr = NULL;
  } else {
    //
    // Check routing data in <ConfigHdr>.
    // Note: if only one Storage is used, then this checking could be skipped.
    //
    if (!HiiIsConfigHdrMatch (Request, &gDriverSampleFormSetGuid, NULL)) {
      return EFI_NOT_FOUND;
    }
    //
    // Check whether request for EFI Varstore. EFI varstore get data
    // through hii database, not support in this path.
    //
    if (HiiIsConfigHdrMatch(Request, &gDriverSampleFormSetGuid, MyEfiVar)) {
      return EFI_UNSUPPORTED;
    }
    if (HiiIsConfigHdrMatch(Request, &gDriverSampleFormSetGuid, MyEfiBitVar)) {
      return EFI_UNSUPPORTED;
    }
    if (HiiIsConfigHdrMatch(Request, &gDriverSampleFormSetGuid, MyEfiUnionVar)) {
      return EFI_UNSUPPORTED;
    }

    //
    // Set Request to the unified request string.
    //
    ConfigRequest = Request;
    //
    // Check whether Request includes Request Element.
    //
    if (StrStr (Request, L"OFFSET") == NULL) {
      //
      // Check Request Element does exist in Reques String
      //
      StrPointer = StrStr (Request, L"PATH");
      if (StrPointer == NULL) {
        return EFI_INVALID_PARAMETER;
      }
      if (StrStr (StrPointer, L"&") == NULL) {
        Size = (StrLen (Request) + 32 + 1) * sizeof (CHAR16);
        ConfigRequest    = AllocateZeroPool (Size);
        ASSERT (ConfigRequest != NULL);
        AllocatedRequest = TRUE;
        UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", Request, (UINT64)BufferSize);
      }
    }
  }

  //
  // Check if requesting Name/Value storage
  //
  if (StrStr (ConfigRequest, L"OFFSET") == NULL) {
    //
    // Update Name/Value storage Names
    //
    Status = LoadNameValueNames (PrivateData);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    // Allocate memory for <ConfigResp>, e.g. Name0=0x11, Name1=0x1234, Name2="ABCD"
    // <Request>   ::=<ConfigHdr>&Name0&Name1&Name2
    // <ConfigResp>::=<ConfigHdr>&Name0=11&Name1=1234&Name2=0041004200430044
    //
    BufferSize = (StrLen (ConfigRequest) +
      1 + sizeof (PrivateData->Configuration.NameValueVar0) * 2 +
      1 + sizeof (PrivateData->Configuration.NameValueVar1) * 2 +
      1 + sizeof (PrivateData->Configuration.NameValueVar2) * 2 + 1) * sizeof (CHAR16);
    *Results = AllocateZeroPool (BufferSize);
    ASSERT (*Results != NULL);
    StrCpyS (*Results, BufferSize / sizeof (CHAR16), ConfigRequest);
    Value = *Results;

    //
    // Append value of NameValueVar0, type is UINT8
    //
    if ((Value = StrStr (*Results, PrivateData->NameValueName[0])) != NULL) {
      Value += StrLen (PrivateData->NameValueName[0]);
      ValueStrLen = ((sizeof (PrivateData->Configuration.NameValueVar0) * 2) + 1);
      CopyMem (Value + ValueStrLen, Value, StrSize (Value));

      BackupChar = Value[ValueStrLen];
      *Value++   = L'=';
      UnicodeValueToStringS (
        Value,
        BufferSize - ((UINTN)Value - (UINTN)*Results),
        PREFIX_ZERO | RADIX_HEX,
        PrivateData->Configuration.NameValueVar0,
        sizeof (PrivateData->Configuration.NameValueVar0) * 2
        );
      Value += StrnLenS (Value, (BufferSize - ((UINTN)Value - (UINTN)*Results)) / sizeof (CHAR16));
      *Value = BackupChar;
    }

    //
    // Append value of NameValueVar1, type is UINT16
    //
    if ((Value = StrStr (*Results, PrivateData->NameValueName[1])) != NULL) {
      Value += StrLen (PrivateData->NameValueName[1]);
      ValueStrLen = ((sizeof (PrivateData->Configuration.NameValueVar1) * 2) + 1);
      CopyMem (Value + ValueStrLen, Value, StrSize (Value));

      BackupChar = Value[ValueStrLen];
      *Value++   = L'=';
      UnicodeValueToStringS (
        Value,
        BufferSize - ((UINTN)Value - (UINTN)*Results),
        PREFIX_ZERO | RADIX_HEX,
        PrivateData->Configuration.NameValueVar1,
        sizeof (PrivateData->Configuration.NameValueVar1) * 2
        );
      Value += StrnLenS (Value, (BufferSize - ((UINTN)Value - (UINTN)*Results)) / sizeof (CHAR16));
      *Value = BackupChar;
    }

    //
    // Append value of NameValueVar2, type is CHAR16 *
    //
    if ((Value = StrStr (*Results, PrivateData->NameValueName[2])) != NULL) {
      Value += StrLen (PrivateData->NameValueName[2]);
      ValueStrLen = StrLen (PrivateData->Configuration.NameValueVar2) * 4 + 1;
      CopyMem (Value + ValueStrLen, Value, StrSize (Value));

      *Value++ = L'=';
      //
      // Convert Unicode String to Config String, e.g. "ABCD" => "0041004200430044"
      //
      StrPointer = (CHAR16 *) PrivateData->Configuration.NameValueVar2;
      for (; *StrPointer != L'\0'; StrPointer++) {
        UnicodeValueToStringS (
          Value,
          BufferSize - ((UINTN)Value - (UINTN)*Results),
          PREFIX_ZERO | RADIX_HEX,
          *StrPointer,
          4
          );
        Value += StrnLenS (Value, (BufferSize - ((UINTN)Value - (UINTN)*Results)) / sizeof (CHAR16));
      }
    }

    Status = EFI_SUCCESS;
  } else {
    //
    // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
    //
    Status = HiiConfigRouting->BlockToConfig (
                                  HiiConfigRouting,
                                  ConfigRequest,
                                  (UINT8 *) &PrivateData->Configuration,
                                  BufferSize,
                                  Results,
                                  Progress
                                  );
    if (!EFI_ERROR (Status)) {
      ConfigRequestHdr = HiiConstructConfigHdr (&gDriverSampleFormSetGuid, VariableName, PrivateData->DriverHandle[0]);
      AppendAltCfgString(Results, ConfigRequestHdr);
    }
  }

  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
  }

  if (ConfigRequestHdr != NULL) {
    FreePool (ConfigRequestHdr);
  }
  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = Request + StrLen (Request);
  }

  return Status;
}


/**
  This function processes the results of changes in configuration.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Configuration          A null-terminated Unicode string in <ConfigResp>
                                 format.
  @param  Progress               A pointer to a string filled in with the offset of
                                 the most recent '&' before the first failing
                                 name/value pair (or the beginning of the string if
                                 the failure is in the first name/value pair) or
                                 the terminating NULL if all was successful.

  @retval EFI_SUCCESS            The Results is processed successfully.
  @retval EFI_INVALID_PARAMETER  Configuration is NULL.
  @retval EFI_NOT_FOUND          Routing data doesn't match any storage in this
                                 driver.

**/
EFI_STATUS
EFIAPI
RouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  EFI_STATUS                       Status;
  UINTN                            BufferSize;
  DRIVER_SAMPLE_PRIVATE_DATA       *PrivateData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;
  CHAR16                           *Value;
  CHAR16                           *StrPtr;
  CHAR16                           TemStr[5];
  UINT8                            *DataBuffer;
  UINT8                            DigitUint8;
  UINTN                            Index;
  CHAR16                           *StrBuffer;

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = DRIVER_SAMPLE_PRIVATE_FROM_THIS (This);
  HiiConfigRouting = PrivateData->HiiConfigRouting;
  *Progress = Configuration;

  //
  // Check routing data in <ConfigHdr>.
  // Note: if only one Storage is used, then this checking could be skipped.
  //
  if (!HiiIsConfigHdrMatch (Configuration, &gDriverSampleFormSetGuid, NULL)) {
    return EFI_NOT_FOUND;
  }

  //
  // Check whether request for EFI Varstore. EFI varstore get data
  // through hii database, not support in this path.
  //
  if (HiiIsConfigHdrMatch(Configuration, &gDriverSampleFormSetGuid, MyEfiVar)) {
    return EFI_UNSUPPORTED;
  }
  if (HiiIsConfigHdrMatch(Configuration, &gDriverSampleFormSetGuid, MyEfiBitVar)) {
    return EFI_UNSUPPORTED;
  }
  if (HiiIsConfigHdrMatch(Configuration, &gDriverSampleFormSetGuid, MyEfiUnionVar)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Get Buffer Storage data from EFI variable
  //
  BufferSize = sizeof (DRIVER_SAMPLE_CONFIGURATION);
  Status = gRT->GetVariable (
            VariableName,
            &gDriverSampleFormSetGuid,
            NULL,
            &BufferSize,
            &PrivateData->Configuration
            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Check if configuring Name/Value storage
  //
  if (StrStr (Configuration, L"OFFSET") == NULL) {
    //
    // Update Name/Value storage Names
    //
    Status = LoadNameValueNames (PrivateData);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    // Convert value for NameValueVar0
    //
    if ((Value = StrStr (Configuration, PrivateData->NameValueName[0])) != NULL) {
      //
      // Skip "Name="
      //
      Value += StrLen (PrivateData->NameValueName[0]);
      Value++;
      //
      // Get Value String
      //
      StrPtr = StrStr (Value, L"&");
      if (StrPtr == NULL) {
        StrPtr = Value + StrLen (Value);
      }
      //
      // Convert Value to Buffer data
      //
      DataBuffer = (UINT8 *) &PrivateData->Configuration.NameValueVar0;
      ZeroMem (TemStr, sizeof (TemStr));
      for (Index = 0, StrPtr --; StrPtr >= Value; StrPtr --, Index ++) {
        TemStr[0] = *StrPtr;
        DigitUint8 = (UINT8) StrHexToUint64 (TemStr);
        if ((Index & 1) == 0) {
          DataBuffer [Index/2] = DigitUint8;
        } else {
          DataBuffer [Index/2] = (UINT8) ((UINT8) (DigitUint8 << 4) + DataBuffer [Index/2]);
        }
      }
    }

    //
    // Convert value for NameValueVar1
    //
    if ((Value = StrStr (Configuration, PrivateData->NameValueName[1])) != NULL) {
      //
      // Skip "Name="
      //
      Value += StrLen (PrivateData->NameValueName[1]);
      Value++;
      //
      // Get Value String
      //
      StrPtr = StrStr (Value, L"&");
      if (StrPtr == NULL) {
        StrPtr = Value + StrLen (Value);
      }
      //
      // Convert Value to Buffer data
      //
      DataBuffer = (UINT8 *) &PrivateData->Configuration.NameValueVar1;
      ZeroMem (TemStr, sizeof (TemStr));
      for (Index = 0, StrPtr --; StrPtr >= Value; StrPtr --, Index ++) {
        TemStr[0] = *StrPtr;
        DigitUint8 = (UINT8) StrHexToUint64 (TemStr);
        if ((Index & 1) == 0) {
          DataBuffer [Index/2] = DigitUint8;
        } else {
          DataBuffer [Index/2] = (UINT8) ((UINT8) (DigitUint8 << 4) + DataBuffer [Index/2]);
        }
      }
    }

    //
    // Convert value for NameValueVar2
    //
    if ((Value = StrStr (Configuration, PrivateData->NameValueName[2])) != NULL) {
      //
      // Skip "Name="
      //
      Value += StrLen (PrivateData->NameValueName[2]);
      Value++;
      //
      // Get Value String
      //
      StrPtr = StrStr (Value, L"&");
      if (StrPtr == NULL) {
        StrPtr = Value + StrLen (Value);
      }
      //
      // Convert Config String to Unicode String, e.g "0041004200430044" => "ABCD"
      //
      StrBuffer = (CHAR16 *) PrivateData->Configuration.NameValueVar2;
      ZeroMem (TemStr, sizeof (TemStr));
      while (Value < StrPtr) {
        StrnCpyS (TemStr, sizeof (TemStr) / sizeof (CHAR16), Value, 4);
        *(StrBuffer++) = (CHAR16) StrHexToUint64 (TemStr);
        Value += 4;
      }
      *StrBuffer = L'\0';
    }

    //
    // Store Buffer Storage back to EFI variable
    //
    Status = gRT->SetVariable(
      VariableName,
      &gDriverSampleFormSetGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
      sizeof (DRIVER_SAMPLE_CONFIGURATION),
      &PrivateData->Configuration
      );

    return Status;
  }

  //
  // Convert <ConfigResp> to buffer data by helper function ConfigToBlock()
  //
  BufferSize = sizeof (DRIVER_SAMPLE_CONFIGURATION);
  Status = HiiConfigRouting->ConfigToBlock (
                               HiiConfigRouting,
                               Configuration,
                               (UINT8 *) &PrivateData->Configuration,
                               &BufferSize,
                               Progress
                               );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Store Buffer Storage back to EFI variable
  //
  Status = gRT->SetVariable(
                  VariableName,
                  &gDriverSampleFormSetGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (DRIVER_SAMPLE_CONFIGURATION),
                  &PrivateData->Configuration
                  );

  return Status;
}


/**
  This function processes the results of changes in configuration.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Action                 Specifies the type of action taken by the browser.
  @param  QuestionId             A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param  Type                   The type of value for the question.
  @param  Value                  A pointer to the data being sent to the original
                                 exporting driver.
  @param  ActionRequest          On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.

**/
EFI_STATUS
EFIAPI
DriverCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  DRIVER_SAMPLE_PRIVATE_DATA      *PrivateData;
  EFI_STATUS                      Status;
  VOID                            *StartOpCodeHandle;
  VOID                            *OptionsOpCodeHandle;
  EFI_IFR_GUID_LABEL              *StartLabel;
  VOID                            *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL              *EndLabel;
  EFI_INPUT_KEY                   Key;
  DRIVER_SAMPLE_CONFIGURATION     *Configuration;
  MY_EFI_VARSTORE_DATA            *EfiData;
  EFI_FORM_ID                     FormId;
  EFI_STRING                      Progress;
  EFI_STRING                      Results;
  UINT32                          ProgressErr;
  CHAR16                          *TmpStr;
  UINTN                           Index;
  UINT64                          BufferValue;
  EFI_HII_POPUP_SELECTION         UserSelection;

  UserSelection = 0xFF;

  if (((Value == NULL) && (Action != EFI_BROWSER_ACTION_FORM_OPEN) && (Action != EFI_BROWSER_ACTION_FORM_CLOSE))||
    (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }


  FormId = 0;
  ProgressErr = 0;
  Status = EFI_SUCCESS;
  BufferValue = 3;
  PrivateData = DRIVER_SAMPLE_PRIVATE_FROM_THIS (This);

  switch (Action) {
  case EFI_BROWSER_ACTION_FORM_OPEN:
    {
      if (QuestionId == 0x1234) {
        //
        // Sample CallBack for UEFI FORM_OPEN action:
        //   Add Save action into Form 3 when Form 1 is opened.
        //   This will be done only in FORM_OPEN CallBack of question with ID 0x1234 from Form 1.
        //
        PrivateData = DRIVER_SAMPLE_PRIVATE_FROM_THIS (This);

        //
        // Initialize the container for dynamic opcodes
        //
        StartOpCodeHandle = HiiAllocateOpCodeHandle ();
        ASSERT (StartOpCodeHandle != NULL);

        //
        // Create Hii Extend Label OpCode as the start opcode
        //
        StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
        StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
        StartLabel->Number       = LABEL_UPDATE2;

        HiiCreateActionOpCode (
          StartOpCodeHandle,                // Container for dynamic created opcodes
          0x1238,                           // Question ID
          STRING_TOKEN(STR_SAVE_TEXT),      // Prompt text
          STRING_TOKEN(STR_SAVE_TEXT),      // Help text
          EFI_IFR_FLAG_CALLBACK,            // Question flag
          0                                 // Action String ID
        );

        HiiUpdateForm (
          PrivateData->HiiHandle[0],  // HII handle
          &gDriverSampleFormSetGuid,  // Formset GUID
          0x3,                        // Form ID
          StartOpCodeHandle,          // Label for where to insert opcodes
          NULL                        // Insert data
          );

        HiiFreeOpCodeHandle (StartOpCodeHandle);
      }

      if (QuestionId == 0x1247) {
        Status = InternalStartMonitor ();
        ASSERT_EFI_ERROR (Status);
      }
    }
    break;

  case EFI_BROWSER_ACTION_FORM_CLOSE:
    {
      if (QuestionId == 0x5678) {
        //
        // Sample CallBack for UEFI FORM_CLOSE action:
        //   Show up a pop-up to specify Form 3 will be closed when exit Form 3.
        //
        do {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"",
            L"You are going to leave third Form!",
            L"Press ESC or ENTER to continue ...",
            L"",
            NULL
            );
        } while ((Key.ScanCode != SCAN_ESC) && (Key.UnicodeChar != CHAR_CARRIAGE_RETURN));
      }

      if (QuestionId == 0x1247) {
        Status = InternalStopMonitor ();
        ASSERT_EFI_ERROR (Status);
      }
    }
    break;

  case EFI_BROWSER_ACTION_RETRIEVE:
    {
      switch (QuestionId ) {
      case 0x1248:
        if (Type != EFI_IFR_TYPE_REF) {
          return EFI_INVALID_PARAMETER;
        }
        Value->ref.FormId = 0x3;
        break;

      case 0x5678:
      case 0x1247:
        //
        // We will reach here once the Question is refreshed
        //

        //
        // Initialize the container for dynamic opcodes
        //
        StartOpCodeHandle = HiiAllocateOpCodeHandle ();
        ASSERT (StartOpCodeHandle != NULL);

        //
        // Create Hii Extend Label OpCode as the start opcode
        //
        StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
        StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
        if (QuestionId == 0x5678) {
          StartLabel->Number       = LABEL_UPDATE2;
          FormId                   = 0x03;
          PrivateData->Configuration.DynamicRefresh++;
        } else if (QuestionId == 0x1247 ) {
          StartLabel->Number       = LABEL_UPDATE3;
          FormId                   = 0x06;
          PrivateData->Configuration.RefreshGuidCount++;
        }

        HiiCreateActionOpCode (
          StartOpCodeHandle,                // Container for dynamic created opcodes
          0x1237,                           // Question ID
          STRING_TOKEN(STR_EXIT_TEXT),      // Prompt text
          STRING_TOKEN(STR_EXIT_TEXT),      // Help text
          EFI_IFR_FLAG_CALLBACK,            // Question flag
          0                                 // Action String ID
        );

        HiiUpdateForm (
          PrivateData->HiiHandle[0],        // HII handle
          &gDriverSampleFormSetGuid,        // Formset GUID
          FormId,                           // Form ID
          StartOpCodeHandle,                // Label for where to insert opcodes
          NULL                              // Insert data
        );

        HiiFreeOpCodeHandle (StartOpCodeHandle);

        //
        // Refresh the Question value
        //
        Status = gRT->SetVariable(
                        VariableName,
                        &gDriverSampleFormSetGuid,
                        EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                        sizeof (DRIVER_SAMPLE_CONFIGURATION),
                        &PrivateData->Configuration
                        );

        if (QuestionId == 0x5678) {
          //
          // Update uncommitted data of Browser
          //
          EfiData = AllocateZeroPool (sizeof (MY_EFI_VARSTORE_DATA));
          ASSERT (EfiData != NULL);
          if (HiiGetBrowserData (&gDriverSampleFormSetGuid, MyEfiVar, sizeof (MY_EFI_VARSTORE_DATA), (UINT8 *) EfiData)) {
            EfiData->Field8 = 111;
            HiiSetBrowserData (
              &gDriverSampleFormSetGuid,
              MyEfiVar,
              sizeof (MY_EFI_VARSTORE_DATA),
              (UINT8 *) EfiData,
              NULL
            );
          }
          FreePool (EfiData);
        }
        break;
      }
    }
    break;

  case EFI_BROWSER_ACTION_DEFAULT_STANDARD:
    {
      switch (QuestionId) {
      case 0x1240:
        Value->u8 = DEFAULT_CLASS_STANDARD_VALUE;
      break;

      case 0x1252:
        for (Index = 0; Index < 3; Index ++) {
          SetArrayData (Value, EFI_IFR_TYPE_NUM_SIZE_8, Index, BufferValue--);
        }
      break;

      case 0x6666:
        Value->u8 = 12;
        break;

      default:
        Status = EFI_UNSUPPORTED;
      break;
      }
    }
    break;

  case EFI_BROWSER_ACTION_DEFAULT_MANUFACTURING:
    {
      switch (QuestionId) {
      case 0x1240:
        Value->u8 = DEFAULT_CLASS_MANUFACTURING_VALUE;
      break;

     case 0x6666:
        Value->u8 = 13;
        break;

      default:
        Status = EFI_UNSUPPORTED;
      break;
      }
    }
    break;

  case EFI_BROWSER_ACTION_CHANGING:
  {
    switch (QuestionId) {
    case 0x1249:
      {
        if (Type != EFI_IFR_TYPE_REF) {
          return EFI_INVALID_PARAMETER;
        }

        Value->ref.FormId = 0x1234;
      }
    break;
    case 0x1234:
      //
      // Initialize the container for dynamic opcodes
      //
      StartOpCodeHandle = HiiAllocateOpCodeHandle ();
      ASSERT (StartOpCodeHandle != NULL);

      EndOpCodeHandle = HiiAllocateOpCodeHandle ();
      ASSERT (EndOpCodeHandle != NULL);

      //
      // Create Hii Extend Label OpCode as the start opcode
      //
      StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
      StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
      StartLabel->Number       = LABEL_UPDATE1;

      //
      // Create Hii Extend Label OpCode as the end opcode
      //
      EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
      EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
      EndLabel->Number       = LABEL_END;

      HiiCreateActionOpCode (
        StartOpCodeHandle,                // Container for dynamic created opcodes
        0x1237,                           // Question ID
        STRING_TOKEN(STR_EXIT_TEXT),      // Prompt text
        STRING_TOKEN(STR_EXIT_TEXT),      // Help text
        EFI_IFR_FLAG_CALLBACK,            // Question flag
        0                                 // Action String ID
      );

      //
      // Create Option OpCode
      //
      OptionsOpCodeHandle = HiiAllocateOpCodeHandle ();
      ASSERT (OptionsOpCodeHandle != NULL);

      HiiCreateOneOfOptionOpCode (
        OptionsOpCodeHandle,
        STRING_TOKEN (STR_BOOT_OPTION1),
        0,
        EFI_IFR_NUMERIC_SIZE_1,
        1
        );

      HiiCreateOneOfOptionOpCode (
        OptionsOpCodeHandle,
        STRING_TOKEN (STR_BOOT_OPTION2),
        0,
        EFI_IFR_NUMERIC_SIZE_1,
        2
        );

      //
      // Prepare initial value for the dynamic created oneof Question
      //
      PrivateData->Configuration.DynamicOneof = 2;
      Status = gRT->SetVariable(
                      VariableName,
                      &gDriverSampleFormSetGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                      sizeof (DRIVER_SAMPLE_CONFIGURATION),
                      &PrivateData->Configuration
                      );

      //
      // Set initial vlaue of dynamic created oneof Question in Form Browser
      //
      Configuration = AllocateZeroPool (sizeof (DRIVER_SAMPLE_CONFIGURATION));
      ASSERT (Configuration != NULL);
      if (HiiGetBrowserData (&gDriverSampleFormSetGuid, VariableName, sizeof (DRIVER_SAMPLE_CONFIGURATION), (UINT8 *) Configuration)) {
        Configuration->DynamicOneof = 2;

        //
        // Update uncommitted data of Browser
        //
        HiiSetBrowserData (
          &gDriverSampleFormSetGuid,
          VariableName,
          sizeof (DRIVER_SAMPLE_CONFIGURATION),
          (UINT8 *) Configuration,
          NULL
          );
      }
      FreePool (Configuration);

      HiiCreateOneOfOpCode (
        StartOpCodeHandle,                         // Container for dynamic created opcodes
        0x8001,                                    // Question ID (or call it "key")
        CONFIGURATION_VARSTORE_ID,                 // VarStore ID
        (UINT16) DYNAMIC_ONE_OF_VAR_OFFSET,        // Offset in Buffer Storage
        STRING_TOKEN (STR_ONE_OF_PROMPT),          // Question prompt text
        STRING_TOKEN (STR_ONE_OF_HELP),            // Question help text
        EFI_IFR_FLAG_CALLBACK,                     // Question flag
        EFI_IFR_NUMERIC_SIZE_1,                    // Data type of Question Value
        OptionsOpCodeHandle,                       // Option Opcode list
        NULL                                       // Default Opcode is NULl
        );

      HiiCreateOrderedListOpCode (
        StartOpCodeHandle,                         // Container for dynamic created opcodes
        0x8002,                                    // Question ID
        CONFIGURATION_VARSTORE_ID,                 // VarStore ID
        (UINT16) DYNAMIC_ORDERED_LIST_VAR_OFFSET,  // Offset in Buffer Storage
        STRING_TOKEN (STR_BOOT_OPTIONS),           // Question prompt text
        STRING_TOKEN (STR_BOOT_OPTIONS),           // Question help text
        EFI_IFR_FLAG_RESET_REQUIRED,               // Question flag
        0,                                         // Ordered list flag, e.g. EFI_IFR_UNIQUE_SET
        EFI_IFR_NUMERIC_SIZE_1,                    // Data type of Question value
        5,                                         // Maximum container
        OptionsOpCodeHandle,                       // Option Opcode list
        NULL                                       // Default Opcode is NULl
        );

      HiiCreateTextOpCode (
        StartOpCodeHandle,
        STRING_TOKEN(STR_TEXT_SAMPLE_HELP),
        STRING_TOKEN(STR_TEXT_SAMPLE_HELP),
        STRING_TOKEN(STR_TEXT_SAMPLE_STRING)
      );

      HiiCreateDateOpCode (
        StartOpCodeHandle,
        0x8004,
        0x0,
        0x0,
        STRING_TOKEN(STR_DATE_SAMPLE_HELP),
        STRING_TOKEN(STR_DATE_SAMPLE_HELP),
        0,
        QF_DATE_STORAGE_TIME,
        NULL
        );

      HiiCreateTimeOpCode (
        StartOpCodeHandle,
        0x8005,
        0x0,
        0x0,
        STRING_TOKEN(STR_TIME_SAMPLE_HELP),
        STRING_TOKEN(STR_TIME_SAMPLE_HELP),
        0,
        QF_TIME_STORAGE_TIME,
        NULL
        );

      HiiCreateGotoOpCode (
        StartOpCodeHandle,                // Container for dynamic created opcodes
        1,                                // Target Form ID
        STRING_TOKEN (STR_GOTO_FORM1),    // Prompt text
        STRING_TOKEN (STR_GOTO_HELP),     // Help text
        0,                                // Question flag
        0x8003                            // Question ID
        );

      HiiUpdateForm (
        PrivateData->HiiHandle[0],  // HII handle
        &gDriverSampleFormSetGuid,  // Formset GUID
        0x1234,                     // Form ID
        StartOpCodeHandle,          // Label for where to insert opcodes
        EndOpCodeHandle             // Replace data
        );

      HiiFreeOpCodeHandle (StartOpCodeHandle);
      HiiFreeOpCodeHandle (OptionsOpCodeHandle);
      HiiFreeOpCodeHandle (EndOpCodeHandle);
      break;

    default:
      break;
    }
  }
  break;

  case EFI_BROWSER_ACTION_CHANGED:
    switch (QuestionId) {
      case 0x1237:
        //
        // User press "Exit now", request Browser to exit
        //
        *ActionRequest = EFI_BROWSER_ACTION_REQUEST_EXIT;
        break;

      case 0x1238:
        //
        // User press "Save now", request Browser to save the uncommitted data.
        //
        *ActionRequest = EFI_BROWSER_ACTION_REQUEST_SUBMIT;
        break;

      case 0x1241:
      case 0x1246:
        //
        // User press "Submit current form and Exit now", request Browser to submit current form and exit
        //
        *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_SUBMIT_EXIT;
        break;

      case 0x1242:
        //
        // User press "Discard current form now", request Browser to discard the uncommitted data.
        //
        *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_DISCARD;
        break;

      case 0x1243:
        //
        // User press "Submit current form now", request Browser to save the uncommitted data.
        //
        *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_APPLY;
        break;

      case 0x1244:
      case 0x1245:
        //
        // User press "Discard current form and Exit now", request Browser to discard the uncommitted data and exit.
        //
        *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_DISCARD_EXIT;
        break;

      case 0x1231:
        //
        // 1. Check to see whether system support keyword.
        //
        Status = PrivateData->HiiKeywordHandler->GetData (PrivateData->HiiKeywordHandler,
                                                          L"NAMESPACE=x-UEFI-ns",
                                                          L"KEYWORD=iSCSIBootEnable",
                                                          &Progress,
                                                          &ProgressErr,
                                                          &Results
                                                         );
        if (EFI_ERROR (Status)) {
          do {
            CreatePopUp (
              EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
              &Key,
              L"",
              L"This system not support this keyword!",
              L"Press ENTER to continue ...",
              L"",
              NULL
              );
          } while (Key.UnicodeChar != CHAR_CARRIAGE_RETURN);

          Status = EFI_SUCCESS;
          break;
        }

        //
        // 2. If system support this keyword, just try to change value.
        //

        //
        // Change value from '0' to '1' or from '1' to '0'
        //
        TmpStr = StrStr (Results, L"&VALUE=");
        ASSERT (TmpStr != NULL);
        TmpStr += StrLen (L"&VALUE=");
        TmpStr++;
        if (*TmpStr == L'0') {
          *TmpStr = L'1';
        } else {
          *TmpStr = L'0';
        }

        //
        // 3. Call the keyword handler protocol to change the value.
        //
        Status = PrivateData->HiiKeywordHandler->SetData (PrivateData->HiiKeywordHandler,
                                                          Results,
                                                          &Progress,
                                                          &ProgressErr
                                                         );
        if (EFI_ERROR (Status)) {
          do {
            CreatePopUp (
              EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
              &Key,
              L"",
              L"Set keyword to the system failed!",
              L"Press ENTER to continue ...",
              L"",
              NULL
              );
          } while (Key.UnicodeChar != CHAR_CARRIAGE_RETURN);

          Status = EFI_SUCCESS;
          break;
        }
        break;

      case 0x1330:
        Status = mPrivateData->HiiPopup->CreatePopup (
          mPrivateData->HiiPopup,
          EfiHiiPopupStyleInfo,
          EfiHiiPopupTypeYesNo,
          mPrivateData->HiiHandle[0],
          STRING_TOKEN (STR_POPUP_STRING),
          &UserSelection
          );
        if (!EFI_ERROR (Status)) {
          if (UserSelection == EfiHiiPopupSelectionYes) {
            *ActionRequest = EFI_BROWSER_ACTION_REQUEST_EXIT;
          }
        }
        break;

      default:
      break;
    }
  break;

  case EFI_BROWSER_ACTION_SUBMITTED:
    {
      if (QuestionId == 0x1250) {
        //
        // Sample CallBack for EFI_BROWSER_ACTION_SUBMITTED action:
        // Show up a pop-up to show SUBMITTED callback has been triggered.
        //
        do {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"",
            L"EfiVarstore value has been submitted!",
            L"Press ESC or ENTER to continue ...",
            L"",
            NULL
            );
        } while ((Key.ScanCode != SCAN_ESC) && (Key.UnicodeChar != CHAR_CARRIAGE_RETURN));
      }
    }
    break;

  default:
    Status = EFI_UNSUPPORTED;
    break;
  }

  return Status;
}

/**
  Main entry for this driver.

  @param ImageHandle     Image handle this driver.
  @param SystemTable     Pointer to SystemTable.

  @retval EFI_SUCESS     This function always complete successfully.

**/
EFI_STATUS
EFIAPI
DriverSampleInit (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_HII_HANDLE                  HiiHandle[2];
  EFI_SCREEN_DESCRIPTOR           Screen;
  EFI_HII_DATABASE_PROTOCOL       *HiiDatabase;
  EFI_HII_STRING_PROTOCOL         *HiiString;
  EFI_FORM_BROWSER2_PROTOCOL      *FormBrowser2;
  EFI_HII_CONFIG_ROUTING_PROTOCOL *HiiConfigRouting;
  EFI_CONFIG_KEYWORD_HANDLER_PROTOCOL *HiiKeywordHandler;
  EFI_HII_POPUP_PROTOCOL              *PopupHandler;
  CHAR16                          *NewString;
  UINTN                           BufferSize;
  DRIVER_SAMPLE_CONFIGURATION     *Configuration;
  BOOLEAN                         ActionFlag;
  EFI_STRING                      ConfigRequestHdr;
  EFI_STRING                      NameRequestHdr;
  MY_EFI_VARSTORE_DATA            *VarStoreConfig;
  MY_EFI_BITS_VARSTORE_DATA       *BitsVarStoreConfig;
  MY_EFI_UNION_DATA               *UnionConfig;
  EFI_INPUT_KEY                   HotKey;
  EDKII_FORM_BROWSER_EXTENSION_PROTOCOL *FormBrowserEx;

  //
  // Initialize the local variables.
  //
  ConfigRequestHdr = NULL;
  NewString        = NULL;

  //
  // Initialize screen dimensions for SendForm().
  // Remove 3 characters from top and bottom
  //
  ZeroMem (&Screen, sizeof (EFI_SCREEN_DESCRIPTOR));
  gST->ConOut->QueryMode (gST->ConOut, gST->ConOut->Mode->Mode, &Screen.RightColumn, &Screen.BottomRow);

  Screen.TopRow     = 3;
  Screen.BottomRow  = Screen.BottomRow - 3;

  //
  // Initialize driver private data
  //
  mPrivateData = AllocateZeroPool (sizeof (DRIVER_SAMPLE_PRIVATE_DATA));
  if (mPrivateData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  mPrivateData->Signature = DRIVER_SAMPLE_PRIVATE_SIGNATURE;

  mPrivateData->ConfigAccess.ExtractConfig = ExtractConfig;
  mPrivateData->ConfigAccess.RouteConfig = RouteConfig;
  mPrivateData->ConfigAccess.Callback = DriverCallback;

  //
  // Locate Hii Database protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID **) &HiiDatabase);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mPrivateData->HiiDatabase = HiiDatabase;

  //
  // Locate HiiString protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiStringProtocolGuid, NULL, (VOID **) &HiiString);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mPrivateData->HiiString = HiiString;

  //
  // Locate Formbrowser2 protocol
  //
  Status = gBS->LocateProtocol (&gEfiFormBrowser2ProtocolGuid, NULL, (VOID **) &FormBrowser2);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mPrivateData->FormBrowser2 = FormBrowser2;

  //
  // Locate ConfigRouting protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID **) &HiiConfigRouting);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mPrivateData->HiiConfigRouting = HiiConfigRouting;

  //
  // Locate keyword handler protocol
  //
  Status = gBS->LocateProtocol (&gEfiConfigKeywordHandlerProtocolGuid, NULL, (VOID **) &HiiKeywordHandler);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mPrivateData->HiiKeywordHandler = HiiKeywordHandler;

  //
  // Locate HiiPopup protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiPopupProtocolGuid, NULL, (VOID **) &PopupHandler);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mPrivateData->HiiPopup = PopupHandler;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &DriverHandle[0],
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath0,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mPrivateData->ConfigAccess,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  mPrivateData->DriverHandle[0] = DriverHandle[0];

  //
  // Publish our HII data
  //
  HiiHandle[0] = HiiAddPackages (
                   &gDriverSampleFormSetGuid,
                   DriverHandle[0],
                   DriverSampleStrings,
                   VfrBin,
                   NULL
                   );
  if (HiiHandle[0] == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  mPrivateData->HiiHandle[0] = HiiHandle[0];

  //
  // Publish another Fromset
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &DriverHandle[1],
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath1,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mPrivateData->ConfigAccess,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  mPrivateData->DriverHandle[1] = DriverHandle[1];

  HiiHandle[1] = HiiAddPackages (
                   &gDriverSampleInventoryGuid,
                   DriverHandle[1],
                   DriverSampleStrings,
                   InventoryBin,
                   NULL
                   );
  if (HiiHandle[1] == NULL) {
    DriverSampleUnload (ImageHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  mPrivateData->HiiHandle[1] = HiiHandle[1];

  //
  // Update the device path string.
  //
  NewString = ConvertDevicePathToText((EFI_DEVICE_PATH_PROTOCOL*)&mHiiVendorDevicePath0, FALSE, FALSE);
  if (HiiSetString (HiiHandle[0], STRING_TOKEN (STR_DEVICE_PATH), NewString, NULL) == 0) {
    DriverSampleUnload (ImageHandle);
    return EFI_OUT_OF_RESOURCES;
  }
  if (NewString != NULL) {
    FreePool (NewString);
  }

  //
  // Very simple example of how one would update a string that is already
  // in the HII database
  //
  NewString = L"700 Mhz";

  if (HiiSetString (HiiHandle[0], STRING_TOKEN (STR_CPU_STRING2), NewString, NULL) == 0) {
    DriverSampleUnload (ImageHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  HiiSetString (HiiHandle[0], 0, NewString, NULL);

  //
  // Initialize Name/Value name String ID
  //
  mPrivateData->NameStringId[0] = STR_NAME_VALUE_VAR_NAME0;
  mPrivateData->NameStringId[1] = STR_NAME_VALUE_VAR_NAME1;
  mPrivateData->NameStringId[2] = STR_NAME_VALUE_VAR_NAME2;

  //
  // Initialize configuration data
  //
  Configuration = &mPrivateData->Configuration;
  ZeroMem (Configuration, sizeof (DRIVER_SAMPLE_CONFIGURATION));

  //
  // Try to read NV config EFI variable first
  //
  ConfigRequestHdr = HiiConstructConfigHdr (&gDriverSampleFormSetGuid, VariableName, DriverHandle[0]);
  ASSERT (ConfigRequestHdr != NULL);

  NameRequestHdr = HiiConstructConfigHdr (&gDriverSampleFormSetGuid, NULL, DriverHandle[0]);
  ASSERT (NameRequestHdr != NULL);

  BufferSize = sizeof (DRIVER_SAMPLE_CONFIGURATION);
  Status = gRT->GetVariable (VariableName, &gDriverSampleFormSetGuid, NULL, &BufferSize, Configuration);
  if (EFI_ERROR (Status)) {
    //
    // Store zero data Buffer Storage to EFI variable
    //
    Status = gRT->SetVariable(
                    VariableName,
                    &gDriverSampleFormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (DRIVER_SAMPLE_CONFIGURATION),
                    Configuration
                    );
    if (EFI_ERROR (Status)) {
      DriverSampleUnload (ImageHandle);
      return Status;
    }
    //
    // EFI variable for NV config doesn't exit, we should build this variable
    // based on default values stored in IFR
    //
    ActionFlag = HiiSetToDefaults (NameRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    if (!ActionFlag) {
      DriverSampleUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }

    ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    if (!ActionFlag) {
      DriverSampleUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }
  } else {
    //
    // EFI variable does exist and Validate Current Setting
    //
    ActionFlag = HiiValidateSettings (NameRequestHdr);
    if (!ActionFlag) {
      DriverSampleUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }

    ActionFlag = HiiValidateSettings (ConfigRequestHdr);
    if (!ActionFlag) {
      DriverSampleUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }
  }
  FreePool (ConfigRequestHdr);

  //
  // Initialize efi varstore configuration data
  //
  VarStoreConfig = &mPrivateData->VarStoreConfig;
  ZeroMem (VarStoreConfig, sizeof (MY_EFI_VARSTORE_DATA));

  ConfigRequestHdr = HiiConstructConfigHdr (&gDriverSampleFormSetGuid, MyEfiVar, DriverHandle[0]);
  ASSERT (ConfigRequestHdr != NULL);

  BufferSize = sizeof (MY_EFI_VARSTORE_DATA);
  Status = gRT->GetVariable (MyEfiVar, &gDriverSampleFormSetGuid, NULL, &BufferSize, VarStoreConfig);
  if (EFI_ERROR (Status)) {
    //
    // Store zero data to EFI variable Storage.
    //
    Status = gRT->SetVariable(
                    MyEfiVar,
                    &gDriverSampleFormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (MY_EFI_VARSTORE_DATA),
                    VarStoreConfig
                    );
    if (EFI_ERROR (Status)) {
      DriverSampleUnload (ImageHandle);
      return Status;
    }
    //
    // EFI variable for NV config doesn't exit, we should build this variable
    // based on default values stored in IFR
    //
    ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    if (!ActionFlag) {
      DriverSampleUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }
  } else {
    //
    // EFI variable does exist and Validate Current Setting
    //
    ActionFlag = HiiValidateSettings (ConfigRequestHdr);
    if (!ActionFlag) {
      DriverSampleUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }
  }
  FreePool (ConfigRequestHdr);

  //
  // Initialize Bits efi varstore configuration data
  //
  BitsVarStoreConfig = &mPrivateData->BitsVarStoreConfig;
  ZeroMem (BitsVarStoreConfig, sizeof (MY_EFI_BITS_VARSTORE_DATA));

  ConfigRequestHdr = HiiConstructConfigHdr (&gDriverSampleFormSetGuid, MyEfiBitVar, DriverHandle[0]);
  ASSERT (ConfigRequestHdr != NULL);

  BufferSize = sizeof (MY_EFI_BITS_VARSTORE_DATA);
  Status = gRT->GetVariable (MyEfiBitVar, &gDriverSampleFormSetGuid, NULL, &BufferSize, BitsVarStoreConfig);
  if (EFI_ERROR (Status)) {
    //
    // Store zero data to EFI variable Storage.
    //
    Status = gRT->SetVariable(
                    MyEfiBitVar,
                    &gDriverSampleFormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (MY_EFI_BITS_VARSTORE_DATA),
                    BitsVarStoreConfig
                    );
    if (EFI_ERROR (Status)) {
      DriverSampleUnload (ImageHandle);
      return Status;
    }
    //
    // EFI variable for NV config doesn't exit, we should build this variable
    // based on default values stored in IFR
    //
    ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    if (!ActionFlag) {
      DriverSampleUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }
  } else {
    //
    // EFI variable does exist and Validate Current Setting
    //
    ActionFlag = HiiValidateSettings (ConfigRequestHdr);
    if (!ActionFlag) {
      DriverSampleUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }
  }
  FreePool (ConfigRequestHdr);

   //
  // Initialize Union efi varstore configuration data
  //
  UnionConfig = &mPrivateData->UnionConfig;
  ZeroMem (UnionConfig, sizeof (MY_EFI_UNION_DATA));

  ConfigRequestHdr = HiiConstructConfigHdr (&gDriverSampleFormSetGuid, MyEfiUnionVar, DriverHandle[0]);
  ASSERT (ConfigRequestHdr != NULL);

  BufferSize = sizeof (MY_EFI_UNION_DATA);
  Status = gRT->GetVariable (MyEfiUnionVar, &gDriverSampleFormSetGuid, NULL, &BufferSize, UnionConfig);
  if (EFI_ERROR (Status)) {
    //
    // Store zero data to EFI variable Storage.
    //
    Status = gRT->SetVariable(
                    MyEfiUnionVar,
                    &gDriverSampleFormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (MY_EFI_UNION_DATA),
                    UnionConfig
                    );
    if (EFI_ERROR (Status)) {
      DriverSampleUnload (ImageHandle);
      return Status;
    }
    //
    // EFI variable for NV config doesn't exit, we should build this variable
    // based on default values stored in IFR
    //
    ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    if (!ActionFlag) {
      DriverSampleUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }
  } else {
    //
    // EFI variable does exist and Validate Current Setting
    //
    ActionFlag = HiiValidateSettings (ConfigRequestHdr);
    if (!ActionFlag) {
      DriverSampleUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }
  }
  FreePool (ConfigRequestHdr);

  Status = gBS->CreateEventEx (
        EVT_NOTIFY_SIGNAL,
        TPL_NOTIFY,
        EfiEventEmptyFunction,
        NULL,
        &gEfiIfrRefreshIdOpGuid,
        &mEvent
        );
  ASSERT_EFI_ERROR (Status);

  //
  // Example of how to use BrowserEx protocol to register HotKey.
  //
  Status = gBS->LocateProtocol (&gEdkiiFormBrowserExProtocolGuid, NULL, (VOID **) &FormBrowserEx);
  if (!EFI_ERROR (Status)) {
    //
    // First unregister the default hot key F9 and F10.
    //
    HotKey.UnicodeChar = CHAR_NULL;
    HotKey.ScanCode    = SCAN_F9;
    FormBrowserEx->RegisterHotKey (&HotKey, 0, 0, NULL);
    HotKey.ScanCode    = SCAN_F10;
    FormBrowserEx->RegisterHotKey (&HotKey, 0, 0, NULL);

    //
    // Register the default HotKey F9 and F10 again.
    //
    HotKey.ScanCode   = SCAN_F10;
    NewString         = HiiGetString (mPrivateData->HiiHandle[0], STRING_TOKEN (FUNCTION_TEN_STRING), NULL);
    ASSERT (NewString != NULL);
    FormBrowserEx->RegisterHotKey (&HotKey, BROWSER_ACTION_SUBMIT, 0, NewString);
    HotKey.ScanCode   = SCAN_F9;
    NewString         = HiiGetString (mPrivateData->HiiHandle[0], STRING_TOKEN (FUNCTION_NINE_STRING), NULL);
    ASSERT (NewString != NULL);
    FormBrowserEx->RegisterHotKey (&HotKey, BROWSER_ACTION_DEFAULT, EFI_HII_DEFAULT_CLASS_STANDARD, NewString);
  }

  //
  // In default, this driver is built into Flash device image,
  // the following code doesn't run.
  //

  //
  // Example of how to display only the item we sent to HII
  // When this driver is not built into Flash device image,
  // it need to call SendForm to show front page by itself.
  //
  if (DISPLAY_ONLY_MY_ITEM <= 1) {
    //
    // Have the browser pull out our copy of the data, and only display our data
    //
    Status = FormBrowser2->SendForm (
                             FormBrowser2,
                             &(HiiHandle[DISPLAY_ONLY_MY_ITEM]),
                             1,
                             NULL,
                             0,
                             NULL,
                             NULL
                             );

    HiiRemovePackages (HiiHandle[0]);

    HiiRemovePackages (HiiHandle[1]);
  }

  return EFI_SUCCESS;
}

/**
  Unloads the application and its installed protocol.

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
**/
EFI_STATUS
EFIAPI
DriverSampleUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  UINTN Index;

  ASSERT (mPrivateData != NULL);

  if (DriverHandle[0] != NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
            DriverHandle[0],
            &gEfiDevicePathProtocolGuid,
            &mHiiVendorDevicePath0,
            &gEfiHiiConfigAccessProtocolGuid,
            &mPrivateData->ConfigAccess,
            NULL
           );
    DriverHandle[0] = NULL;
  }

  if (DriverHandle[1] != NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
            DriverHandle[1],
            &gEfiDevicePathProtocolGuid,
            &mHiiVendorDevicePath1,
            &gEfiHiiConfigAccessProtocolGuid,
            &mPrivateData->ConfigAccess,
            NULL
           );
    DriverHandle[1] = NULL;
  }

  if (mPrivateData->HiiHandle[0] != NULL) {
    HiiRemovePackages (mPrivateData->HiiHandle[0]);
  }

  if (mPrivateData->HiiHandle[1] != NULL) {
    HiiRemovePackages (mPrivateData->HiiHandle[1]);
  }

  for (Index = 0; Index < NAME_VALUE_NAME_NUMBER; Index++) {
    if (mPrivateData->NameValueName[Index] != NULL) {
      FreePool (mPrivateData->NameValueName[Index]);
    }
  }
  FreePool (mPrivateData);
  mPrivateData = NULL;

  gBS->CloseEvent (mEvent);

  return EFI_SUCCESS;
}



 
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	(void)ImageHandle;
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;

	gBS = SystemTable->BootServices;
	gRT = SystemTable->RuntimeServices;

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
