#ifndef K2_IO_H
#define K2_IO_H

/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * See LICENSE for details.
 */

#include <lib65816/cpu.h>

//#include "mgia.h"
//#include "irqc.h"
//#include "kimo.h"
//#include "timers.h"
//#include "serbus.h"


/* The HIBYTE/LOBYTE macros are endian-dependent.  Adjust for your local
 * platform.
 */

#define BYTEPT(x) ((byte *)(&(x)))

#define LOBYTE(x) (*(BYTEPT(x)+0))
#define HIBYTE(x) (*(BYTEPT(x)+1))
#define LOBANK(x) (*(BYTEPT(x)+2))



int
io_initialize( void );

void
io_expunge( void );

byte
io_read( word32 address, word32 timestamp );

void
io_write( word32 address, byte b, word32 timestamp );

#endif

