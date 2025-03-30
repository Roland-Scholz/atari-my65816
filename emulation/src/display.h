#ifndef IO_DISPLAY_H
#define IO_DISPLAY_H

/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * All Rights Reserved
 *
 * Keyboard Input and MOuse controller
 */

#include <lib65816/cpu.h>
#include <SDL.h>

typedef struct IO_DISPLAY  IO_DISPLAY;

#define IOBASE_DISPLAY		0xFFFE00
#define IOMASK_DISPLAY		0xFFFF00

#define IOBASE_PALETTE0		0xFFFC00
#define IOBASE_PALETTE1		0xFFFD00
#define IOMASK_PALETTE		0xFFFF00

#define COLORBORDERLO		0
#define COLORBORDERHI		1
#define COLORBACKGROUNDLO	2
#define COLORBACKGROUNDHI	3
#define COLOR00LO			4
#define COLOR00HI			5
#define SCREENBASELO		6
#define SCREENBASEHI		7
#define SCREENBASEBANK		8
#define CONTROL				9
#define CURSORLO			10
#define CURSORHI			11

struct IO_DISPLAY {
	byte colorBorderLo;
	byte colorBorderHi;
	byte colorBackgroundLo;
	byte colorBackgroundHi;
	byte color00Lo;
	byte color00Hi;
	byte screenBaseLo;
	byte screenBaseHi;
	byte screenBaseBank;
	byte control;
	byte cursorLo;
	byte cursorHi;
};

struct Threadinfo {
	SDL_Renderer *sdlRenderer;
	SDL_Texture *sdlTexture;
};

typedef struct Threadinfo Threadinfo;


int
display_initialize( void );

void
display_expunge( void );

byte
display_read( word32 address, word32 timestamp );

byte
display_read_palette( word32 address, word32 timestamp );

void
display_write( word32 address, byte b, word32 timestamp );

void
display_write_palette( word32 address, byte b, word32 timestamp );

int
display_update(void *);

SDL_Window *getWindow();
#endif

