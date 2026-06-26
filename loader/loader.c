/*
* Copyright (c) 2019-2024 CTCaer
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

#include "../loader/payload.h"

#include <memory_map.h>
#include <../bdk/libs/compr/nrv/nrv2e.h>
#include <soc/bpmp.h>
#include <soc/clock.h>
#include <soc/t210.h>

// 0x4003D000: Safe for panic preserving, 0x40038000: Safe for debugging needs.
#define IPL_RELOC_TOP        0x40038000
#define IPL_PATCHED_RELOC_SZ 0x94
#define IPL_VERSION_RCFG_OFF 0x120

const volatile ipl_ver_meta_t __attribute__((section ("._ipl_version"))) iplVer = {
	.magic             = TE_MAGIC,
	.version           = (TE_VER_MJ + '0') | ((TE_VER_MN + '0') << 8) | ((TE_VER_HF + '0') << 16),
	.rcfg.rsvd_flags   = 0,
	.rcfg.bclk_t210    = BPMP_CLK_LOWER_BOOST,
	.rcfg.bclk_t210b01 = BPMP_CLK_DEFAULT_BOOST
};

void loaderMain()
{
	// Preliminary BPMP clocks init.
	CLOCK(CLK_RST_CONTROLLER_CLK_SYSTEM_RATE)    = 0x10;       // Set HCLK div to 2 and PCLK div to 1.
	CLOCK(CLK_RST_CONTROLLER_CLK_SOURCE_SYS)     = 0;          // Set SCLK div to 1.
	CLOCK(CLK_RST_CONTROLLER_SCLK_BURST_POLICY)  = 0x20004444; // Set clk source to Run and PLLP_OUT2 (204MHz).
	CLOCK(CLK_RST_CONTROLLER_SUPER_SCLK_DIVIDER) = 0x80000000; // Enable SUPER_SDIV to 1.
	CLOCK(CLK_RST_CONTROLLER_CLK_SYSTEM_RATE)    = 2;          // Set HCLK div to 1 and PCLK div to 3.
	CLOCK(CLK_RST_CONTROLLER_SCLK_BURST_POLICY)  = 0x20003333; // Set SCLK to PLLP_OUT (408MHz).

	// Set arbiter.
	ARB_PRI(ARB_PRIO_CPU_PRIORITY) = 0x12412D1;
	ARB_PRI(ARB_PRIO_COP_PRIORITY) = 0x0000000;
	ARB_PRI(ARB_PRIO_VCP_PRIORITY) = 0x220244A;
	ARB_PRI(ARB_PRIO_DMA_PRIORITY) = 0x320369B;

	// Get Payload size.
	u32 payloadSize  = sizeof(payload);               // Actual payload size.
	payloadSize      = ALIGN(payloadSize, 4);        // Align size to 4 bytes.
	u32 *payloadAddr = (u32 *)payload;

	// Relocate payload to a safer place.
	u32 words = payloadSize >> 2;
	u32 *src  = payloadAddr + words - 1;
	u32 *dst  = (u32 *)(IPL_RELOC_TOP - 4);
	while (words)
	{
		*dst = *src;
		src--;
		dst--;
		words--;
	}

	// Set source address of the first part.
	u8 *srcAddr = (void *)(IPL_RELOC_TOP - payloadSize);

	// Uncompress.
	u32 outLen;
	nrv2e_decompress_8(srcAddr, sizeof(payload), (u8 *)IPL_LOAD_ADDR, &outLen);

	// Copy new reserved configuration.
	memcpy((u8 *)(IPL_LOAD_ADDR + IPL_VERSION_RCFG_OFF), (rsvd_cfg_t *)&iplVer.rcfg, sizeof(rsvd_cfg_t));

	// Chainload into uncompressed payload.
	void (*iplPtr)() = (void *)IPL_LOAD_ADDR;
	(*iplPtr)();

	// Halt if we managed to get out of execution.
	while (true)
		;
}