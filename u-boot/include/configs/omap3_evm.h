/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration settings for the TI OMAP3 EVM board.
 *
 * Copyright (C) 2006-2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Author :
 *	Manikandan Pillai <mani.pillai@ti.com>
 * Derived from Beagle Board and 3430 SDP code by
 *	Richard Woodruff <r-woodruff2@ti.com>
 *	Syed Mohammed Khasim <khasim@ti.com>
 *
 * Manikandan Pillai <mani.pillai@ti.com>
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <configs/ti_omap3_common.h>

/*
 * We are only ever GP parts and will utilize all of the "downloaded image"
 * area in SRAM which starts at 0x40200000 and ends at 0x4020FFFF (64KB).
 */

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG

/* NAND */
#if defined(CONFIG_MTD_RAW_NAND)
#define CONFIG_SYS_FLASH_BASE		NAND_BASE
#define CONFIG_SYS_MAX_NAND_DEVICE      1
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_PAGE_COUNT      64
#define CONFIG_SYS_NAND_PAGE_SIZE       2048
#define CONFIG_SYS_NAND_OOBSIZE         64
#define CONFIG_SYS_NAND_BLOCK_SIZE      (128*1024)
#define CONFIG_SYS_NAND_BAD_BLOCK_POS   NAND_LARGE_BADBLOCK_POS
#define CONFIG_SYS_NAND_ECCPOS          {2, 3, 4, 5, 6, 7, 8, 9,\
                                         10, 11, 12, 13}
#define CONFIG_SYS_NAND_ECCSIZE         512
#define CONFIG_SYS_NAND_ECCBYTES        3
#define CONFIG_NAND_OMAP_ECCSCHEME      OMAP_ECC_BCH8_CODE_HW_DETECTION_SW
#define CONFIG_SYS_ENV_SECT_SIZE        SZ_128K
/* NAND: SPL falcon mode configs */
#if defined(CONFIG_SPL_OS_BOOT)
#define CONFIG_SYS_NAND_SPL_KERNEL_OFFS 0x2a0000
#endif /* CONFIG_SPL_OS_BOOT */
#endif /* CONFIG_MTD_RAW_NAND */

#define BOOTENV_DEV_LEGACY_MMC(devtypeu, devtypel, instance) \
	"bootcmd_" #devtypel #instance "=" \
	"setenv mmcdev " #instance "; " \
	"run mmcboot\0"
#define BOOTENV_DEV_NAME_LEGACY_MMC(devtypeu, devtypel, instance) \
	#devtypel #instance " "

#if defined(CONFIG_MTD_RAW_NAND)

#define BOOTENV_DEV_NAND(devtypeu, devtypel, instance) \
	"bootcmd_" #devtypel #instance "=" \
	"if test ${mtdids} = '' || test ${mtdparts} = '' ; then " \
		"echo NAND boot disabled: No mtdids and/or mtdparts; " \
	"else " \
		"run nandboot; " \
	"fi\0"
#define BOOTENV_DEV_NAME_NAND(devtypeu, devtypel, instance) \
	#devtypel #instance " "

#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(LEGACY_MMC, legacy_mmc, 0) \
	func(UBIFS, ubifs, 0) \
	func(NAND, nand, 0)

#else /* !CONFIG_MTD_RAW_NAND */

#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(LEGACY_MMC, legacy_mmc, 0)

#endif /* CONFIG_MTD_RAW_NAND */

#include <config_distro_bootcmd.h>

#include <environment/ti/mmc.h>

#define CONFIG_EXTRA_ENV_SETTINGS \
	DEFAULT_LINUX_BOOT_ENV \
	DEFAULT_MMC_TI_ARGS \
	DEFAULT_FIT_TI_ARGS \
	"fdtfile=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"mtdids=" CONFIG_MTDIDS_DEFAULT "\0" \
	"mtdparts=" CONFIG_MTDPARTS_DEFAULT "\0" \
	"fdt_high=0xffffffff\0" \
	"console=ttyO0,115200n8\0" \
	"bootdir=/boot\0" \
	"bootenv=uEnv.txt\0" \
	"bootfile=zImage\0" \
	"bootpart=0:2\0" \
	"bootubivol=rootfs\0" \
	"bootubipart=rootfs\0" \
	"optargs=\0" \
	"nandroot=ubi0:rootfs ubi.mtd=rootfs rw noinitrd\0" \
	"nandrootfstype=ubifs rootwait\0" \
	"nandargs=setenv bootargs console=${console} " \
		"${mtdparts} " \
		"${optargs} " \
		"root=${nandroot} " \
		"rootfstype=${nandrootfstype}\0" \
	"nandboot=if nand read ${loadaddr} kernel && nand read ${fdtaddr} dtb; then " \
			"echo Booting uImage from NAND MTD 'kernel' partition ...; " \
			"run nandargs; " \
			"bootm ${loadaddr} - ${fdtaddr}; " \
		"fi\0" \
	BOOTENV

#endif /* __CONFIG_H */
