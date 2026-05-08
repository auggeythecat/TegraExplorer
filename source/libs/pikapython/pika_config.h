//
// Created by auggiethecat on 5/6/26.
//

#ifndef _PIKA_CONFIG_H
#define _PIKA_CONFIG_H

#include <bdk.h>

#define PIKA_OPTIMIZE PIKA_OPTIMIZE_SIZE // Saves ~700 bytes of binary
#define PIKA_FLOAT_TYPE_DOUBLE 0         // Disable doubles in favor of standard 4 byte floats. adds 480 bytes to binary?

#define PIKA_BOOL bool
#define PIKA_FALSE false
#define PIKA_TRUE true

#endif //_PIKA_CONFIG_H