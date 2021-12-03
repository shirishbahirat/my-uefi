
#include "loader.h"

uint64_t UINT64_MAX_LOADER  = 0xFFFFFFFF;

static void debug_boot(
	struct efi_simple_text_output_protocol *out,
	const char *fmt,
	va_list args)
{
	uint16_t msg[512];

	vsnprintf(msg, sizeof(msg), fmt, args);
	out->output_string(out, msg);
}

void info_boot(struct efi_system_table *system, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	debug_boot(system->out, fmt, args);
	va_end(args);
}

void err_boot(struct efi_system_table *system, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	debug_boot(system->err, fmt, args);
	va_end(args);
}


void *memset(void *ptr, int value, size_t size)
{
	char *to = ptr;

	for (size_t i = 0; i < size; ++i)
		*to++ = value;
	return ptr;
}

void *memcpy(void *dst, const void *src, size_t size)
{
	const char *from = src;
	char *to = dst;

	for (size_t i = 0; i < size; ++i)
		*to++ = *from++;
	return dst;
}

efi_status_t efi_read_fixed(
	struct efi_system_table *system,
	struct efi_file_protocol *file,
	uint64_t offset,
	size_t size,
	void *dst)
{
	efi_status_t status = EFI_SUCCESS;
	unsigned char *buf = dst;
	size_t read = 0;

	status = file->set_position(file, offset);
	if (status != EFI_SUCCESS) {
		return status;
	}

	while (read < size) {
		efi_uint_t remains = size - read;

		status = file->read(file, &remains, (void *)(buf + read));
		if (status != EFI_SUCCESS) {
			return status;
		}

		read += remains;
	}

	return status;
}

static efi_status_t get_loader_image(
	efi_handle_t loader,
	struct efi_system_table *system,
	struct efi_loaded_image_protocol **image)
{
	struct efi_guid guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;

	return system->boot->open_protocol(
		loader,
		&guid,
		(void **)image,
		loader,
		NULL,
		EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
}

static efi_status_t get_rootfs(
	efi_handle_t loader,
	struct efi_system_table *system,
	efi_handle_t device,
	struct efi_simple_file_system_protocol **rootfs)
{
	struct efi_guid guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

	return system->boot->open_protocol(
		device,
		&guid,
		(void **)rootfs,
		loader,
		NULL,
		EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
}

static efi_status_t get_rootdir(
	struct efi_simple_file_system_protocol *rootfs,
	struct efi_file_protocol **rootdir)
{
	return rootfs->open_volume(rootfs, rootdir);
}

efi_status_t setup_loader(
	efi_handle_t handle,
	struct efi_system_table *system,
	struct loader *loader)
{
	efi_status_t status = EFI_SUCCESS;

	memset(loader, 0, sizeof(*loader));
	loader->system = system;
	loader->handle = handle;

	status = get_loader_image(handle, system, &loader->image);
	if (status != EFI_SUCCESS) {
		return status;
	}

	loader->root_device = loader->image->device;
	status = get_rootfs(
		handle, system, loader->root_device, &loader->rootfs);
	if (status != EFI_SUCCESS) {
		return status;
	}

	status = get_rootdir(loader->rootfs, &loader->rootdir);
	if (status != EFI_SUCCESS) {
		return status;
	}

	return EFI_SUCCESS;
}

efi_status_t load_config(
	struct loader *loader,
	const uint16_t *config_path)
{
	efi_status_t status = EFI_SUCCESS;
	struct efi_guid guid = EFI_FILE_INFO_GUID;
	struct efi_file_info file_info;
	efi_uint_t size;

	status = loader->rootdir->open(
		loader->rootdir,
		&loader->config,
		(uint16_t *)config_path,
		EFI_FILE_MODE_READ,
		EFI_FILE_READ_ONLY);
	if (status != EFI_SUCCESS) {
		return status;
	}

	size = sizeof(file_info);
	status = loader->config->get_info(
		loader->config,
		&guid,
		&size,
		(void *)&file_info);
	if (status != EFI_SUCCESS) {
		return status;
	}

	status = loader->system->boot->allocate_pool(
		EFI_LOADER_DATA,
		file_info.file_size + 1,
		(void **)&loader->config_data);
	if (status != EFI_SUCCESS) {
		return status;
	}

	memset((void *)loader->config_data, 0, file_info.file_size + 1);

	return efi_read_fixed(
		loader->system,
		loader->config,
		/* offset */0,
		/* size */file_info.file_size,
		(void *)loader->config_data);
}


static size_t skip_ws(const char *data, size_t i)
{
	while (isspace(data[i]))
		++i;
	return i;
}

static size_t skip_name(const char *data, size_t i)
{
	while (isalnum(data[i])
	       || data[i] == '_'
	       || data[i] == '-'
	       || data[i] == '.')
		++i;
	return i;
}

static size_t skip_path(const char *data, size_t i)
{
	while (isalnum(data[i])
	       || data[i] == '_'
	       || data[i] == '-'
	       || data[i] == '.'
	       || data[i] == '\\'
	       || data[i] == '/')
		++i;
	return i;
}

static efi_status_t add_module(
	struct loader *loader,
	const char *name,
	const uint16_t *path)
{
	if (loader->modules == loader->module_capacity) {
		efi_status_t status = EFI_SUCCESS;
		size_t new_size = 2 * loader->modules;
		struct module *new_module = NULL;
		struct module *old_module = loader->module;

		if (new_size == 0)
			new_size = 16;

		status = loader->system->boot->allocate_pool(
			EFI_LOADER_DATA,
			new_size * sizeof(struct module),
			(void **)&new_module);
		if (status != EFI_SUCCESS) {
			return status;
		}

		memcpy(
			new_module,
			old_module,
			loader->modules * sizeof(struct module));
		loader->module = new_module;
		loader->module_capacity = new_size;

		if (old_module != NULL) {
			status = loader->system->boot->free_pool(
				(void *)old_module);
			if (status != EFI_SUCCESS) {
				return status;
			}
		}
	}

	memset(&loader->module[loader->modules], 0, sizeof(struct module));
	loader->module[loader->modules].name = name;
	loader->module[loader->modules].path = path;
	++loader->modules;
	return EFI_SUCCESS;
}


efi_status_t parse_config(struct loader *loader)
{
	size_t i = 0;

	while (1) {
		efi_status_t status = EFI_SUCCESS;
		uint16_t *path = NULL;
		char *name = NULL;
		size_t name_begin, name_size;
		size_t path_begin, path_size;

		i = skip_ws(loader->config_data, i);
		if (loader->config_data[i] == '\0')
			break;

		name_begin = i;
		i = skip_name(loader->config_data, i);
		name_size = i - name_begin;
		i = skip_ws(loader->config_data, i);

		if (loader->config_data[i] != ':') {
			return EFI_INVALID_PARAMETER;
		}

		i = skip_ws(loader->config_data, i + 1);
		path_begin = i;
		i = skip_path(loader->config_data, i);
		path_size = i - path_begin;

		if (name_size == 0) {
			return EFI_INVALID_PARAMETER;
		}

		if (path_size == 0) {
			return EFI_INVALID_PARAMETER;
		}

		status = loader->system->boot->allocate_pool(
			EFI_LOADER_DATA,
			name_size + 1,
			(void **)&name);
		if (status != EFI_SUCCESS) {
			return status;
		}
		strncpy(name, &loader->config_data[name_begin], name_size);
		name[name_size] = '\0';

		status = loader->system->boot->allocate_pool(
			EFI_LOADER_DATA,
			2 * (path_size + 1),
			(void **)&path);
		if (status != EFI_SUCCESS) {
			return status;
		}
		to_u16strncpy(
			path, &loader->config_data[path_begin], path_size);
		path[path_size] = '\0';

		status = add_module(loader, name, path);
		if (status != EFI_SUCCESS) {
			return status;
		}
	}

	for (size_t j = 0; j < loader->modules; ++j) {
		if (strcmp(loader->module[j].name, "kernel") != 0)
			continue;

		loader->kernel = j;
		return EFI_SUCCESS;
	}

	return EFI_INVALID_PARAMETER;
}

static efi_status_t read_elf64_header(
	struct efi_system_table *system,
	struct efi_file_protocol *file,
	struct elf64_ehdr *hdr)
{
	return efi_read_fixed(system, file, /*offset*/0, sizeof(*hdr), hdr);
}

static efi_status_t verify_elf64_header(
	struct efi_system_table *system,
	const struct elf64_ehdr *hdr)
{
	if (hdr->e_ident[EI_MAG0] != 0x7f
		|| hdr->e_ident[EI_MAG1] != 'E'
		|| hdr->e_ident[EI_MAG2] != 'L'
		|| hdr->e_ident[EI_MAG3] != 'F') {
		return EFI_UNSUPPORTED;
	}

	if (hdr->e_type != ET_EXEC && hdr->e_type != ET_DYN) {
		return EFI_UNSUPPORTED;
	}

	if (hdr->e_ident[EI_CLASS] != ELFCLASS64) {
		return EFI_UNSUPPORTED;
	}

	if (hdr->e_phnum == 0) {
		return EFI_UNSUPPORTED;
	}

	if (hdr->e_phentsize != sizeof(struct elf64_phdr)) {
		return EFI_UNSUPPORTED;
	}

	return EFI_SUCCESS;
}


static efi_status_t read_elf64_program_headers(
	struct efi_system_table *system,
	struct efi_boot_table *boot,
	struct efi_file_protocol *file,
	const struct elf64_ehdr *hdr,
	struct elf64_phdr **phdrs)
{
	efi_status_t status;

	status = boot->allocate_pool(
		EFI_LOADER_DATA,
		hdr->e_phnum * hdr->e_phentsize,
		(void **)phdrs);
	if (status != EFI_SUCCESS) {
		return status;
	}

	status = efi_read_fixed(
		system,
		file,
		hdr->e_phoff,
		hdr->e_phentsize * hdr->e_phnum,
		(void *)*phdrs);
	if (status != EFI_SUCCESS) {
		boot->free_pool((void *)*phdrs);
		return status;
	}

	return EFI_SUCCESS;
}

static void elf64_image_size(
	struct loader *loader,
	uint64_t alignment,
	uint64_t *begin,
	uint64_t *end)
{
	*begin = UINT64_MAX_LOADER;
	*end = 0;

	for (size_t i = 0; i < loader->kernel_header.e_phnum; ++i) {
		struct elf64_phdr *phdr = &loader->program_headers[i];
		uint64_t phdr_begin, phdr_end;
		uint64_t align = alignment;

		if (phdr->p_type != PT_LOAD)
			continue;

		if (phdr->p_align > align)
			align = phdr->p_align;

		phdr_begin = phdr->p_vaddr;
		phdr_begin &= ~(align - 1);
		if (*begin > phdr_begin)
			*begin = phdr_begin;

		phdr_end = phdr->p_vaddr + phdr->p_memsz + align - 1;
		phdr_end &= ~(align - 1);
		if (*end < phdr_end)
			*end = phdr_end;
	}
}


static efi_status_t reserve(
	struct loader *loader,
	const char *name,
	uint64_t begin,
	uint64_t end)
{
	if (loader->reserves == loader->reserve_capacity) {
		efi_status_t status = EFI_SUCCESS;
		size_t new_size = 2 * loader->reserves;
		struct reserve *new_reserve = NULL;
		struct reserve *old_reserve = loader->reserve;

		if (new_size == 0)
			new_size = 16;

		status = loader->system->boot->allocate_pool(
			EFI_LOADER_DATA,
			new_size * sizeof(struct reserve),
			(void **)&new_reserve);
		if (status != EFI_SUCCESS) {
			return status;
		}

		memcpy(
			new_reserve,
			old_reserve,
			loader->reserves * sizeof(struct reserve));
		loader->reserve = new_reserve;
		loader->reserve_capacity = new_size;

		if (old_reserve != NULL) {
			status = loader->system->boot->free_pool(
				(void *)old_reserve);
			if (status != EFI_SUCCESS) {
				return status;
			}
		}
	}

	memset(&loader->reserve[loader->reserves], 0, sizeof(struct reserve));
	loader->reserve[loader->reserves].name = name;
	loader->reserve[loader->reserves].begin = begin;
	loader->reserve[loader->reserves].end = end;
	++loader->reserves;
	return EFI_SUCCESS;
}


efi_status_t load_kernel(struct loader *loader)
{
	efi_status_t status = EFI_SUCCESS;
	uint64_t page_size = 4096;
	uint64_t image_begin;
	uint64_t image_end;
	uint64_t image_size;
	uint64_t* image_addr = NULL;

	status = loader->rootdir->open(
		loader->rootdir,
		&loader->kernel_image,
		(uint16_t *)loader->module[loader->kernel].path,
		EFI_FILE_MODE_READ,
		EFI_FILE_READ_ONLY);
	if (status != EFI_SUCCESS) {
		return status;
	}

	status = read_elf64_header(
		loader->system, loader->kernel_image, &loader->kernel_header);
	if (status != EFI_SUCCESS) {
		return status;
	}

	status = verify_elf64_header(
		loader->system, &loader->kernel_header);
	if (status != EFI_SUCCESS) {
		return status;
	}

	status = read_elf64_program_headers(
		loader->system,
		loader->system->boot,
		loader->kernel_image,
		&loader->kernel_header,
		&loader->program_headers);
	if (status != EFI_SUCCESS) {
		return status;
	}

	elf64_image_size(
		loader,
		page_size,
		&image_begin,
		&image_end);
	image_size = image_end - image_begin;
	status = loader->system->boot->allocate_pages(
		EFI_ALLOCATE_ANY_PAGES,
		EFI_LOADER_DATA,
		image_size / page_size,
		image_addr);
	if (status != EFI_SUCCESS) {
		return status;
	}

	memset((void *)image_addr, 0, image_size);
	for (size_t i = 0; i < loader->kernel_header.e_phnum; ++i) {
		struct elf64_phdr *phdr = &loader->program_headers[i];
		uint64_t *phdr_addr = NULL;

		if (phdr->p_type != PT_LOAD)
			continue;

		*phdr_addr = *image_addr + phdr->p_vaddr - image_begin;
		status = efi_read_fixed(
			loader->system,
			loader->kernel_image,
			phdr->p_offset,
			phdr->p_filesz,
			(void*)phdr_addr);
		if (status != EFI_SUCCESS) {
			return status;
		}

		status = reserve(
			loader,
			"kernel",
			*phdr_addr,
			*phdr_addr + phdr->p_memsz);
		if (status != EFI_SUCCESS) {
			return status;
		}
	}

	loader->kernel_image_entry =
		*image_addr + loader->kernel_header.e_entry - image_begin;
	return EFI_SUCCESS;
}

static efi_status_t load_module(
	struct loader *loader,
	struct efi_file_protocol *file,
	const char *name)
{
	efi_status_t status = EFI_SUCCESS;
	struct efi_guid guid = EFI_FILE_INFO_GUID;
	void *addr = NULL;
	struct efi_file_info file_info;
	efi_uint_t size;

	size = sizeof(file_info);
	status = file->get_info(
		file,
		&guid,
		&size,
		(void *)&file_info);
	if (status != EFI_SUCCESS) {
		return status;
	}

	status = loader->system->boot->allocate_pool(
		EFI_LOADER_DATA,
		file_info.file_size,
		&addr);
	if (status != EFI_SUCCESS) {
		return status;
	}

	status = efi_read_fixed(
		loader->system,
		file,
		/* offset */0,
		/* size */file_info.file_size,
		addr);
	if (status != EFI_SUCCESS) {
		return status;
	}

	status = reserve(
		loader,
		name,
		(uint64_t)addr,
		(uint64_t)addr + file_info.file_size);
	if (status != EFI_SUCCESS) {
		return status;
	}
	return EFI_SUCCESS;
}


efi_status_t load_modules(struct loader *loader)
{
	for (size_t i = 0; i < loader->modules; ++i) {
		efi_status_t status = EFI_SUCCESS;
		struct efi_file_protocol *file = NULL;

		if (i == loader->kernel)
			continue;

		status = loader->rootdir->open(
			loader->rootdir,
			&file,
			(uint16_t *)loader->module[i].path,
			EFI_FILE_MODE_READ,
			EFI_FILE_READ_ONLY);
		if (status != EFI_SUCCESS) {
			return status;
		}

		status = load_module(loader, file, loader->module[i].name);
		if (status != EFI_SUCCESS) {
			return status;
		}

		status = file->close(file);
		if (status != EFI_SUCCESS) {
			return status;
		}
	}

	return EFI_SUCCESS;
}


static efi_status_t exit_efi_boot_services(struct loader *loader)
{
	struct efi_memory_descriptor *mmap;
	efi_uint_t mmap_size = 4096;
	efi_uint_t mmap_key;
	efi_uint_t desc_size;
	uint32_t desc_version;
	efi_status_t status;

	while (1) {
		status = loader->system->boot->allocate_pool(
			EFI_LOADER_DATA,
			mmap_size,
			(void **)&mmap);
		if (status != EFI_SUCCESS)
			return status;

		status = loader->system->boot->get_memory_map(
			&mmap_size,
			mmap,
			&mmap_key,
			&desc_size,
			&desc_version);
		if (status == EFI_SUCCESS)
			break;

		loader->system->boot->free_pool(mmap);

		if (status == EFI_BUFFER_TOO_SMALL) {
			mmap_size *= 2;
			continue;
		}

		return status;
	}

	status = loader->system->boot->exit_boot_services(
		loader->handle,
		mmap_key);
	if (status != EFI_SUCCESS)
		loader->system->boot->free_pool(mmap);

	return status;
}


efi_status_t start_kernel(struct loader *loader)
{
	efi_status_t status = EFI_SUCCESS;
	void (ELFABI *entry)(struct reserve *, size_t);

	info_boot(loader->system, "Shutting down UEFI boot services\r\n");
	status = exit_efi_boot_services(loader);
	if (status != EFI_SUCCESS)
		return status;

	entry = (void (ELFABI *)(struct reserve *, size_t))
		loader->kernel_image_entry;
	(*entry)(loader->reserve, loader->reserves);

	while (1) {}
	return EFI_LOAD_ERROR;
}


efi_status_t efi_main(efi_handle_t handle, struct efi_system_table* system)
{
	efi_status_t status;
	struct loader loader;

	uint16_t config_path[] = u"efi\\boot\\config.txt";

	info_boot(system, "Setting up the loader...\r\n");
	status = setup_loader(handle, system, &loader);
	if(EFI_ERROR(status))
		return status;

	info_boot(system, "Loading the config...\r\n");
	status = load_config(&loader, config_path);
	if (status != EFI_SUCCESS)
		return status;

	info_boot(system, "Parsing the config...\r\n");
	status = parse_config(&loader);
	if (status != EFI_SUCCESS)
		return status;

	info_boot(system, "Loading the kernel...\r\n");
	status = load_kernel(&loader);
	if (status != EFI_SUCCESS)
		return status;

	info_boot(system, "Loading the data...\r\n");
	status = load_modules(&loader);
	if (status != EFI_SUCCESS)
		return status;

	info_boot(system, "Starting the kernel...\r\n");
	status = start_kernel(&loader);
	if (status != EFI_SUCCESS)
		return status;

	return status;
}
