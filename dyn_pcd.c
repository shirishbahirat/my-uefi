#include "efi.h"
#include "log.h"

EFI_GUID gEfiUnicodeCollation2ProtocolGuid = {0xa4c751fc, 0x23ae, 0x4c3e, { 0x92, 0xe9, 0x49, 0x64, 0xcf, 0x63, 0xf3, 0x49 }};
EFI_GUID gEfiPcdProtocolGuid = { 0x13a3f0f6, 0x264a, 0x3ef0, { 0xf2, 0xe0, 0xde, 0xc5, 0x12, 0x34, 0x2f, 0x34 } };
EFI_GUID gEfiGetPcdInfoProtocolGuid      = { 0xfd0f4478,  0xefd, 0x461d, { 0xba, 0x2d, 0xe5, 0x8c, 0x45, 0xfd, 0x5f, 0x5e } };


typedef struct _EFI_UNICODE_COLLATION_PROTOCOL  EFI_UNICODE_COLLATION_PROTOCOL;
typedef struct _EFI_GET_PCD_INFO_PROTOCOL  EFI_GET_PCD_INFO_PROTOCOL;

typedef enum {
  EFI_PCD_TYPE_8,
  EFI_PCD_TYPE_16,
  EFI_PCD_TYPE_32,
  EFI_PCD_TYPE_64,
  EFI_PCD_TYPE_BOOL,
  EFI_PCD_TYPE_PTR
} EFI_PCD_TYPE;

typedef struct {
  EFI_PCD_TYPE      PcdType;
  UINTN             PcdSize;
  CHAR8             *PcdName;
} EFI_PCD_INFO;

typedef
EFI_STATUS
(EFIAPI *EFI_GET_PCD_INFO_PROTOCOL_GET_INFO) (
  IN CONST  EFI_GUID        *Guid,
  IN        UINTN           TokenNumber,
  OUT       EFI_PCD_INFO    *PcdInfo
);

typedef
UINTN
(EFIAPI *EFI_GET_PCD_INFO_PROTOCOL_GET_SKU) (
  VOID
);
struct _EFI_GET_PCD_INFO_PROTOCOL {
  EFI_GET_PCD_INFO_PROTOCOL_GET_INFO    GetInfo;
  EFI_GET_PCD_INFO_PROTOCOL_GET_SKU     GetSku;
};

EFI_GET_PCD_INFO_PROTOCOL   *mPiPcdInfo = NULL;

typedef VOID (EFIAPI *EFI_PCD_PROTOCOL_SET_SKU)(IN UINTN SkuId);

typedef UINT8 (EFIAPI *EFI_PCD_PROTOCOL_GET_8)(IN CONST EFI_GUID *Guid, IN UINTN TokenNumber);

typedef UINT16 (EFIAPI *EFI_PCD_PROTOCOL_GET_16)(IN CONST EFI_GUID *Guid, IN UINTN TokenNumber);

typedef UINT32 (EFIAPI *EFI_PCD_PROTOCOL_GET_32)(IN CONST EFI_GUID *Guid, IN UINTN TokenNumber);

typedef UINT64 (EFIAPI *EFI_PCD_PROTOCOL_GET_64)(IN CONST EFI_GUID *Guid, IN UINTN TokenNumber);

typedef VOID * (EFIAPI *EFI_PCD_PROTOCOL_GET_POINTER)(IN CONST EFI_GUID Guid, IN UINTN TokenNumber);

typedef BOOLEAN (EFIAPI *EFI_PCD_PROTOCOL_GET_BOOLEAN)(IN CONST EFI_GUID *Guid, IN UINTN TokenNumber);

typedef UINTN (EFIAPI *EFI_PCD_PROTOCOL_GET_SIZE)(IN CONST EFI_GUID Guid, IN UINTN TokenNumber);
 
typedef EFI_STATUS (EFIAPI *EFI_PCD_PROTOCOL_SET_8)(IN CONST EFI_GUID Guid, IN UINTN TokenNumber, IN UINT8 Value);

typedef EFI_STATUS (EFIAPI *EFI_PCD_PROTOCOL_SET_16)(IN CONST  EFI_GUID *Guid, IN UINTN TokenNumber, IN UINT16 Value);
 
typedef EFI_STATUS (EFIAPI *EFI_PCD_PROTOCOL_SET_32)(IN CONST  EFI_GUID *Guid, IN UINTN TokenNumber, IN UINT32 Value);

typedef EFI_STATUS (EFIAPI *EFI_PCD_PROTOCOL_SET_64)(IN CONST  EFI_GUID *Guid, IN UINTN TokenNumber, IN UINT64 Value);

typedef EFI_STATUS (EFIAPI *EFI_PCD_PROTOCOL_SET_POINTER)(IN CONST  EFI_GUID *Guid, IN UINTN TokenNumber, IN OUT UINTN *SizeOfValue, IN VOID *Buffer);

typedef EFI_STATUS (EFIAPI *EFI_PCD_PROTOCOL_SET_BOOLEAN)(IN CONST  EFI_GUID *Guid, IN UINTN TokenNumber, IN BOOLEAN Value);

typedef VOID (EFIAPI *EFI_PCD_PROTOCOL_CALLBACK)(IN EFI_GUID *Guid OPTIONAL, IN UINTN CallBackToken, IN OUT VOID *TokenData, IN UINTN TokenDataSize);

typedef EFI_STATUS (EFIAPI *EFI_PCD_PROTOCOL_CALLBACK_ON_SET)(IN CONST  EFI_GUID *Guid OPTIONAL, IN UINTN CallBackToken, IN EFI_PCD_PROTOCOL_CALLBACK CallBackFunction);

typedef EFI_STATUS (EFIAPI *EFI_PCD_PROTOCOL_CANCEL_CALLBACK)(IN CONST  EFI_GUID *Guid OPTIONAL, IN UINTN CallBackToken, IN EFI_PCD_PROTOCOL_CALLBACK CallBackFunction);

typedef EFI_STATUS (EFIAPI *EFI_PCD_PROTOCOL_GET_NEXT_TOKEN)(IN CONST  EFI_GUID *Guid, OPTIONAL IN UINTN *TokenNumber);

typedef EFI_STATUS (EFIAPI *EFI_PCD_PROTOCOL_GET_NEXT_TOKEN_SPACE)(IN OUT CONST EFI_GUID **Guid);


typedef struct _EFI_PCD_PROTOCOL {
  EFI_PCD_PROTOCOL_SET_SKU              SetSku;
  EFI_PCD_PROTOCOL_GET_8                Get8;
  EFI_PCD_PROTOCOL_GET_16               Get16;
  EFI_PCD_PROTOCOL_GET_32               Get32;
  EFI_PCD_PROTOCOL_GET_64               Get64;
  EFI_PCD_PROTOCOL_GET_POINTER          GetPtr;
  EFI_PCD_PROTOCOL_GET_BOOLEAN          GetBool;
  EFI_PCD_PROTOCOL_GET_SIZE             GetSize;
  EFI_PCD_PROTOCOL_SET_8                Set8;
  EFI_PCD_PROTOCOL_SET_16               Set16;
  EFI_PCD_PROTOCOL_SET_32               Set32;
  EFI_PCD_PROTOCOL_SET_64               Set64;
  EFI_PCD_PROTOCOL_SET_POINTER          SetPtr;
  EFI_PCD_PROTOCOL_SET_BOOLEAN          SetBool;
  EFI_PCD_PROTOCOL_CALLBACK_ON_SET      CallbackOnSet;
  EFI_PCD_PROTOCOL_CANCEL_CALLBACK      CancelCallback;
  EFI_PCD_PROTOCOL_GET_NEXT_TOKEN       GetNextToken;
  EFI_PCD_PROTOCOL_GET_NEXT_TOKEN_SPACE GetNextTokenSpace;
} EFI_PCD_PROTOCOL;

EFI_PCD_PROTOCOL            *mPiPcd     = NULL;


typedef
INTN
(EFIAPI *EFI_UNICODE_COLLATION_STRICOLL)(
  IN EFI_UNICODE_COLLATION_PROTOCOL         *This,
  IN CHAR16                                 *Str1,
  IN CHAR16                                 *Str2
  );

typedef
BOOLEAN
(EFIAPI *EFI_UNICODE_COLLATION_METAIMATCH)(
  IN EFI_UNICODE_COLLATION_PROTOCOL         *This,
  IN CHAR16                                 *String,
  IN CHAR16                                 *Pattern
  );

typedef
VOID
(EFIAPI *EFI_UNICODE_COLLATION_STRLWR)(
  IN EFI_UNICODE_COLLATION_PROTOCOL         *This,
  IN OUT CHAR16                             *Str
  );

typedef
VOID
(EFIAPI *EFI_UNICODE_COLLATION_STRUPR)(
  IN EFI_UNICODE_COLLATION_PROTOCOL         *This,
  IN OUT CHAR16                             *Str
  );

typedef
VOID
(EFIAPI *EFI_UNICODE_COLLATION_FATTOSTR)(
  IN EFI_UNICODE_COLLATION_PROTOCOL         *This,
  IN UINTN                                  FatSize,
  IN CHAR8                                  *Fat,
  OUT CHAR16                                *String
  );

typedef
BOOLEAN
(EFIAPI *EFI_UNICODE_COLLATION_STRTOFAT)(
  IN EFI_UNICODE_COLLATION_PROTOCOL         *This,
  IN CHAR16                                 *String,
  IN UINTN                                  FatSize,
  OUT CHAR8                                 *Fat
  );

struct _EFI_UNICODE_COLLATION_PROTOCOL {
  	EFI_UNICODE_COLLATION_STRICOLL    StriColl;
  	EFI_UNICODE_COLLATION_METAIMATCH  MetaiMatch;
  	EFI_UNICODE_COLLATION_STRLWR      StrLwr;
  	EFI_UNICODE_COLLATION_STRUPR      StrUpr;
  	EFI_UNICODE_COLLATION_FATTOSTR    FatToStr;
  	EFI_UNICODE_COLLATION_STRTOFAT    StrToFat;
	CHAR8                             *SupportedLanguages;
};




static EFI_UNICODE_COLLATION_PROTOCOL  *mUnicodeCollation  = NULL;


UINTN
EFIAPI
AsciiStrLen (
  IN      CONST CHAR8               *String
  )
{
  UINTN                             Length;

  for (Length = 0; *String != '\0'; String++, Length++) {}

  return Length;
}

UINTN
EFIAPI
AsciiStrSize (
  IN      CONST CHAR8               *String
  )
{
  return (AsciiStrLen (String) + 1) * sizeof (*String);
}

#define MSG PcdGet32(10)

#define PcdGet32(TokenName) PCD_GET_MODE_32_##TokenName

#define PCD_GET_MODE_32_10 MESSAGE


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{

 	UINT32 Index;
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;
	(void)ImageHandle;
	//CHAR16 *InputPcdName = NULL;
  	EFI_GUID              *TokenSpace = NULL;
  	UINTN                 TokenNumber = 0;
  	EFI_PCD_INFO          PcdInfo;
  	UINTN                 PcdNameSize = 0;


  	Index = 0;

  	Status = SystemTable->BootServices->LocateProtocol(&gEfiUnicodeCollation2ProtocolGuid, NULL, (VOID **) &mUnicodeCollation);
	if (EFI_ERROR (Status)) {
	   mUnicodeCollation = NULL;
	   Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Locating Unicode Collation2 Protocol Failed\n\r");
	}
	else {
    Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Locating Unicode Collation2 Protocol Passed\n\r");
	if(EFI_ERROR(Status))
		return Status;
	}

	Status = SystemTable->BootServices->LocateProtocol(&gEfiPcdProtocolGuid, NULL, (VOID **) &mPiPcd);
	if (EFI_ERROR (Status)) {
	   mPiPcd = NULL;
	   Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Locating PCD Protocol Failed\n\r");
	}
	else {
    Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Locating PCD Protocol Passed\n\r");
	if(EFI_ERROR(Status))
		return Status;
	}


	Status = SystemTable->BootServices->LocateProtocol(&gEfiGetPcdInfoProtocolGuid, NULL, (VOID **) &mPiPcdInfo);
	if (EFI_ERROR (Status)) {
	   mPiPcdInfo = NULL;
	   Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"PCD Info Failed\n\r");
	}
	else {
    Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"PCD Info Passed\n\r");
	if(EFI_ERROR(Status))
		return Status;
	}

	UINTN SKUInfo = mPiPcdInfo->GetSku();
	(void)SKUInfo;

	Status = mPiPcd->GetNextToken (TokenSpace, &TokenNumber);
	Status = mPiPcdInfo->GetInfo (TokenSpace, TokenNumber, &PcdInfo);

	//info(SystemTable, (const wchar_t*)PcdInfo.PcdName);
	Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, (const wchar_t*)PcdInfo.PcdName);
	PcdNameSize = AsciiStrSize (PcdInfo.PcdName) * sizeof (CHAR16);
	
	if (PcdNameSize > 0)
	{
	    Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"PCD Name > than zero\n\r" MSG);
		if(EFI_ERROR(Status)) {
			return Status;
		}
	}		
	
	//mUnicodeCollation->MetaiMatch (mUnicodeCollation, mTempPcdNameBuffer, InputPcdName);

	/* Empty the console input buffer to flush out any keystrokes entered before this point. */
	Status = SystemTable->ConIn->Reset(SystemTable->ConIn, false);
	if(EFI_ERROR(Status))
		return Status;
 
	/* Wait for keypress. */
	while((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY) ;
 

	return EFI_SUCCESS;

}
