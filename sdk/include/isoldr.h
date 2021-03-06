/** 
 * \file    isoldr.h
 * \brief   DreamShell ISO loader
 * \date    2009-2014
 * \author  SWAT www.dc-swat.ru
 */

#ifndef _DS_ISOLDR_H_
#define _DS_ISOLDR_H_

#include <arch/types.h>
#include <dc/cdrom.h>
#include "isofs/isofs.h"
#include "isofs/ciso.h"


/**
 * Loader params size
 */
#define ISOLDR_PARAMS_SIZE 1024


/**
 * Maximum memory usage by loader and params
 */
#define ISOLDR_MAX_MEM_USAGE 32768


/**
 * It's a default loader addresses
 * 
 * 0x8c004000 - Can't be used with dcl loader
 *
 * You can find more suitable by own experience.
 */
#define ISOLDR_DEFAULT_ADDR      0x8ce00000
#define ISOLDR_DEFAULT_ADDR_LOW  0x8c004000
#define ISOLDR_DEFAULT_ADDR_HIGH 0x8cfe8000


/**
 * Supported devices
 */
#define ISOLDR_DEV_GDROM  "cd"
#define ISOLDR_DEV_SDCARD "sd"
#define ISOLDR_DEV_G1ATA  "ide"
#define ISOLDR_DEV_DCLOAD "dcl"
#define ISOLDR_DEV_DCIO   "dcio"


/**
 * Supported file systems
 */
#define ISOLDR_FS_FAT     "fat"
#define ISOLDR_FS_EXT2    "ext2"    /* EXT2 doesn't supported yet, just define for now */
#define ISOLDR_FS_ISO9660 "iso9660"
#define ISOLDR_FS_DCLOAD  "dc-load"


/**
 * Boot mode
 */
typedef enum isoldr_boot_mode {
	BOOT_MODE_DIRECT = 0,
	BOOT_MODE_IPBIN = 1,  /* Bootstrap 1 */
	BOOT_MODE_IPBIN_TRUNC /* Bootstrap 2 */
} isoldr_boot_mode_t;


/**
 * Executable types
 */
typedef enum isoldr_exec_type {
	BIN_TYPE_AUTO = 0,
	BIN_TYPE_KOS = 1,
	BIN_TYPE_KATANA,
	BIN_TYPE_WINCE     /* Not supported yet */
} isoldr_exec_type_t;


/**
 * Executable info
 */
typedef struct isoldr_exec_info {
	
	uint32 lba;               /* File LBA */
	uint32 size;              /* Size in bytes */
	uint32 addr;              /* Memory address */
	char   file[16];          /* File name */
	int    type;              /* See isoldr_exec_type_t */
	
} isoldr_exec_info_t;


typedef struct isoldr_info {

	char magic[12];			           /* isoldr magic code - 'DSISOLDR050' */
	
	int  image_type;                    /* See isofs_image_type_t */
	char image_file[256];               /* Full path to image */
	char image_second[12];              /* Second data track file for the multitrack GDI image */
	
	char fs_dev[8];                     /* Device name, see supported devices */
	char fs_type[8];                    /* File system type (fat or ext2), only for SD and IDE devices */
	int  fs_part;                       /* Partition on device (0-3), only for SD and IDE devices */
	
	CISO_header_t ciso;                 /* CISO header for CSO/ZSO images */
	CDROM_TOC toc;                      /* Table of content */
	uint32 track_offset;                /* Data track offset, for the CDI images only */
	uint32 track_lba[2];                /* Data track LBA, second value for the multitrack GDI image */
	uint32 sector_size;                 /* Data track sector size */
	
	int boot_mode;                      /* See isoldr_boot_mode_t */
	int emu_cdda;                       /* Emulate CDDA audio */
	int emu_async;                      /* Emulate async data transfer */
	int use_dma;                        /* Use DMA data transfer for G1-bus devices (GD drive and IDE) */
	
	isoldr_exec_info_t exec;            /* Executable info */
	uint32 gdtex;                       /* Memory address for GD texture */
	
	uint32 cdda_offset[55];             /* CDDA tracks offset, only for CDI images */

} isoldr_info_t;


/**
 * Get some info from CD image and fill info structure
 */
isoldr_info_t *isoldr_get_info(const char *file, int use_gdtex);

/**
 * Execute loader for specified device at any valid memory address
 */
void isoldr_exec(isoldr_info_t *info, uint32 addr);

/**
 * Send the data to DCIO board and reboot DC
 */
void isoldr_exec_dcio(isoldr_info_t *info, const char *file);


#endif /* ifndef _DS_ISOLDR_H_*/
