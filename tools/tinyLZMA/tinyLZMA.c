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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "TinyLZMACompress.h"

char filename[1024];

int main(int argc, char *argv[])
{
    int filename_len;
    struct stat statbuf;
    FILE *in_file, *out_file;

    if(stat(argv[1], &statbuf))
        goto error;

    if((in_file=fopen(argv[1], "rb")) == NULL)
        goto error;

    strcpy(filename, argv[1]);
    filename_len = strlen(filename);

    const uint32_t in_size  = statbuf.st_size;
    const uint32_t out_size = statbuf.st_size + 257;
          uint8_t *in_buf   = (uint8_t *)malloc(in_size);
          uint8_t *out_buf  = (uint8_t *)malloc(out_size);


    if(!(in_buf && out_buf))
        goto error;

    if(fread(in_buf, 1, in_size, in_file) != in_size)
        goto error;

    fclose(in_file);

    size_t nbytes;
    strcpy(filename + filename_len, ".lzma");

    tinyLzmaCompress(in_buf, in_size, out_buf, &nbytes);

    if (nbytes > out_size)
        goto error;

    if((out_file = fopen(filename,"wb")) == NULL)
        goto error;

    if (fwrite(out_buf, 1, nbytes, out_file) != nbytes)
        goto error;

    fclose(out_file);

    return 0;

    error:
    fprintf(stderr, "Failed to compress: %s\n", argv[1]);
    exit(1);
}