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

#include "math.h"

u32 log10Approximation(u32 n) {
   // (n >= 1000000000) ? 9 :
   //  (n >= 100000000) ? 8 :
   //   (n >= 10000000) ? 7 :
   //    (n >= 1000000) ? 6 :
   //     (n >= 100000) ? 5 :
   //      (n >= 10000) ? 4 :

    // Currently, this is only being used where
    // the max input would be 1024. If needed,
    // the other conditions can be added back.
    return (n >= 1000) ? 3 :
            (n >= 100) ? 2 :
             (n >= 10) ? 1 :
                         0 ;
}