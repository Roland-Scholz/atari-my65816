#ifndef K2_ROM_H
#define K2_ROM_H

/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * See LICENSE for details.
 */

#include <lib65816/cpu.h>

int
rom_initialize( void );

void
rom_expunge( void );

byte
rom_read( word32 address, word32 unusedTimestamp );

void
rom_write( word32 address, byte b, word32 timestamp );

#endif

