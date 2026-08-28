/*—————————————————————————————————————————————————————————————————————————————
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

#ifndef _MATH_H
#define _MATH_H

#define    CLAMP(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))
#define CLAMPMIN(val, min)      ((val) < (min) ? (min) :  (val))
#define CLAMPMAX(val, max)                               ((val) > (max) ? (max) : (val))

#define ABS(val) (((val) < 0) ? (-val) : (val))

#endif //_MATH_H
