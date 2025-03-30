/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * All Rights Reserved
 *
 * Keyboard Input and MOuse controller
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "mouse.h"
#include "display.h"

static IO_MOUSE mouse;
static int mouseState = 3;
static byte mouse0, mouse1, mouse2;

extern byte *ram;
extern word32 cpu_irq;

void mouse_expunge(void) {
	/* does nothing for the time being */
}

byte mouse_read(word32 address, word32 timestamp) {
	address &= 0x0f;
	if (address == RBR1) {
		mouse.regs[IIR1] = 1;		//clear interrupt
		mouse.regs[LSR1] = 0;		//clear status
	}

	return mouse.regs[address];
}

void mouse_write(word32 address, byte b, word32 timestamp) {
	address &= 0x0f;

	mouse.regs[address] = b;

	if (address == THR1) {
		mouse.regs[LSR1] = 1;				//set status
		if (mouse.regs[IER1] & 0x01) {//if interrupt enabled, set IIR and fire IRQ
			mouse.regs[IIR1] = 0x04;
			CPU_addIRQ(1);
		}
	}
}

void mouse_state() {
	if (mouseState == 1 && cpu_irq == 0) {
		//printf("mouseState: %d : %d\n", mouseState, mouse1);
		mouseState++;
		mouse_write(THR1, mouse1, 0);
	}

	if (mouseState == 2 && cpu_irq == 0) {
		//printf("mouseState: %d : %d\n", mouseState, mouse2);
		mouseState++;
		mouse_write(THR1, mouse2, 0);
	}
}

void mouse_start(SDL_Event *evt) {
	int x, y;
	char title[64];

	switch (evt->type) {

	case SDL_MOUSEBUTTONDOWN:
		switch (evt->button.button) {
		case SDL_BUTTON_LEFT:
			ram[0x377] |= 0x80;
			break;
		case SDL_BUTTON_RIGHT:
			ram[0x377] |= 0x40;
			break;
		default:
			break;
		}

		break;
	case SDL_MOUSEBUTTONUP:
		switch (evt->button.button) {
		case SDL_BUTTON_LEFT:
			ram[0x377] &= ~0x80;
			break;
		case SDL_BUTTON_RIGHT:
			ram[0x377] &= ~0x40;
			break;
		default:
			break;
		}

		break;

	case SDL_MOUSEMOTION:

		if (mouseState == 3) {

			mouse0 = 0x40;
			if (evt->motion.xrel < 0) mouse0 |= 3;
			if (evt->motion.yrel < 0) mouse0 |= 12;

			//mouse0 |= (evt->motion.xrel >> 6);
			//mouse0 |= ((evt->motion.yrel & 0xc0) >> 4);

			mouse1 = evt->motion.xrel & 0x3f;
			mouse2 = evt->motion.yrel & 0x3f;

			mouseState = 1;
			//printf("mouseState: %d : %d\n", 0, mouse0);
			mouse_write(THR1, mouse0, 0);

		}
		x = evt->motion.x - 50;

		if (x < 0)
			x = 0;
		if (x > 639)
			x = 639;
		ram[0x373] = x;
		ram[0x374] = x >> 8;

		y = (evt->motion.y - 50) >> 1;
		if (y < 0)
			y = 0;
		if (y > 199)
			y = 199;

		ram[0x375] = y;
		ram[0x376] = y >> 8;

		sprintf(title, "My65816 X:%d, Y:%d, %d %d", x, y, evt->motion.xrel,
				evt->motion.yrel);
		SDL_SetWindowTitle(getWindow(), title);
		break;

	default:
		break;
	}
}

int mouse_initialize(void) {

	return 1;
}
