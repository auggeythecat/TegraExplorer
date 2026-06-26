/*
 * Copyright (c) 2019-2022 CTCaer
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdlib.h>

#include <bdk.h>

#include "emummc.h"
#include "../util/config.h"
#include <libs/fatfs/ff.h>

emummcCFG_t emuCFG = { 0 };

void emummcLoadCFG() {
	emuCFG.enabled = 0;
	emuCFG.path = NULL;
	emuCFG.sector = 0;
	emuCFG.id = 0;
	emuCFG.fileBasedPartSize = 0;
	emuCFG.activePart = 0;
	emuCFG.fsVer = 0;
	if (!emuCFG.nintendoPath)
		emuCFG.nintendoPath = (char *)malloc(0x200);
	if (!emuCFG.emummcFileBasedPath)
		emuCFG.emummcFileBasedPath = (char *)malloc(0x200);

	emuCFG.nintendoPath[0] = 0;
	emuCFG.emummcFileBasedPath[0] = 0;

	LIST_INIT(ini_sections);
	if (!ini_parse(&ini_sections, "emuMMC/emummc.ini", false)) {
		LIST_FOREACH_ENTRY(ini_sec_t, ini_sec, &ini_sections, link) {
			if (ini_sec->type == INI_CHOICE) {
				if (strcmp(ini_sec->name, "emummc"))
					continue;

				LIST_FOREACH_ENTRY(ini_kv_t, kv, &ini_sec->kvs, link) {
					if      (!strcmp("enabled",       kv->key))
						emuCFG.enabled = atoi(kv->val);
					else if (!strcmp("sector",        kv->key))
						emuCFG.sector  = strtol(kv->val, NULL, 16);
					else if (!strcmp("id",            kv->key))
						emuCFG.id      = strtol(kv->val, NULL, 16);
					else if (!strcmp("path",          kv->key))
						emuCFG.path   = kv->val;
					else if (!strcmp("nintendo_path", kv->key))
						strcpy(emuCFG.nintendoPath, kv->val);
				}
				break;
			}
		}
	}
}

bool emummcSetPath(char *path) {
	FIL fp;
	bool found = false;

	strcpy(emuCFG.emummcFileBasedPath, path);
	strcat(emuCFG.emummcFileBasedPath, "/raw_based");

	if (!f_open(&fp, emuCFG.emummcFileBasedPath, FA_READ)) {
		if (!f_read(&fp, &emuCFG.sector, 4, NULL))
			if (emuCFG.sector)
				found = true;
	} else {
		strcpy(emuCFG.emummcFileBasedPath, path);
		strcat(emuCFG.emummcFileBasedPath, "/file_based");

		if (!f_stat(emuCFG.emummcFileBasedPath, NULL)) {
			emuCFG.sector = 0;
			emuCFG.path = path;

			found = true;
		}
	}

	if (found) {
		emuCFG.enabled = 1;

		// Get ID from path.
		u32 id_from_path = 0;
		u32 path_size = strlen(path);
		if (path_size >= 4)
			memcpy(&id_from_path, path + path_size - 4, 4);
		emuCFG.id = id_from_path;

		strcpy(emuCFG.nintendoPath, path);
		strcat(emuCFG.nintendoPath, "/Nintendo");
	}

	return found;
}

static int _emummcRawGetPartOff(int partIdx) {
	switch (partIdx) {
	case 0:
		return 2;
	case 1:
		return 0;
	case 2:
		return 1;
	}
	return 2;
}

int emummcStorageInitMMC() {
	FILINFO fno;
	emuCFG.activePart = 0;

	// Always init eMMC even when in emuMMC. eMMC is needed from the emuMMC driver anyway.
	if (emmc_initialize(false))
		return 2;

	if (!emuCFG.enabled || TEConfig.emummcForceDisable)
		return 0;

	if (sd_mount())
		goto out;

	if (!emuCFG.sector) {
		strcpy(emuCFG.emummcFileBasedPath, emuCFG.path);
		strcat(emuCFG.emummcFileBasedPath, "/eMMC");

		if (f_stat(emuCFG.emummcFileBasedPath, &fno)) {
			EPRINTF("Failed to open eMMC folder.");
			goto out;
		}
		f_chmod(emuCFG.emummcFileBasedPath, AM_ARC, AM_ARC);

		strcat(emuCFG.emummcFileBasedPath, "/00");
		if (f_stat(emuCFG.emummcFileBasedPath, &fno)) {
			EPRINTF("Failed to open emuMMC rawnand.");
			goto out;
		}
		emuCFG.fileBasedPartSize = fno.fsize >> 9;
	}

	return 0;

out:
	return 1;
}

int emummcStorageEnd() {
	if (!emuCFG.enabled || TEConfig.emummcForceDisable)
		emmc_end();
	else
		sd_end();

	return 0;
}

int emummcStorageRead(u32 sector, u32 numSectors, void *buf) {
	FIL fp;
	if (!emuCFG.enabled || TEConfig.emummcForceDisable)
		return sdmmc_storage_read(&emmc_storage, sector, numSectors, buf);
	else if (emuCFG.sector)	{
		sector += emuCFG.sector;
		sector += _emummcRawGetPartOff(emuCFG.activePart) * 0x2000;
		return sdmmc_storage_read(&sd_storage, sector, numSectors, buf);
	} else {
		if (!emuCFG.activePart) {
			u32 file_part = sector / emuCFG.fileBasedPartSize;
			sector = sector % emuCFG.fileBasedPartSize;
			if (file_part >= 10)
				itoa(file_part, emuCFG.emummcFileBasedPath + strlen(emuCFG.emummcFileBasedPath) - 2, 10);
			else {
				emuCFG.emummcFileBasedPath[strlen(emuCFG.emummcFileBasedPath) - 2] = '0';
				itoa(file_part, emuCFG.emummcFileBasedPath + strlen(emuCFG.emummcFileBasedPath) - 1, 10);
			}
		}
		if (f_open(&fp, emuCFG.emummcFileBasedPath, FA_READ)) {
			EPRINTF("Failed to open emuMMC image.");
			return 1;
		}
		f_lseek(&fp, (u64)sector << 9);
		if (f_read(&fp, buf, (u64)numSectors << 9, NULL)) {
			EPRINTF("Failed to read emuMMC image.");
			f_close(&fp);
			return 1;
		}

		f_close(&fp);
		return 0;
	}
}

int emummcStorageWrite(u32 sector, u32 numSectors, void *buf) {
	FIL fp;
	if (!emuCFG.enabled || TEConfig.emummcForceDisable)
		return sdmmc_storage_write(&emmc_storage, sector, numSectors, buf);
	else if (emuCFG.sector)	{
		sector += emuCFG.sector;
		sector += _emummcRawGetPartOff(emuCFG.activePart) * 0x2000;
		return sdmmc_storage_write(&sd_storage, sector, numSectors, buf);
	} else {
		if (!emuCFG.activePart) {
			u32 file_part = sector / emuCFG.fileBasedPartSize;
			sector = sector % emuCFG.fileBasedPartSize;
			if (file_part >= 10)
				itoa(file_part, emuCFG.emummcFileBasedPath + strlen(emuCFG.emummcFileBasedPath) - 2, 10);
			else {
				emuCFG.emummcFileBasedPath[strlen(emuCFG.emummcFileBasedPath) - 2] = '0';
				itoa(file_part, emuCFG.emummcFileBasedPath + strlen(emuCFG.emummcFileBasedPath) - 1, 10);
			}
		}

		if (f_open(&fp, emuCFG.emummcFileBasedPath, FA_WRITE))
			return 1;

		f_lseek(&fp, (u64)sector << 9);
		if (f_write(&fp, buf, (u64)numSectors << 9, NULL)) {
			f_close(&fp);
			return 1;
		}

		f_close(&fp);
		return 0;
	}
}

int emummcStorageSetMMCPartition(u32 partition) {
	emuCFG.activePart = partition;
	emmc_set_partition(partition);

	if (!emuCFG.enabled || TEConfig.emummcForceDisable || emuCFG.sector)
		return 0;
	else {
		strcpy(emuCFG.emummcFileBasedPath, emuCFG.path);
		strcat(emuCFG.emummcFileBasedPath, "/eMMC");

		switch (partition) {
		case 0:
			strcat(emuCFG.emummcFileBasedPath, "/00");
			break;
		case 1:
			strcat(emuCFG.emummcFileBasedPath, "/BOOT0");
			break;
		case 2:
			strcat(emuCFG.emummcFileBasedPath, "/BOOT1");
			break;
		}

		return 0;
	}
}