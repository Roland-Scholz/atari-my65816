#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib65816/cpu.h"
#include "lib65816/cpuevent.h"
#include "decoder.h"
#include "display.h"
#include "ram.h"
#define SDL_MAIN_HANDLED
#include "SDL.h"

extern byte *ram;

#define WIDTH 640
#define HEIGHT 200
#define STRETCHX 1
#define STRETCHY 2
#define BORDER 50

static Threadinfo thi;
static byte *font;
static byte *frameBuf;
static byte *displayRam;
static byte *colorRam;
static SDL_Rect DestR;
static IO_DISPLAY display;
static byte foregroundColor[] = { 0xff, 0xff, 0xff };
static byte backgroundColor[] = { 40, 30, 10 };
static byte borderColor[] = { 0x00, 0x80, 0xff };
static SDL_Window *window;

union palette {
	byte bytes[512];
	unsigned short words[256];
} palette;

SDL_Window* getWindow() {
	return window;
}

byte display_read(word32 address, word32 timestamp) {
	switch (address & 0x0f) {

	case COLORBORDERLO:
		return display.colorBorderLo;
	case COLORBORDERHI:
		return display.colorBorderHi;
	case COLORBACKGROUNDLO:
		return display.colorBackgroundLo;
	case COLORBACKGROUNDHI:
		return display.colorBackgroundHi;
	case COLOR00LO:
		return display.color00Lo;
	case COLOR00HI:
		return display.color00Hi;
	case CURSORLO:
		return display.cursorLo;
	case CURSORHI:
		return display.cursorHi;
	case SCREENBASELO:
		return display.screenBaseLo;
	case SCREENBASEHI:
		return display.screenBaseHi;
	case SCREENBASEBANK:
		return display.screenBaseBank;
	case CONTROL:
		return display.control;
	default:
		return 0;
	}
}

byte display_read_palette(word32 address, word32 timestamp) {
	return palette.bytes[(address & 0x1ff)];
}

void display_write_palette(word32 address, byte b, word32 timestamp) {
	//printf("palette write %02X at %08X %08X\n", b, (unsigned int)address, (address & 0x1ff) ^ 0x100);
	palette.bytes[(address & 0x1ff)] = b;
}

void display_write(word32 address, byte b, word32 timestamp) {

	switch (address & 0x1f) {
	case COLORBORDERLO:
		display.colorBorderLo = b;
		break;
	case COLORBORDERHI:
		display.colorBorderHi = b;
		break;
	case COLORBACKGROUNDLO:
		display.colorBackgroundLo = b;
		break;
	case COLORBACKGROUNDHI:
		display.colorBackgroundHi = b;
		break;
	case COLOR00LO:
		display.color00Lo = b;
		break;
	case COLOR00HI:
		display.color00Hi = b;
		break;
	case CURSORLO:
		display.cursorLo = b;
		break;
	case CURSORHI:
		display.cursorHi = b;
		break;
	case SCREENBASELO:
		display.screenBaseLo = b;
		break;
	case SCREENBASEHI:
		display.screenBaseHi = b;
		break;
	case SCREENBASEBANK:
		display.screenBaseBank = b;
		break;
	case CONTROL:
		//printf("control: %02X\n", b);
		display.control = b;
		break;
	default:
		break;
	}
}

void setBuf(byte **buf, int bits, int bw, int color) {

	int index, i, pixel;

	for (i = 128; i != 0; i >>= 1) {
		pixel = bits & i;

		if (bw == 0) {
			index = 0;
			if (pixel) index = 255;
		} else {
			index = color >> 4;
			if (pixel) index = color & 0x0f;
		}

		**buf = palette.words[index] << 5;
		(*buf)++;
		**buf = (palette.words[index] & 0x38) << 2;
		(*buf)++;
		**buf = (palette.words[index] & 0x1c0) >> 1;
		(*buf)++;
	}

	return;
}

int display_update(void *data) {
//Print incoming data
	int line;
	int x, i, xmax, charline;
	unsigned int index, screenBase;
	byte *buf, *scanline, *colorline;
	byte bits;
	int red, green, blue, color;

	Threadinfo *thi = (Threadinfo *) data;

	scanline = malloc(WIDTH);
	colorline = malloc(WIDTH);

	for (;;) {
		//printf("update\n");
		//fflush(stdout);

		buf = frameBuf;
		i = 0;
		charline = 0;

		screenBase = display.screenBaseLo + (display.screenBaseHi << 8)
				+ (display.screenBaseBank << 16);
		screenBase <<= 1;

		//printf("screenBase: %08X\n", screenBase);

		displayRam = &(ram[screenBase]);
		colorRam = displayRam + 0x800;

		//320x200 selected?
		xmax = WIDTH / 8;
		if (display.control & 0x02) {
			xmax = 320;
		}

		memcpy(scanline, displayRam, xmax);
		memcpy(colorline, colorRam, xmax);

		for (line = 0; line < HEIGHT; line++) {
			for (x = 0; x < xmax; x++) {

				if ((display.control & 3) == 0) {
					index = (scanline[x] << 3) + charline;
					bits = font[index];
					color = colorline[x];
					if ((i + x == display.cursorLo + (display.cursorHi << 8))
							&& (display.control & 0x04) == 4) {
						if (display.control & 0x08) color = 0x90;
						else bits ^= 0xff;
					}
				} else {
					bits = scanline[x];
				}

				if ((display.control & 0x02) == 0) {
					setBuf(&buf, bits, display.control & 0x08, color);
				} else {
					*buf++ = (palette.words[bits] << 5);
					*buf++ = (palette.words[bits] << 2) & 0xe0;
					*buf++ = (palette.words[bits] >> 1) & 0xe0;
					*buf++ = (palette.words[bits] << 5);
					*buf++ = (palette.words[bits] << 2) & 0xe0;
					*buf++ = (palette.words[bits] >> 1) & 0xe0;
				}

			}

			if ((display.control & 3) == 0) {
				charline++;
				if (charline >= 8) {
					charline = 0;
					i += xmax;
					memcpy(scanline, displayRam + i, xmax);
					memcpy(colorline, colorRam + i, xmax);
				}
			} else {
				i += xmax;
				memcpy(scanline, displayRam + i, xmax);
			}
		}

		//GGBBBRRR

		red = display.colorBorderLo & 7;
		green = (display.colorBorderLo >> 3) & 7;
		blue = (display.colorBorderLo >> 6)
				| ((display.colorBorderHi & 1) << 2);
		borderColor[0] = red << 5;
		borderColor[1] = green << 5;
		borderColor[2] = blue << 5;

		red = display.colorBackgroundLo & 7;
		green = (display.colorBackgroundLo >> 3) & 7;
		blue = (display.colorBackgroundLo >> 6)
				| ((display.colorBackgroundHi & 1) << 2);
		backgroundColor[0] = red << 5;
		backgroundColor[1] = green << 5;
		backgroundColor[2] = blue << 5;

		red = display.color00Lo & 7;
		green = (display.color00Lo >> 3) & 7;
		blue = (display.color00Lo >> 6) | ((display.color00Hi & 1) << 2);

		foregroundColor[0] = red << 5;
		foregroundColor[1] = green << 5;
		foregroundColor[2] = blue << 5;

		SDL_UpdateTexture(thi->sdlTexture, NULL, frameBuf, WIDTH * 3);

		SDL_RenderClear(thi->sdlRenderer);
		SDL_SetRenderDrawColor(thi->sdlRenderer, borderColor[0], borderColor[1],
				borderColor[2], 255);
		SDL_RenderFillRect(thi->sdlRenderer, NULL);
		SDL_RenderSetViewport(thi->sdlRenderer, &DestR);
		SDL_RenderCopy(thi->sdlRenderer, thi->sdlTexture, NULL, NULL);
		SDL_RenderPresent(thi->sdlRenderer);

		SDL_Delay(20);
	}

	return 0;
}

int display_initialize() {

	font = malloc(2 * 1024);
	frameBuf = malloc(WIDTH * HEIGHT * 3);

	FILE *ffont = fopen("Bm437_IBM_BIOS1.bin", "rb");

	if (ffont == NULL) {
		SDL_Log("Unable to load font");
		return 0;
	}

	SDL_Log("font bytes read %d\n", fread(font, 1, 2048, ffont));

	fclose(ffont);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 0;
	}

	window = SDL_CreateWindow("My65816", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, WIDTH * STRETCHX + (BORDER << 1),
			(HEIGHT * STRETCHY) + (BORDER << 1), 0);

	if (window == NULL) {
		SDL_Log("Unable to create SDL-Window: %s", SDL_GetError());
		return 0;
	}

	SDL_Renderer *sdlRenderer = SDL_CreateRenderer(window, -1, 0);
	if (sdlRenderer == NULL) {
		SDL_Log("Unable to initialize SDL-Renderer: %s", SDL_GetError());
		return 0;
	}

	SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer,
			SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	if (sdlTexture == NULL) {
		SDL_Log("Unable to initialize SDL-Texture: %s", SDL_GetError());
		return 0;
	}

	thi.sdlRenderer = sdlRenderer;
	thi.sdlTexture = sdlTexture;

	DestR.x = BORDER;
	DestR.y = BORDER;
	DestR.w = WIDTH * STRETCHX;
	DestR.h = HEIGHT * STRETCHY;

	display.colorBorderLo = 0x1c;
	display.colorBorderHi = 0x00;
	display.colorBackgroundLo = 0xc0;
	display.colorBackgroundHi = 0x00;
	display.color00Lo = 0xff;
	display.color00Hi = 0x01;
	display.screenBaseLo = 0x00;
	display.screenBaseHi = 0xF8;
	display.screenBaseBank = 0x3f;
	display.control = 0x04;

	SDL_ShowCursor(SDL_DISABLE);

	SDL_CreateThread(display_update, "DisplayThread",
			(void *)&thi);

	return 1;
}

