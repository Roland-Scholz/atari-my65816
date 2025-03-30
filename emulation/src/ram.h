#ifndef K2_RAM_H
#define K2_RAM_H

/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * See LICENSE for details.
 */

#include <lib65816/cpu.h>

int
ram_initialize( void );

void
ram_expunge( void );

byte
ram_read( word32 address, word32 unusedTimestamp );

void
ram_write( word32 address, byte b, word32 timestamp );

#endif

