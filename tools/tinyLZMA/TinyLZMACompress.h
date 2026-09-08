/*—————————————————————————————————————————————————————————————————————————————
 — Copyright (c) 2025 WangXuan95!                                             —
 — Copyright (c) 2025 jbro885!                                                —
 — Copyright (c) 2026 auggiethecat!                                           —
 —                                                                            —
 — All code was handwritten without the assistance of a Large Language Model. —
 —                                                                            —
 — This program is free software; you can redistribute it and/or modify it    —
 — under the terms and conditions of the GNU General Public License,          —
 — version 2, as published by the Free Software Foundation.                   —
 —                                                                            —
 — This program is distributed in the hope it will be useful, but WITHOUT     —
 — ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      —
 — FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for  —
 — more details.                                                              —
 —                                                                            —
 — You should have received a copy of the GNU General Public License          —
 — along with this program.  If not, see <http://www.gnu.org/licenses/>.      —
 —————————————————————————————————————————————————————————————————————————————*/

// Derived from TinyLZMA by jbro888, which was derived from TinyZZZ by WangXuan95
// https://github.com/jbro885/TinyLZMA- https://github.com/WangXuan95/TinyZZZ

#ifndef _TINYLZMACOMPRESS_H
#define _TINYLZMACOMPRESS_H

#include <stdint.h>
#include <stddef.h>

#define   R_OK                           0
#define   R_ERR_MEMORY_RUNOUT            1
#define   R_ERR_UNSUPPORTED              2
#define   R_ERR_OUTPUT_OVERFLOW          3

int tinyLzmaCompress(const uint8_t *source, size_t sourceLength, uint8_t *dest, size_t *destLength);

#endif //_TINYLZMACOMPRESS_H