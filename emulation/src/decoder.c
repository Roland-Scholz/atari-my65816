/*
 * decoder.c
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * See LICENSE for details.
 */

#include <lib65816/cpu.h>

#include "decoder.h"
#include "ram.h"
#include "rom.h"
#include "io.h"

/*
 * Kestrel 2 places its on-board I/O space in the $FF0000-$FFFFFF range.
 * A future enhancement would be to allow the I/O space to occupy either
 * bank $FF or $7F, depending on a control bit setting.
 */

typedef byte rfunc( word32, word32 );
typedef void wfunc( word32, byte, word32 );

rfunc *rmap[65536];
wfunc *wmap[65536];

/*
    static void
    unmapped_write( word32 unusedAddr, byte unusedByte, word32 timestamp )
    {
    }

    static byte
    unmapped_read( word32 unusedAddr, word32 timestamp )
    {
        return 0xFF;
    }
*/

void
MEM_initialize( void )
{
/*
    int i;

    for( i = 0; i < 0xFF00; i++ )
    {
        rmap[i] = &ram_read;
        wmap[i] = &ram_write;
    }

    for( i = 0x00E0; i <= 0x00FF; i++ )
        rmap[i] = &rom_read;

    for( i = 0xFF00; i <= 0xFFFF; i++ )
    {
        rmap[i] = &unmapped_read;
        wmap[i] = &unmapped_write;
    }

    rmap[ 0xFF00 ] = mgia_read;
    wmap[ 0xFF00 ] = mgia_write;

    rmap[ 0xFF01 ] = irqc_read;
    wmap[ 0xFF01 ] = irqc_write;

    rmap[ 0xFF02 ] = kimo_read;
    wmap[ 0xFF02 ] = kimo_write;
 */
}

#define IS_IO(a)  (((a) & 0xFFF800)==0x00D800)
#define IS_ROM(a) (((a) & 0xFFE000)==0x00E000)

byte
MEM_readMem( word32 address, word32 timestamp, word32 ignored )
{
    address &= 0x00FFFFFF;

    if( IS_IO( address ) )          return io_read( address, timestamp );
    else if( IS_ROM( address ) )    return rom_read( address, timestamp );
    else                            return ram_read( address, timestamp );
}

void
MEM_writeMem( word32 address, byte v, word32 timestamp )
{
    address &= 0x00FFFFFF;

    if( IS_IO( address ) )  io_write( address, v, timestamp );
    else                    ram_write( address, v, timestamp );
}

