#ifndef __DRIVER_SAMPLE_H__
#define __DRIVER_SAMPLE_H__

#include <stdlib.h>

#include "efi.h"
#include "log.h"

typedef UINTN  *BASE_LIST;

//typedef int         *va_list[1];
#define VA_LIST     va_list

#define VA_START(Marker, Parameter)   ((void)0)// __va_start(Marker, Parameter)
#define VA_END(Marker)                ((void)0)

#define LEFT_JUSTIFY      0x01
#define COMMA_TYPE        0x08
#define PREFIX_ZERO       0x20
#define RADIX_HEX         0x80

typedef struct _EFI_IFR_OP_HEADER {
  UINT8                    OpCode;
  UINT8                    Length:7;
  UINT8                    Scope:1;
} EFI_IFR_OP_HEADER;

typedef struct _EFI_IFR_GUID_LABEL {
  EFI_IFR_OP_HEADER   Header;
  EFI_GUID            Guid;
  UINT8               ExtendOpCode;
  UINT16              Number;
} EFI_IFR_GUID_LABEL;


typedef
UINTN
(EFIAPI *UNICODE_BS_PRINT)(
  OUT CHAR16        *StartOfBuffer,
  IN  UINTN         BufferSize,
  IN  CONST CHAR16  *FormatString,
  IN  BASE_LIST     Marker
  );


typedef
UINTN
(EFIAPI *UNICODE_BS_PRINT_ASCII_FORMAT)(
  OUT CHAR16       *StartOfBuffer,
  IN  UINTN        BufferSize,
  IN  CONST CHAR8  *FormatString,
  IN  BASE_LIST    Marker
  );

typedef
UINTN
(EFIAPI *UNICODE_S_PRINT)(
  OUT CHAR16        *StartOfBuffer,
  IN  UINTN         BufferSize,
  IN  CONST CHAR16  *FormatString,
  ...
  );

typedef
UINTN
(EFIAPI *UNICODE_S_PRINT_ASCII_FORMAT)(
  OUT CHAR16       *StartOfBuffer,
  IN  UINTN        BufferSize,
  IN  CONST CHAR8  *FormatString,
  ...
  );

typedef
UINTN
(EFIAPI *UNICODE_VALUE_TO_STRING)(
  IN OUT CHAR16  *Buffer,
  IN UINTN       Flags,
  IN INT64       Value,
  IN UINTN       Width
  );


typedef
UINTN
(EFIAPI *ASCII_BS_PRINT)(
  OUT CHAR8         *StartOfBuffer,
  IN  UINTN         BufferSize,
  IN  CONST CHAR8   *FormatString,
  IN  BASE_LIST     Marker
  );

typedef
UINTN
(EFIAPI *ASCII_S_PRINT)(
  OUT CHAR8        *StartOfBuffer,
  IN  UINTN        BufferSize,
  IN  CONST CHAR8  *FormatString,
  ...
  );

typedef
UINTN
(EFIAPI *ASCII_BS_PRINT_UNICODE_FORMAT)(
  OUT CHAR8         *StartOfBuffer,
  IN  UINTN         BufferSize,
  IN  CONST CHAR16  *FormatString,
  IN  BASE_LIST     Marker
  );

typedef
UINTN
(EFIAPI *ASCII_S_PRINT_UNICODE_FORMAT)(
  OUT CHAR8         *StartOfBuffer,
  IN  UINTN         BufferSize,
  IN  CONST CHAR16  *FormatString,
  ...
  );


typedef
UINTN
(EFIAPI *ASCII_VALUE_TO_STRING)(
  OUT CHAR8      *Buffer,
  IN  UINTN      Flags,
  IN  INT64      Value,
  IN  UINTN      Width
  );


UINTN
EFIAPI
UnicodeBSPrint (
  OUT CHAR16        *StartOfBuffer,
  IN  UINTN         BufferSize,
  IN  CONST CHAR16  *FormatString,
  IN  BASE_LIST     Marker
  );

UINTN
EFIAPI
UnicodeBSPrintAsciiFormat (
  OUT CHAR16       *StartOfBuffer,
  IN  UINTN        BufferSize,
  IN  CONST CHAR8  *FormatString,
  IN  BASE_LIST    Marker
  );

RETURN_STATUS
EFIAPI
UnicodeValueToStringS (
  IN OUT CHAR16  *Buffer,
  IN UINTN       BufferSize,
  IN UINTN       Flags,
  IN INT64       Value,
  IN UINTN       Width
  );


typedef struct _EFI_PRINT2_PROTOCOL  EFI_PRINT2_PROTOCOL;



struct _EFI_PRINT2_PROTOCOL {
  UNICODE_BS_PRINT                     UnicodeBSPrint;
  UNICODE_S_PRINT                      UnicodeSPrint;
  UNICODE_BS_PRINT_ASCII_FORMAT        UnicodeBSPrintAsciiFormat;
  UNICODE_S_PRINT_ASCII_FORMAT         UnicodeSPrintAsciiFormat;
  UNICODE_VALUE_TO_STRING              UnicodeValueToString;
  ASCII_BS_PRINT                       AsciiBSPrint;
  ASCII_S_PRINT                        AsciiSPrint;
  ASCII_BS_PRINT_UNICODE_FORMAT        AsciiBSPrintUnicodeFormat;
  ASCII_S_PRINT_UNICODE_FORMAT         AsciiSPrintUnicodeFormat;
  ASCII_VALUE_TO_STRING                AsciiValueToString;
};

#define EFI_PRINT2S_PROTOCOL_GUID  \
  { 0xcc252d2, 0xc106, 0x4661, { 0xb5, 0xbd, 0x31, 0x47, 0xa4, 0xf8, 0x1f, 0x92 } }


#define ASSERT_UNICODE_BUFFER(Buffer) ASSERT ((((UINTN) (Buffer)) & 0x01) == 0)




#define EFI_HII_DEFAULT_CLASS_STANDARD       0x0000
#define EFI_HII_DEFAULT_CLASS_MANUFACTURING  0x0001
#define EFI_HII_DEFAULT_CLASS_SAFE           0x0002
#define EFI_HII_DEFAULT_CLASS_PLATFORM_BEGIN 0x4000
#define EFI_HII_DEFAULT_CLASS_PLATFORM_END   0x7fff
#define EFI_HII_DEFAULT_CLASS_HARDWARE_BEGIN 0x8000
#define EFI_HII_DEFAULT_CLASS_HARDWARE_END   0xbfff
#define EFI_HII_DEFAULT_CLASS_FIRMWARE_BEGIN 0xc000
#define EFI_HII_DEFAULT_CLASS_FIRMWARE_END   0xffff

#define OFFSET_OF(TYPE, Field) ((UINTN) &(((TYPE *)0)->Field))

#define BASE_CR(Record, TYPE, Field)  ((TYPE *) ((CHAR8 *) (Record) - OFFSET_OF (TYPE, Field)))

#define CR(Record, TYPE, Field, TestSignature)                                              \
    BASE_CR (Record, TYPE, Field)

#define DRIVER_SAMPLE_PRIVATE_FROM_THIS(a)  CR (a, DRIVER_SAMPLE_PRIVATE_DATA, ConfigAccess, DRIVER_SAMPLE_PRIVATE_SIGNATURE)


typedef struct _EFI_HII_DATABASE_PROTOCOL EFI_HII_DATABASE_PROTOCOL;

#define HW_VENDOR_DP              0x04

#define END_DEVICE_PATH_TYPE                 0x7f
#define END_ENTIRE_DEVICE_PATH_SUBTYPE       0xFF
#define END_INSTANCE_DEVICE_PATH_SUBTYPE     0x01

UINT8  VfrBin[100];
UINT8  InventoryBin[100];
UINT8  DriverSampleStrings[100];

typedef UINT16  EFI_STRING_ID;
typedef UINT16  EFI_QUESTION_ID;
typedef UINT16  EFI_FORM_ID;

typedef struct {
  EFI_QUESTION_ID QuestionId;
  EFI_FORM_ID     FormId;
  EFI_GUID        FormSetGuid;
  EFI_STRING_ID   DevicePath;
} EFI_HII_REF;

typedef UINT8 EFI_KEY_TOGGLE_STATE;

typedef struct _EFI_KEY_STATE {
  UINT32                KeyShiftState;
  EFI_KEY_TOGGLE_STATE  KeyToggleState;
} EFI_KEY_STATE;

typedef struct {
  EFI_INPUT_KEY   Key;
  EFI_KEY_STATE   KeyState;
} EFI_KEY_DATA;

typedef struct {
  UINT8 Hour;
  UINT8 Minute;
  UINT8 Second;
} EFI_HII_TIME;

typedef struct {
  UINT16 Year;
  UINT8  Month;
  UINT8  Day;
} EFI_HII_DATE;

typedef union {
  UINT8    UnionNumeric;
  UINT8    UnionNumericAlias;
} MY_EFI_UNION_DATA;

typedef UINTN   EFI_HII_DATABASE_NOTIFY_TYPE;

#define EFI_HII_DATABASE_NOTIFY_NEW_PACK    0x00000001
#define EFI_HII_DATABASE_NOTIFY_REMOVE_PACK 0x00000002
#define EFI_HII_DATABASE_NOTIFY_EXPORT_PACK 0x00000004
#define EFI_HII_DATABASE_NOTIFY_ADD_PACK    0x00000008

typedef struct {
  UINT32  Length:24;
  UINT32  Type:8;
  // UINT8  Data[...];
} EFI_HII_PACKAGE_HEADER;

typedef struct {
  EFI_DEVICE_PATH_PROTOCOL        Header;
  EFI_GUID                        Guid;
} VENDOR_DEVICE_PATH;

typedef struct {
  EFI_GUID               PackageListGuid;
  UINT32                 PackageLength;
} EFI_HII_PACKAGE_LIST_HEADER;

typedef struct {
  UINT16                  LayoutLength;
  EFI_GUID                Guid;
  UINT32                  LayoutDescriptorStringOffset;
  UINT8                   DescriptorCount;
  // EFI_KEY_DESCRIPTOR    Descriptors[];
} EFI_HII_KEYBOARD_LAYOUT;

typedef
EFI_STATUS
(EFIAPI *EFI_HII_DATABASE_NOTIFY)(
  IN        UINT8                         PackageType,
  IN CONST  EFI_GUID                      *PackageGuid,
  IN CONST  EFI_HII_PACKAGE_HEADER        *Package,
  IN        EFI_HII_HANDLE                 Handle,
  IN        EFI_HII_DATABASE_NOTIFY_TYPE  NotifyType
);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_DATABASE_NEW_PACK)(
  IN CONST  EFI_HII_DATABASE_PROTOCOL   *This,
  IN CONST  EFI_HII_PACKAGE_LIST_HEADER *PackageList,
  IN        EFI_HANDLE                  DriverHandle, OPTIONAL
  OUT       EFI_HII_HANDLE               *Handle
);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_DATABASE_REMOVE_PACK)(
  IN CONST  EFI_HII_DATABASE_PROTOCOL *This,
  IN        EFI_HII_HANDLE             Handle
);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_DATABASE_UPDATE_PACK)(
  IN CONST  EFI_HII_DATABASE_PROTOCOL   *This,
  IN        EFI_HII_HANDLE               Handle,
  IN CONST  EFI_HII_PACKAGE_LIST_HEADER *PackageList
);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_DATABASE_LIST_PACKS)(
  IN CONST  EFI_HII_DATABASE_PROTOCOL *This,
  IN        UINT8                     PackageType,
  IN CONST  EFI_GUID                  *PackageGuid,
  IN OUT    UINTN                     *HandleBufferLength,
  OUT       EFI_HII_HANDLE            *Handle
);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_DATABASE_EXPORT_PACKS)(
  IN CONST  EFI_HII_DATABASE_PROTOCOL      *This,
  IN        EFI_HII_HANDLE                 Handle,
  IN OUT    UINTN                          *BufferSize,
  OUT       EFI_HII_PACKAGE_LIST_HEADER    *Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_DATABASE_REGISTER_NOTIFY)(
  IN CONST  EFI_HII_DATABASE_PROTOCOL     *This,
  IN        UINT8                         PackageType,
  IN CONST  EFI_GUID                      *PackageGuid,
  IN        EFI_HII_DATABASE_NOTIFY       PackageNotifyFn,
  IN        EFI_HII_DATABASE_NOTIFY_TYPE  NotifyType,
  OUT       EFI_HANDLE                    *NotifyHandle
);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_DATABASE_UNREGISTER_NOTIFY)(
  IN CONST  EFI_HII_DATABASE_PROTOCOL *This,
  IN        EFI_HANDLE                NotificationHandle
);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_FIND_KEYBOARD_LAYOUTS)(
  IN CONST  EFI_HII_DATABASE_PROTOCOL *This,
  IN OUT    UINT16                    *KeyGuidBufferLength,
  OUT       EFI_GUID                  *KeyGuidBuffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_GET_KEYBOARD_LAYOUT)(
  IN CONST  EFI_HII_DATABASE_PROTOCOL *This,
  IN CONST  EFI_GUID                  *KeyGuid,
  IN OUT UINT16                       *KeyboardLayoutLength,
  OUT       EFI_HII_KEYBOARD_LAYOUT   *KeyboardLayout
);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_SET_KEYBOARD_LAYOUT)(
  IN CONST  EFI_HII_DATABASE_PROTOCOL *This,
  IN CONST  EFI_GUID                  *KeyGuid
);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_DATABASE_GET_PACK_HANDLE)(
  IN CONST  EFI_HII_DATABASE_PROTOCOL *This,
  IN        EFI_HII_HANDLE             PackageListHandle,
  OUT       EFI_HANDLE                *DriverHandle
);

struct _EFI_HII_DATABASE_PROTOCOL {
  EFI_HII_DATABASE_NEW_PACK           NewPackageList;
  EFI_HII_DATABASE_REMOVE_PACK        RemovePackageList;
  EFI_HII_DATABASE_UPDATE_PACK        UpdatePackageList;
  EFI_HII_DATABASE_LIST_PACKS         ListPackageLists;
  EFI_HII_DATABASE_EXPORT_PACKS       ExportPackageLists;
  EFI_HII_DATABASE_REGISTER_NOTIFY    RegisterPackageNotify;
  EFI_HII_DATABASE_UNREGISTER_NOTIFY  UnregisterPackageNotify;
  EFI_HII_FIND_KEYBOARD_LAYOUTS       FindKeyboardLayouts;
  EFI_HII_GET_KEYBOARD_LAYOUT         GetKeyboardLayout;
  EFI_HII_SET_KEYBOARD_LAYOUT         SetKeyboardLayout;
  EFI_HII_DATABASE_GET_PACK_HANDLE    GetPackageListHandle;
};


typedef struct {
  UINT16  MyStringData[40];
  UINT16  SomethingHiddenForHtml;
  UINT8   HowOldAreYouInYearsManual;
  UINT16  HowTallAreYouManual;
  UINT8   HowOldAreYouInYears;
  UINT16  HowTallAreYou;
  UINT8   MyFavoriteNumber;
  UINT8   TestLateCheck;
  UINT8   TestLateCheck2;
  UINT8   QuestionAboutTreeHugging;
  UINT8   ChooseToActivateNuclearWeaponry;
  UINT8   SuppressGrayOutSomething;
  UINT8   OrderedList[8];
  UINT16  BootOrder[8];
  UINT8   BootOrderLarge;
  UINT8   DynamicRefresh;
  UINT8   DynamicOneof;
  UINT8   DynamicOrderedList[5];
  UINT8   Reserved;
  EFI_HII_REF RefData;
  UINT8   NameValueVar0;
  UINT16  NameValueVar1;
  UINT16  NameValueVar2[20];
  UINT8   SerialPortNo;
  UINT8   SerialPortStatus;
  UINT16  SerialPortIo;
  UINT8   SerialPortIrq;
  UINT8   GetDefaultValueFromCallBack;
  UINT8   GetDefaultValueFromAccess;
  EFI_HII_TIME  Time;
  UINT8   RefreshGuidCount;
  UINT8   Match2;
  UINT8   GetDefaultValueFromCallBackForOrderedList[3];
  UINT8   BitCheckbox : 1;
  UINT8   ReservedBits: 7;  // Reserved bit fields for padding.
  UINT16  BitOneof    : 6;
  UINT16              : 0;  // Width 0 used to force alignment.
  UINT16  BitNumeric  : 12;
  MY_BITS_DATA  MyBitData;
  MY_EFI_UNION_DATA MyUnionData;
  UINT8   QuestionXUefiKeywordRestStyle;
  UINT8   QuestionNonXUefiKeywordRestStyle;
} DRIVER_SAMPLE_CONFIGURATION;


typedef struct _EFI_HII_STRING_PROTOCOL EFI_HII_STRING_PROTOCOL;


#define DYNAMIC_ONE_OF_VAR_OFFSET        OFFSET_OF (DRIVER_SAMPLE_CONFIGURATION, DynamicOneof)
#define DYNAMIC_ORDERED_LIST_VAR_OFFSET  OFFSET_OF (DRIVER_SAMPLE_CONFIGURATION, DynamicOrderedList)

#define DEFAULT_CLASS_MANUFACTURING_VALUE     0xFF
#define DEFAULT_CLASS_STANDARD_VALUE          0x0

#define NAME_VALUE_NAME_NUMBER       3

#define DRIVER_SAMPLE_PRIVATE_SIGNATURE SIGNATURE_32 ('D', 'S', 'p', 's')


typedef struct _EFI_HII_CONFIG_ROUTING_PROTOCOL EFI_HII_CONFIG_ROUTING_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI * EFI_HII_EXTRACT_CONFIG)(
  IN CONST  EFI_HII_CONFIG_ROUTING_PROTOCOL *This,
  IN CONST  EFI_STRING                      Request,
  OUT       EFI_STRING                      *Progress,
  OUT       EFI_STRING                      *Results
);

typedef
EFI_STATUS
(EFIAPI * EFI_HII_EXPORT_CONFIG)(
  IN CONST  EFI_HII_CONFIG_ROUTING_PROTOCOL *This,
  OUT       EFI_STRING                      *Results
);

typedef
EFI_STATUS
(EFIAPI * EFI_HII_ROUTE_CONFIG)(
  IN CONST  EFI_HII_CONFIG_ROUTING_PROTOCOL *This,
  IN CONST  EFI_STRING                      Configuration,
  OUT       EFI_STRING                      *Progress
);

typedef
EFI_STATUS
(EFIAPI * EFI_HII_BLOCK_TO_CONFIG)(
  IN CONST  EFI_HII_CONFIG_ROUTING_PROTOCOL *This,
  IN CONST  EFI_STRING                      ConfigRequest,
  IN CONST  UINT8                           *Block,
  IN CONST  UINTN                           BlockSize,
  OUT       EFI_STRING                      *Config,
  OUT       EFI_STRING                      *Progress
);

typedef
EFI_STATUS
(EFIAPI * EFI_HII_CONFIG_TO_BLOCK)(
  IN CONST  EFI_HII_CONFIG_ROUTING_PROTOCOL *This,
  IN CONST  EFI_STRING                      ConfigResp,
  IN OUT    UINT8                           *Block,
  IN OUT    UINTN                           *BlockSize,
  OUT       EFI_STRING                      *Progress
);

typedef
EFI_STATUS
(EFIAPI * EFI_HII_GET_ALT_CFG)(
  IN  CONST EFI_HII_CONFIG_ROUTING_PROTOCOL    *This,
  IN  CONST EFI_STRING                         ConfigResp,
  IN  CONST EFI_GUID                           *Guid,
  IN  CONST EFI_STRING                         Name,
  IN  CONST EFI_DEVICE_PATH_PROTOCOL           *DevicePath,
  IN  CONST UINT16                             *AltCfgId,
  OUT EFI_STRING                               *AltCfgResp
  );

struct _EFI_HII_CONFIG_ROUTING_PROTOCOL {
  EFI_HII_EXTRACT_CONFIG  ExtractConfig;
  EFI_HII_EXPORT_CONFIG   ExportConfig;
  EFI_HII_ROUTE_CONFIG    RouteConfig;
  EFI_HII_BLOCK_TO_CONFIG BlockToConfig;
  EFI_HII_CONFIG_TO_BLOCK ConfigToBlock;
  EFI_HII_GET_ALT_CFG     GetAltConfig;
};

#define KEYWORD_HANDLER_NO_ERROR                       0x00000000
#define KEYWORD_HANDLER_NAMESPACE_ID_NOT_FOUND         0x00000001
#define KEYWORD_HANDLER_MALFORMED_STRING               0x00000002
#define KEYWORD_HANDLER_KEYWORD_NOT_FOUND              0x00000004
#define KEYWORD_HANDLER_INCOMPATIBLE_VALUE_DETECTED    0x00000008
#define KEYWORD_HANDLER_ACCESS_NOT_PERMITTED           0x00000010
#define KEYWORD_HANDLER_UNDEFINED_PROCESSING_ERROR     0x80000000

typedef struct _EFI_CONFIG_KEYWORD_HANDLER_PROTOCOL EFI_CONFIG_KEYWORD_HANDLER_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_CONFIG_KEYWORD_HANDLER_SET_DATA) (
  IN EFI_CONFIG_KEYWORD_HANDLER_PROTOCOL *This,
  IN CONST EFI_STRING                    KeywordString,
  OUT EFI_STRING                         *Progress,
  OUT UINT32                             *ProgressErr
  );

typedef
EFI_STATUS
(EFIAPI *EFI_CONFIG_KEYWORD_HANDLER_GET_DATA) (
  IN EFI_CONFIG_KEYWORD_HANDLER_PROTOCOL  *This,
  IN CONST EFI_STRING                     NameSpaceId, OPTIONAL
  IN CONST EFI_STRING                     KeywordString, OPTIONAL
  OUT EFI_STRING                          *Progress,
  OUT UINT32                              *ProgressErr,
  OUT EFI_STRING                          *Results
  );


struct _EFI_CONFIG_KEYWORD_HANDLER_PROTOCOL {
  EFI_CONFIG_KEYWORD_HANDLER_SET_DATA  SetData;
  EFI_CONFIG_KEYWORD_HANDLER_GET_DATA  GetData;
};


typedef struct _EFI_HII_POPUP_PROTOCOL EFI_HII_POPUP_PROTOCOL;



typedef enum {
  EfiHiiPopupStyleInfo,
  EfiHiiPopupStyleWarning,
  EfiHiiPopupStyleError
} EFI_HII_POPUP_STYLE;

typedef enum {
  EfiHiiPopupTypeOk,
  EfiHiiPopupTypeOkCancel,
  EfiHiiPopupTypeYesNo,
  EfiHiiPopupTypeYesNoCancel
} EFI_HII_POPUP_TYPE;

typedef enum {
  EfiHiiPopupSelectionOk,
  EfiHiiPopupSelectionCancel,
  EfiHiiPopupSelectionYes,
  EfiHiiPopupSelectionNo
} EFI_HII_POPUP_SELECTION;

typedef
EFI_STATUS
(EFIAPI * EFI_HII_CREATE_POPUP) (
  IN  EFI_HII_POPUP_PROTOCOL  *This,
  IN  EFI_HII_POPUP_STYLE     PopupStyle,
  IN  EFI_HII_POPUP_TYPE      PopupType,
  IN  EFI_HII_HANDLE          HiiHandle,
  IN  EFI_STRING_ID           Message,
  OUT EFI_HII_POPUP_SELECTION *UserSelection OPTIONAL
);

struct _EFI_HII_POPUP_PROTOCOL {
  UINT64                Revision;
  EFI_HII_CREATE_POPUP  CreatePopup;
};


typedef struct _EFI_FORM_BROWSER2_PROTOCOL   EFI_FORM_BROWSER2_PROTOCOL;

typedef struct {
  UINTN   LeftColumn;
  UINTN   RightColumn;
  UINTN   TopRow;
  UINTN   BottomRow;
} EFI_SCREEN_DESCRIPTOR;

typedef UINTN EFI_BROWSER_ACTION_REQUEST;

#define EFI_BROWSER_ACTION_REQUEST_NONE   0
#define EFI_BROWSER_ACTION_REQUEST_RESET  1
#define EFI_BROWSER_ACTION_REQUEST_SUBMIT 2
#define EFI_BROWSER_ACTION_REQUEST_EXIT   3
#define EFI_BROWSER_ACTION_REQUEST_FORM_SUBMIT_EXIT  4
#define EFI_BROWSER_ACTION_REQUEST_FORM_DISCARD_EXIT 5
#define EFI_BROWSER_ACTION_REQUEST_FORM_APPLY        6
#define EFI_BROWSER_ACTION_REQUEST_FORM_DISCARD      7
#define EFI_BROWSER_ACTION_REQUEST_RECONNECT         8


typedef
EFI_STATUS
(EFIAPI *EFI_SEND_FORM2)(
  IN CONST  EFI_FORM_BROWSER2_PROTOCOL  *This,
  IN        EFI_HII_HANDLE              *Handle,
  IN        UINTN                      HandleCount,
  IN        EFI_GUID                   *FormSetGuid, OPTIONAL
  IN        EFI_FORM_ID                FormId, OPTIONAL
  IN CONST  EFI_SCREEN_DESCRIPTOR      *ScreenDimensions, OPTIONAL
  OUT       EFI_BROWSER_ACTION_REQUEST *ActionRequest  OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_BROWSER_CALLBACK2)(
  IN CONST  EFI_FORM_BROWSER2_PROTOCOL *This,
  IN OUT    UINTN                     *ResultsDataSize,
  IN OUT    EFI_STRING                ResultsData,
  IN CONST  BOOLEAN                   RetrieveData,
  IN CONST  EFI_GUID                  *VariableGuid, OPTIONAL
  IN CONST  CHAR16                    *VariableName OPTIONAL
);

struct _EFI_FORM_BROWSER2_PROTOCOL {
  EFI_SEND_FORM2         SendForm;
  EFI_BROWSER_CALLBACK2  BrowserCallback;
} ;


typedef struct _EFI_HII_CONFIG_ACCESS_PROTOCOL  EFI_HII_CONFIG_ACCESS_PROTOCOL;


typedef UINTN EFI_BROWSER_ACTION;

#define EFI_BROWSER_ACTION_CHANGING   0
#define EFI_BROWSER_ACTION_CHANGED    1
#define EFI_BROWSER_ACTION_RETRIEVE   2
#define EFI_BROWSER_ACTION_FORM_OPEN  3
#define EFI_BROWSER_ACTION_FORM_CLOSE 4
#define EFI_BROWSER_ACTION_SUBMITTED  5
#define EFI_BROWSER_ACTION_DEFAULT_STANDARD      0x1000
#define EFI_BROWSER_ACTION_DEFAULT_MANUFACTURING 0x1001
#define EFI_BROWSER_ACTION_DEFAULT_SAFE          0x1002
#define EFI_BROWSER_ACTION_DEFAULT_PLATFORM      0x2000
#define EFI_BROWSER_ACTION_DEFAULT_HARDWARE      0x3000
#define EFI_BROWSER_ACTION_DEFAULT_FIRMWARE      0x4000

typedef union {
  UINT8           u8;
  UINT16          u16;
  UINT32          u32;
  UINT64          u64;
  BOOLEAN         b;
  EFI_HII_TIME    time;
  EFI_HII_DATE    date;
  EFI_STRING_ID   string;
  EFI_HII_REF     ref;
} EFI_IFR_TYPE_VALUE;

typedef
EFI_STATUS
(EFIAPI * EFI_HII_ACCESS_EXTRACT_CONFIG)(
  IN CONST  EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST  EFI_STRING                      Request,
  OUT       EFI_STRING                      *Progress,
  OUT       EFI_STRING                      *Results
);

typedef
EFI_STATUS
(EFIAPI * EFI_HII_ACCESS_ROUTE_CONFIG)(
  IN CONST  EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST  EFI_STRING                      Configuration,
  OUT       EFI_STRING                      *Progress
);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_ACCESS_FORM_CALLBACK)(
  IN     CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN     EFI_BROWSER_ACTION                     Action,
  IN     EFI_QUESTION_ID                        QuestionId,
  IN     UINT8                                  Type,
  IN OUT EFI_IFR_TYPE_VALUE                     *Value,
  OUT    EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  );


struct _EFI_HII_CONFIG_ACCESS_PROTOCOL {
  EFI_HII_ACCESS_EXTRACT_CONFIG     ExtractConfig;
  EFI_HII_ACCESS_ROUTE_CONFIG       RouteConfig;
  EFI_HII_ACCESS_FORM_CALLBACK      Callback;
};


typedef struct {
  UINTN                            Signature;

  EFI_HANDLE                       DriverHandle[2];
  EFI_HII_HANDLE                   HiiHandle[2];
  DRIVER_SAMPLE_CONFIGURATION      Configuration;
  MY_EFI_VARSTORE_DATA             VarStoreConfig;
  MY_EFI_BITS_VARSTORE_DATA        BitsVarStoreConfig;
  MY_EFI_UNION_DATA                UnionConfig;

  EFI_STRING_ID                    NameStringId[NAME_VALUE_NAME_NUMBER];
  EFI_STRING                       NameValueName[NAME_VALUE_NAME_NUMBER];

  EFI_HII_DATABASE_PROTOCOL        *HiiDatabase;
  EFI_HII_STRING_PROTOCOL          *HiiString;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;
  EFI_CONFIG_KEYWORD_HANDLER_PROTOCOL *HiiKeywordHandler;
  EFI_HII_POPUP_PROTOCOL              *HiiPopup;

  EFI_FORM_BROWSER2_PROTOCOL       *FormBrowser2;

  EFI_HII_CONFIG_ACCESS_PROTOCOL   ConfigAccess;
} DRIVER_SAMPLE_PRIVATE_DATA;

#define DRIVER_SAMPLE_PRIVATE_FROM_THIS(a)  CR (a, DRIVER_SAMPLE_PRIVATE_DATA, ConfigAccess, DRIVER_SAMPLE_PRIVATE_SIGNATURE)

#pragma pack(1)

typedef struct {
  VENDOR_DEVICE_PATH             VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL       End;
} HII_VENDOR_DEVICE_PATH;

#pragma pack()

EFI_BOOT_SERVICES* gBS;
EFI_RUNTIME_SERVICES* gRT;

#define EFI_SHIFT_STATE_VALID     0x80000000
#define EFI_RIGHT_SHIFT_PRESSED   0x00000001
#define EFI_LEFT_SHIFT_PRESSED    0x00000002
#define EFI_RIGHT_CONTROL_PRESSED 0x00000004
#define EFI_LEFT_CONTROL_PRESSED  0x00000008
#define EFI_RIGHT_ALT_PRESSED     0x00000010
#define EFI_LEFT_ALT_PRESSED      0x00000020
#define EFI_RIGHT_LOGO_PRESSED    0x00000040
#define EFI_LEFT_LOGO_PRESSED     0x00000080
#define EFI_MENU_KEY_PRESSED      0x00000100
#define EFI_SYS_REQ_PRESSED       0x00000200

#define EFI_TOGGLE_STATE_VALID    0x80
#define EFI_KEY_STATE_EXPOSED     0x40
#define EFI_SCROLL_LOCK_ACTIVE    0x01
#define EFI_NUM_LOCK_ACTIVE       0x02
#define EFI_CAPS_LOCK_ACTIVE      0x04

#define SCAN_F11                  0x0015
#define SCAN_F12                  0x0016
#define SCAN_PAUSE                0x0048
#define SCAN_F13                  0x0068
#define SCAN_F14                  0x0069
#define SCAN_F15                  0x006A
#define SCAN_F16                  0x006B
#define SCAN_F17                  0x006C
#define SCAN_F18                  0x006D
#define SCAN_F19                  0x006E
#define SCAN_F20                  0x006F
#define SCAN_F21                  0x0070
#define SCAN_F22                  0x0071
#define SCAN_F23                  0x0072
#define SCAN_F24                  0x0073
#define SCAN_MUTE                 0x007F
#define SCAN_VOLUME_UP            0x0080
#define SCAN_VOLUME_DOWN          0x0081
#define SCAN_BRIGHTNESS_UP        0x0100
#define SCAN_BRIGHTNESS_DOWN      0x0101
#define SCAN_SUSPEND              0x0102
#define SCAN_HIBERNATE            0x0103
#define SCAN_TOGGLE_DISPLAY       0x0104
#define SCAN_RECOVERY             0x0105
#define SCAN_EJECT                0x0106

#define SCAN_NULL       0x0000
#define SCAN_UP         0x0001
#define SCAN_DOWN       0x0002
#define SCAN_RIGHT      0x0003
#define SCAN_LEFT       0x0004
#define SCAN_HOME       0x0005
#define SCAN_END        0x0006
#define SCAN_INSERT     0x0007
#define SCAN_DELETE     0x0008
#define SCAN_PAGE_UP    0x0009
#define SCAN_PAGE_DOWN  0x000A
#define SCAN_F1         0x000B
#define SCAN_F2         0x000C
#define SCAN_F3         0x000D
#define SCAN_F4         0x000E
#define SCAN_F5         0x000F
#define SCAN_F6         0x0010
#define SCAN_F7         0x0011
#define SCAN_F8         0x0012
#define SCAN_F9         0x0013
#define SCAN_F10        0x0014
#define SCAN_ESC        0x0017

typedef struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL;



typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_READ_KEY_EX)(
  IN  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  OUT EFI_KEY_DATA                      *KeyData
);

typedef
EFI_STATUS
(EFIAPI *EFI_SET_STATE)(
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  IN EFI_KEY_TOGGLE_STATE              *KeyToggleState
);

typedef
EFI_STATUS
(EFIAPI *EFI_KEY_NOTIFY_FUNCTION)(
  IN EFI_KEY_DATA *KeyData
);

typedef
EFI_STATUS
(EFIAPI *EFI_REGISTER_KEYSTROKE_NOTIFY)(
  IN  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  IN  EFI_KEY_DATA                      *KeyData,
  IN  EFI_KEY_NOTIFY_FUNCTION           KeyNotificationFunction,
  OUT VOID                              **NotifyHandle
);

typedef
EFI_STATUS
(EFIAPI *EFI_UNREGISTER_KEYSTROKE_NOTIFY)(
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL  *This,
  IN VOID                               *NotificationHandle
);


typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_RESET_EX)(
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  IN BOOLEAN                           ExtendedVerification
);


struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL{
  EFI_INPUT_RESET_EX              Reset;
  EFI_INPUT_READ_KEY_EX           ReadKeyStrokeEx;
  ///
  /// Event to use with WaitForEvent() to wait for a key to be available.
  ///
  EFI_EVENT                       WaitForKeyEx;
  EFI_SET_STATE                   SetState;
  EFI_REGISTER_KEYSTROKE_NOTIFY   RegisterKeyNotify;
  EFI_UNREGISTER_KEYSTROKE_NOTIFY UnregisterKeyNotify;
};


EFI_STRING
EFIAPI
HiiGetString (
  IN EFI_HII_HANDLE  HiiHandle,
  IN EFI_STRING_ID   StringId,
  IN CONST CHAR8     *Language  OPTIONAL
  );


#define ENCODE_ERROR(a)              ((RETURN_STATUS)(MAX_BIT | (a)))

#define ENCODE_WARNING(a)            ((RETURN_STATUS)(a))
#define RETURN_ERROR(a)              (((INTN)(RETURN_STATUS)(a)) < 0)

#define RETURN_SUCCESS               0
#define RETURN_LOAD_ERROR            ENCODE_ERROR (1)
#define RETURN_INVALID_PARAMETER     ENCODE_ERROR (2)
#define RETURN_UNSUPPORTED           ENCODE_ERROR (3)
#define RETURN_BAD_BUFFER_SIZE       ENCODE_ERROR (4)
#define RETURN_BUFFER_TOO_SMALL      ENCODE_ERROR (5)
#define RETURN_NOT_READY             ENCODE_ERROR (6)
#define RETURN_DEVICE_ERROR          ENCODE_ERROR (7)
#define RETURN_WRITE_PROTECTED       ENCODE_ERROR (8)
#define RETURN_OUT_OF_RESOURCES      ENCODE_ERROR (9)
#define RETURN_VOLUME_CORRUPTED      ENCODE_ERROR (10)
#define RETURN_VOLUME_FULL           ENCODE_ERROR (11)
#define RETURN_NO_MEDIA              ENCODE_ERROR (12)
#define RETURN_MEDIA_CHANGED         ENCODE_ERROR (13)
#define RETURN_NOT_FOUND             ENCODE_ERROR (14)
#define RETURN_ACCESS_DENIED         ENCODE_ERROR (15)
#define RETURN_NO_RESPONSE           ENCODE_ERROR (16)
#define RETURN_NO_MAPPING            ENCODE_ERROR (17)
#define RETURN_TIMEOUT               ENCODE_ERROR (18)
#define RETURN_NOT_STARTED           ENCODE_ERROR (19)
#define RETURN_ALREADY_STARTED       ENCODE_ERROR (20)
#define RETURN_ABORTED               ENCODE_ERROR (21)
#define RETURN_ICMP_ERROR            ENCODE_ERROR (22)
#define RETURN_TFTP_ERROR            ENCODE_ERROR (23)
#define RETURN_PROTOCOL_ERROR        ENCODE_ERROR (24)
#define RETURN_INCOMPATIBLE_VERSION  ENCODE_ERROR (25)
#define RETURN_SECURITY_VIOLATION    ENCODE_ERROR (26)
#define RETURN_CRC_ERROR             ENCODE_ERROR (27)
#define RETURN_END_OF_MEDIA          ENCODE_ERROR (28)
#define RETURN_END_OF_FILE           ENCODE_ERROR (31)

#define RETURN_WARN_UNKNOWN_GLYPH    ENCODE_WARNING (1)
#define RETURN_WARN_DELETE_FAILURE   ENCODE_WARNING (2)
#define RETURN_WARN_WRITE_FAILURE    ENCODE_WARNING (3)
#define RETURN_WARN_BUFFER_TOO_SMALL ENCODE_WARNING (4)

#define EFI_LOAD_ERROR            RETURN_LOAD_ERROR
#define EFI_INVALID_PARAMETER     RETURN_INVALID_PARAMETER
#define EFI_UNSUPPORTED           RETURN_UNSUPPORTED
#define EFI_BAD_BUFFER_SIZE       RETURN_BAD_BUFFER_SIZE
#define EFI_BUFFER_TOO_SMALL      RETURN_BUFFER_TOO_SMALL
#define EFI_DEVICE_ERROR          RETURN_DEVICE_ERROR
#define EFI_WRITE_PROTECTED       RETURN_WRITE_PROTECTED
#define EFI_OUT_OF_RESOURCES      RETURN_OUT_OF_RESOURCES
#define EFI_VOLUME_CORRUPTED      RETURN_VOLUME_CORRUPTED
#define EFI_VOLUME_FULL           RETURN_VOLUME_FULL
#define EFI_NO_MEDIA              RETURN_NO_MEDIA
#define EFI_MEDIA_CHANGED         RETURN_MEDIA_CHANGED
#define EFI_NOT_FOUND             RETURN_NOT_FOUND
#define EFI_ACCESS_DENIED         RETURN_ACCESS_DENIED
#define EFI_NO_RESPONSE           RETURN_NO_RESPONSE
#define EFI_NO_MAPPING            RETURN_NO_MAPPING
#define EFI_TIMEOUT               RETURN_TIMEOUT
#define EFI_NOT_STARTED           RETURN_NOT_STARTED
#define EFI_ALREADY_STARTED       RETURN_ALREADY_STARTED
#define EFI_ABORTED               RETURN_ABORTED
#define EFI_ICMP_ERROR            RETURN_ICMP_ERROR
#define EFI_TFTP_ERROR            RETURN_TFTP_ERROR
#define EFI_PROTOCOL_ERROR        RETURN_PROTOCOL_ERROR
#define EFI_INCOMPATIBLE_VERSION  RETURN_INCOMPATIBLE_VERSION
#define EFI_SECURITY_VIOLATION    RETURN_SECURITY_VIOLATION
#define EFI_CRC_ERROR             RETURN_CRC_ERROR
#define EFI_END_OF_MEDIA          RETURN_END_OF_MEDIA
#define EFI_END_OF_FILE           RETURN_END_OF_FILE

#define EFI_WARN_UNKNOWN_GLYPH    RETURN_WARN_UNKNOWN_GLYPH
#define EFI_WARN_DELETE_FAILURE   RETURN_WARN_DELETE_FAILURE
#define EFI_WARN_WRITE_FAILURE    RETURN_WARN_WRITE_FAILURE
#define EFI_WARN_BUFFER_TOO_SMALL RETURN_WARN_BUFFER_TOO_SMALL


#endif
