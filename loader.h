#ifndef __LOADER_H__
#define __LOADER_H__


#include <stddef.h>
#include <stdint.h>

#include "efi.h"
#include "log.h"

static const uint32_t EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL = 0x00000001;

typedef void *efi_handle_t;
typedef uint64_t efi_status_t;
typedef uint64_t efi_uint_t;

#define ELFABI __attribute__((sysv_abi))

static const size_t EI_MAG0 = 0;
static const size_t EI_MAG1 = 1;
static const size_t EI_MAG2 = 2;
static const size_t EI_MAG3 = 3;
static const size_t EI_CLASS = 4;
static const size_t EI_DATA = 5;
static const size_t EI_VERSION = 6;
static const size_t EI_OSABI = 7;
static const size_t EI_ABIVERSION = 8;
static const size_t EI_PAD = 9;

static const uint32_t PT_NULL = 0;
static const uint32_t PT_LOAD = 1;
static const uint32_t PT_DYNAMIC = 2;
static const uint32_t PT_INTERP = 3;
static const uint32_t PT_NOTE = 4;
static const uint32_t PT_SHLIB = 5;
static const uint32_t PT_PHDR = 6;
static const uint32_t PT_TLS = 7;

static const uint32_t PF_X = 1;
static const uint32_t PF_W = 2;
static const uint32_t PF_R = 4;

static const uint16_t ET_NONE = 0;
static const uint16_t ET_REL = 1;
static const uint16_t ET_EXEC = 2;
static const uint16_t ET_DYN = 3;
static const uint16_t ET_CORE = 4;

static const efi_status_t EFI_LOAD_ERROR = 1;
static const efi_status_t EFI_INVALID_PARAMETER = 2;
static const efi_status_t EFI_UNSUPPORTED = 3;
static const efi_status_t EFI_BUFFER_TOO_SMALL = 5;

static const uint16_t EM_X86_64 = 62;
static const uint16_t EM_AARCH64 = 183;

static const unsigned char ELFCLASSNONE = 0;
static const unsigned char ELFCLASS32 = 1;
static const unsigned char ELFCLASS64 = 2;

static const unsigned char ELFDATANONE = 0;
static const unsigned char ELFDATA2LSB = 1;
static const unsigned char ELFDATA2MSB = 2;


struct module {
	const uint16_t *path;
	const char *name;
};

static const uint64_t EFI_FILE_MODE_READ = 0x0000000000000001;
static const uint64_t EFI_FILE_MODE_WRITE = 0x0000000000000002;
static const uint64_t EFI_FILE_MODE_CREATE = 0x0000000000000008;

static const uint64_t EFI_FILE_READ_ONLY = 0x1;
static const uint64_t EFI_FILE_HIDDEN = 0x2;
static const uint64_t EFI_FILE_SYSTEM = 0x4;
static const uint64_t EFI_FILE_RESERVED = 0x8;
static const uint64_t EFI_FILE_DIRECTORY = 0x10;
static const uint64_t EFI_FILE_ARCHIVE = 0x20;


struct efi_time {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t pad1;
	uint32_t nanosecond;
	int16_t time_zone;
	uint8_t daylight;
	uint8_t pad2;
};

struct efi_file_info {
    uint64_t size;
    uint64_t file_size;
    uint64_t physical_size;
    struct efi_time create_time;
    struct efi_time last_access_time;
    struct efi_time modifiction_time;
    uint64_t attribute;
    uint16_t file_name[256];
};

enum efi_allocate_type {
	EFI_ALLOCATE_ANY_PAGES,
	EFI_ALLOCATE_MAX_ADDRESS,
	EFI_ALLOCATE_ADDRESS,
	EFI_MAX_ALLOCATE_TYPE,
};

enum efi_memory_type {
	EFI_RESERVED_MEMORY_TYPE,
	EFI_LOADER_CODE,
	EFI_LOADER_DATA,
	EFI_BOOT_SERVICES_CODE,
	EFI_BOOT_SERVICES_DATA,
	EFI_RUNTIME_SERVICES_CODE,
	EFI_RUNTIME_SERVICES_DATA,
	EFI_CONVENTIAL_MEMORY,
	EFI_UNUSABLE_MEMORY,
	EFI_ACPI_RECLAIM_MEMORY,
	EFI_ACPI_MEMORY_NVS,
	EFI_MEMORY_MAPPED_IO,
	EFI_MEMORY_MAPPED_IO_PORT_SPACE,
	EFI_PAL_CODE,
	EFI_PERSISTENT_MEMORY,
	EFI_MAX_MEMORY_TYPE,
};


struct reserve {
	const char *name;
	uint64_t begin;
	uint64_t end;
};

struct efi_guid {
	uint32_t data1;
	uint16_t data2;
	uint16_t data3;
	uint8_t data4[8];
};

struct efi_memory_descriptor {
	uint32_t type;
	uint64_t physical_start;
	uint64_t virtual_start;
	uint64_t pages;
	uint64_t attributes;
};

struct efi_file_protocol {
    uint64_t revision;
    efi_status_t (*open)(
        struct efi_file_protocol *,
        struct efi_file_protocol **,
        uint16_t *,
        uint64_t,
        uint64_t);
    efi_status_t (*close)(struct efi_file_protocol *);

    void (*unused1)();

    efi_status_t (*read)(struct efi_file_protocol *, efi_uint_t *, void *);

    void (*unused2)();

    efi_status_t (*get_position)(struct efi_file_protocol *, uint64_t *);
    efi_status_t (*set_position)(struct efi_file_protocol *, uint64_t);

    efi_status_t (*get_info)(
        struct efi_file_protocol *, struct efi_guid *, efi_uint_t *, void *);

    void (*unused6)();
    void (*unused7)();
    void (*unused8)();
    void (*unused9)();
    void (*unused10)();
    void (*unused11)();
};

#define EFI_FILE_INFO_GUID \
    { 0x09576e92, 0x6d3f, 0x11d2, \
      { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }

#define EFI_LOADED_IMAGE_PROTOCOL_GUID \
	{ 0x5b1b31a1, 0x9562, 0x11d2, \
	  { 0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID \
    { 0x0964e5b22, 0x6459, 0x11d2, \
      { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }

struct efi_simple_file_system_protocol {
    uint64_t revision;
    efi_status_t (*open_volume)(
        struct efi_simple_file_system_protocol *, struct efi_file_protocol **);
};

struct efi_table_header {
	uint64_t signature;
	uint32_t revision;
	uint32_t header_size;
	uint32_t crc32;
	uint32_t reserved;
};

struct efi_simple_text_output_protocol
{
	void (*unused1)();

	efi_status_t (*output_string)(
		struct efi_simple_text_output_protocol *,
		uint16_t *);

	void (*unused2)();
	void (*unused3)();
	void (*unused4)();
	void (*unused5)();

	efi_status_t (*clear_screen)(
		struct efi_simple_text_output_protocol *);

	void (*unused6)();
	void (*unused7)();

	void *unused8;
};

struct efi_boot_table
{
	struct efi_table_header header;

	// Task Priority Services
	void (*unused1)();
	void (*unused2)();

	// Memory Services
	efi_status_t (*allocate_pages)(
		enum efi_allocate_type,
		enum efi_memory_type,
		efi_uint_t,
		uint64_t *);
	efi_status_t (*free_pages)(uint64_t, efi_uint_t);
	efi_status_t (*get_memory_map)(
		efi_uint_t *,
		struct efi_memory_descriptor *,
		efi_uint_t *,
		efi_uint_t *,
		uint32_t *);
	efi_status_t (*allocate_pool)(
		enum efi_memory_type, efi_uint_t, void **);
	efi_status_t (*free_pool)(void *);

	// Event & Timer Services
	void (*unused7)();
	void (*unused8)();
	void (*unused9)();
	void (*unused10)();
	void (*unused11)();
	void (*unused12)();

	// Protocol Handler Services
	void (*unused13)();
	void (*unused14)();
	void (*unused15)();
	void (*unused16)();
	void *reserved;
	void (*unused17)();
	void (*unused18)();
	void (*unused19)();
	void (*unused20)();

	// Image Services
	void (*unused21)();
	void (*unused22)();
	void (*unused23)();
	void (*unused24)();

	efi_status_t (*exit_boot_services)(efi_handle_t, efi_uint_t);

	// Miscellaneius Services
	void (*unused26)();
	void (*unused27)();
	void (*unused28)();

	// DriverSupport Services
	void (*unused29)();
	void (*unused30)();

	// Open and Close Protocol Services
	efi_status_t (*open_protocol)(
		efi_handle_t,
		struct efi_guid *,
		void **,
		efi_handle_t,
		efi_handle_t,
		uint32_t);
	efi_status_t (*close_protocol)(
		efi_handle_t,
		struct efi_guid *,
		efi_handle_t,
		efi_handle_t);
	void (*unused33)();

	// Library Services
	efi_status_t (*protocols_per_handle)(
		efi_handle_t, struct efi_guid ***, efi_uint_t *);
	void (*unused35)();
	void (*unused36)();
	void (*unused37)();
	void (*unused38)();

	// 32-bit CRC Services
	void (*unused39)();

	// Miscellaneius Services (cont)
	void (*unused40)();
	void (*unused41)();
	void (*unused42)();
};

struct efi_system_table
{
	struct efi_table_header header;
	uint16_t *unused1;
	uint32_t unused2;
	void *unused3;
	void *unused4;
	void *unused5;
	struct efi_simple_text_output_protocol *out;
	void *unused6;
	struct efi_simple_text_output_protocol *err;
	void *unused8;
	struct efi_boot_table *boot;
	efi_uint_t unused10;
	void *unused11;
};

struct efi_device_path_protocol {
	uint8_t type;
	uint8_t subtype;
	uint16_t length;
};


struct efi_loaded_image_protocol {
	uint32_t revision;
	efi_handle_t parent;
	struct efi_system_table *system;

	// Source location of the image
	efi_handle_t device;
	struct efi_device_path_protocol *filepath;
	void *reserved;

	// Image's load options
	uint32_t load_options_size;
	void *load_options;

	// Location of the image in memory
	void *image_base;
	uint64_t image_size;
	enum efi_memory_type image_code_type;
	enum efi_memory_type image_data_type;
	void (*unused)();
};

struct elf64_ehdr {
    unsigned char e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct elf64_phdr {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
};

struct loader {
	struct efi_system_table *system;
	efi_handle_t handle;
	struct efi_loaded_image_protocol *image;
	efi_handle_t root_device;
	struct efi_simple_file_system_protocol *rootfs;
	struct efi_file_protocol *rootdir;

	struct efi_file_protocol *config;
	const char *config_data;

	struct module *module;
	size_t module_capacity;
	size_t modules;
	size_t kernel;

	struct efi_file_protocol *kernel_image;
	struct elf64_ehdr kernel_header;
	struct elf64_phdr *program_headers;
	unsigned long long kernel_image_entry;

	struct reserve *reserve;
	size_t reserve_capacity;
	size_t reserves;
};

efi_status_t setup_loader(
	efi_handle_t handle,
	struct efi_system_table *system,
	struct loader *loader);

efi_status_t load_config(struct loader *loader, const uint16_t *config_path);

efi_status_t parse_config(struct loader *loader);

efi_status_t load_kernel(struct loader *loader);

efi_status_t load_modules(struct loader *loader);

efi_status_t start_kernel(struct loader *loader);

#endif
