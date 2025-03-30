/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * All Rights Reserved
 */

#include <stdio.h>

//#include <SDL/SDL.h>

#include <lib65816/cpu.h>
#include "io.h"
#include "lib65816/cpu.h"

int io_initialize(void) {
	int success = 1;

	/*
	 success = mgia_initialize();
	 if( !success ) fprintf( stderr, "MGIA failed to initialize\n" );

	 success = kimo_initialize();
	 if( !success ) fprintf( stderr, "KIMO failed to initialize\n" );

	 success = timers_initialize();
	 if( !success ) fprintf( stderr, "TIMERS failed to initialize\n" );

	 

	success = mouse_initialize();
	
	if (!success)
		fprintf( stderr, "MOUSE failed to initialize\n");
	*/
	return success;
}

void io_expunge(void) {
	//mgia_expunge();
}

byte io_read(word32 address, word32 timestamp) {
	//word32 adr_palette = address & IOMASK_PALETTE;

	/*
	 if( ( address & IOMASK_MGIA ) == IOBASE_MGIA )
	 return mgia_read( address, timestamp );

	 if( ( address & IOMASK_IRQC ) == IOBASE_IRQC )
	 return irqc_read( address, timestamp );
	 */
	/*
	if ((address & IOMASK_MOUSE) == IOBASE_MOUSE)
		return mouse_read(address, timestamp);
	else if ((address & IOMASK_KIMO) == IOBASE_KIMO)
		return kimo_read(address, timestamp);
	else if ((address & IOMASK_DISPLAY) == IOBASE_DISPLAY)
		return display_read(address, timestamp);
	else if ((address & IOMASK_TIMER) == IOBASE_TIMER)
		return timer_read(address, timestamp);
	else if (adr_palette == IOBASE_PALETTE0 || adr_palette == IOBASE_PALETTE1)
		return display_read_palette(address, timestamp);
	*/	
	/*
	 if( ( address & IOMASK_TIMERS ) == IOBASE_TIMERS )
	 return timers_read( address, timestamp );

	 if( ( address & IOMASK_SERBUS ) == IOBASE_SERBUS )
	 return serbus_read( address, timestamp );
	 */
	return 0x00;
}

void io_write(word32 address, byte b, word32 timestamp) {
	/*
	word32 adr_palette = address & IOMASK_PALETTE;

	if ((address & IOMASK_MOUSE) == IOBASE_MOUSE)
		mouse_write(address, b, timestamp);
	else if ((address & IOMASK_KIMO) == IOBASE_KIMO)
		kimo_write(address, b, timestamp);
	else if ((address & IOMASK_DISPLAY) == IOBASE_DISPLAY)
		display_write(address, b, timestamp);
	else if ((address & IOMASK_TIMER) == IOBASE_TIMER)
		timer_write(address, b, timestamp);
	else if (adr_palette == IOBASE_PALETTE0 || adr_palette == IOBASE_PALETTE1) {
		display_write_palette(address, b, timestamp);
	}
	*/
	/*
	 else if( ( address & IOMASK_TIMERS ) == IOBASE_TIMERS )
	 timers_write( address, b, timestamp );

	 else if( ( address & IOMASK_SERBUS ) == IOBASE_SERBUS )
	 serbus_write( address, b, timestamp );
	 */
}

/*
 static void
 fps_update( void )
 {
 extern word32 frames;
 static word32 reference = 0;
 word32 now;

 if( reference == 0 )
 reference = SDL_GetTicks();

 now = SDL_GetTicks();
 if( ( now - reference ) > 1000 )
 {
 fprintf( stderr, "\r  FPS: %ld   ", frames );
 fprintf( stderr, "CPU Performance: %ld%%   ", (10*frames)/6);

 frames = 0; reference = now;
 }
 }
 */

//void EMUL_hardwareUpdate( word32 timestamp )
//{
//    fps_update();
//    mgia_update( timestamp );
//    kimo_update( timestamp );
//    irqc_update( timestamp );   /* update this last, to make sure we get all IRQs */
//}
