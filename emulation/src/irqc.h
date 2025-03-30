#ifndef K2_IRQC_H
#define K2_IRQC_H

/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * See LICENSE for details.
 */

#include <lib65816/cpu.h>

typedef struct IO_IRQC  IO_IRQC;

struct IO_IRQC
{
    word16      iqea;   /* IrQ EnAble */
    word16      iqpn;   /* IrQ PeNding */
};

#define IOBASE_IRQC     0xFF0100
#define IOMASK_IRQC     0xFFFF00


/* I wish there was a way to keep this synchronized between the Kestrel's
 * firmware, and the emulator core.
 */
#define IQVRLI          0x0001
#define IQKIMO          0x0002

int
irqc_initialize( void );

void
irqc_expunge( void );

byte
irqc_read( word32 address, word32 timestamp );

void
irqc_write( word32 address, byte b, word32 timestamp );

void
irqc_update( word32 timestamp );

void
irqc_raster( void );

void
irqc_kimo( void );

#endif

