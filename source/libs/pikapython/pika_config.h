//
// Created by auggiethecat on 5/6/26.
//

#ifndef _PIKA_CONFIG_H
#define _PIKA_CONFIG_H

#include <bdk.h>

#define PIKA_OPTIMIZE PIKA_OPTIMIZE_SPEED
#define PIKA_FLOAT_TYPE_DOUBLE            1
#define PIKA_NANO_ENABLE                  1
#define PIKA_SYNTAX_LEVEL_MAXIMAL         1
#define PIKA_BYTECODE_ONLY_ENABLE         1
#define PIKA_PRINT_LLD_ENABLE             0

#define PIKA_SHELL_SAVE_FILE_ENABLE       0
#define PIKA_SHELL_SAVE_BYTECODE_ENABLE   1
# define PIKA_SHELL_SAVE_BYTECODE_PATH    "tegraexplorer.py.o"
#define PIKA_SHELL_SAVE_APP_ENABLE        1
# define PIKA_SHELL_SAVE_APP_PATH         "tegraexplorer.pika"

#endif //_PIKA_CONFIG_H