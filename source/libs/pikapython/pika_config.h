//
// Created by auggiethecat on 5/6/26.
//

#ifndef _PIKA_CONFIG_H
#define _PIKA_CONFIG_H

#include <bdk.h>

#define PIKA_OPTIMIZE PIKA_OPTIMIZE_SIZE // Saves ~700 bytes of binary
#define PIKA_FLOAT_TYPE_DOUBLE 1         // Disable doubles in favor of standard 4 byte floats.
#define PIKA_NANO_ENABLE       1

#endif //_PIKA_CONFIG_H