#ifndef __LOADER_H__
#define __LOADER_H__


#include <stddef.h>
#include <stdint.h>

#include "efi.h"
#include "log.h"

typedef void *efi_handle_t;
typedef uint64_t efi_status_t;
typedef uint64_t efi_uint_t;

struct module {
	const uint16_t *path;
	const char *name;
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
	uint64_t kernel_image_entry;

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
