#ifndef IO_KIMO_H
#define IO_KIMO_H

/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * All Rights Reserved
 *
 * Keyboard Input and MOuse controller
 */

#include "SDL.h"

typedef struct IO_KIMO  IO_KIMO;

#define IOBASE_KIMO     0xFFFFE0
#define IOMASK_KIMO     0xFFFFF0

#define IOBASE_TIMER    0xFFFF80
#define IOMASK_TIMER    0xFFFFF0


#define CCHM	0x1C	//move cursor home
#define CCBT	0x1D	//move cursor to bottom
#define CCLM	0x1E	//move cursor to left margin
#define CCRM	0x1F	//move cursor to right margin
#define BACK	0x08	//backspace
#define EOL		0x0A	//$9B	;end of line (RETURN
#define ESC		0x1B	//escape key
#define CCUP	0x1C	//cursor up
#define CCDN	0x1D	//cursor down
#define CCLF	0x1E	//cursor left
#define CCRT	0x1F	//cursor right
#define CSPACE	0x20	//space
#define CLS		0x01	//clear screen
#define TABU	0x7F	//tabulator
#define CILN	0x9D	//insert line
#define CDCH	0xFE	//delete character
#define CICH	0xFF	//insert character
#define HELP	0x11	//key code for HELP
#define CNTLF1	0x83	//key code for CTRL-F1
#define CNTLF2	0x84	//key code for CTRL-F2
#define CNTLF3	0x93	//key code for CTRL-F3
#define CNTLF4	0x94	//key code for CTRL-F4
#define CPGUP	0x95	//key code for page-up
#define CPGDN	0x96	//key code for page-down
#define CNTL1	0x9F	//key code for CTRL-1

#define RBR0 		0
#define THR0 		0
#define IER0 		1
#define IIR0 		2
#define LCR0 		3
#define MCR0 		4
#define LSR0 		5
#define MSR0 		6
#define SCR0 		7
#define DLL0 		0
#define DLM0 		1

#define	SECPTRLO	8
#define	SECPTRHI	9
#define	READWRITE	10
#define BUFPTRLO	11
#define BUFPTRHI	12

#define LED542		0
#define TIMERLO 	1
#define TIMERHI		2
#define TIMERST		3
#define MULINA0		4
#define MULINA1		5
#define MULINA2		6
#define MULINA3		7
#define MULINB0		8
#define MULINB1		9
#define MULINB2		10
#define MULINB3		11
#define MULOUT0		12
#define MULOUT1		13
#define MULOUT2		14
#define MULOUT3		15

struct IO_KIMO
{
    byte	kbda;
    byte	secptrlo;
    byte	secptrhi;
    byte	readwrite;
    byte	bufptrlo;
    byte	bufptrhi;
    word16	secptr;
    word16	bufptr;
    byte	timerstatus;
    word16	kbmost;

    byte	mula0;
    byte	mula1;
    byte	mula2;
    byte	mula3;
    byte	mulb0;
    byte	mulb1;
    byte	mulb2;
    byte	mulb3;
    byte	mulout0;
    byte	mulout1;
    byte	mulout2;
    byte	mulout3;

    /*
        Bit 15: Keycode received.
        Bit 14: 1=Key press; 0=Key release.
    */
};

int
kimo_initialize( void );

void
kimo_expunge( void );

byte
kimo_read( word32 address, word32 timestamp );
byte
timer_read(word32 address, word32 timestamp);

void
kimo_write( word32 address, byte b, word32 timestamp );

void
timer_write( word32 address, byte b, word32 timestamp );

void
kimo_update( word32 timestamp );

void
loadDiskImage( void );

int
kimo_timer(void *data);

#endif

