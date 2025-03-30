/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * All Rights Reserved
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib65816/cpu.h"
#include "lib65816/cpuevent.h"
#include "decoder.h"
#include "ram.h"
#include "rom.h"
#include "io.h"


FILE *hardfile; /* Used in serbus.c */
static CPUEvent myEvent;

static int k2_initialize(void) {
	if (!ram_initialize())
		goto no_ram;
	if (!rom_initialize())
		goto no_rom;
	if (!io_initialize())
		goto no_io;

		return 1;

	no_io: rom_expunge();

	no_rom: ram_expunge();

	no_ram: return 0;
}

static void k2_expunge(void) {
	io_expunge();
	rom_expunge();
	ram_expunge();
}

void EMUL_handleWDM(byte opcode, word32 timestamp) {
	int i, j;

	switch (opcode) {
	case 0:
		fprintf( stderr, "WDM Emulator Exit Requested\n");
		k2_expunge();
		exit(0);
		break;

	case 1:
		//CPU_setTrace(1);
		cpu_cycle_start = cpu_cycle_sum;
		cpu_cycle_pause = 0;
		break;
	case 2:
		//CPU_setTrace(0);
		cpu_cycle_end = cpu_cycle_sum;
		fprintf(stderr, "CPU cycles: %d\n", (int)(cpu_cycle_end - cpu_cycle_start));
		break;
	case 4:
		cpu_cycle_pause += (cpu_cycle_sum - cpu_cycle_start);
		fprintf(stderr, "CPU cycles: %d\n", (int)cpu_cycle_pause);
		break;
	case 5:
		cpu_cycle_start = cpu_cycle_sum;
		break;
	case 6:
		CPU_setTrace(0);
		break;
	case 7:
		CPU_setTrace(1);
		break;
	case 3: /* simple stack trace */
		i = S.W & 0xFFF0;

		for (j = 0; j < 64; j++) {
			if ((j & 15) == 0)
				fprintf( stderr, "\n%04X - ", i);
			if (i == S.W)
				fprintf( stderr, "*%02X", MEM_readMem(i, timestamp, 0));
			else
				fprintf( stderr, " %02X", MEM_readMem(i, timestamp, 0));
			i += 1;
		}
		fprintf( stderr, "\n");
		break;

	default:
		fprintf( stderr, "Unknown WDM opcode $%02X at $%06lX\n", opcode, PC.A);
		break;
	}
}


void myHandler(word32 timestamp) {
	//printf("myHandler: %u\n", (unsigned int) timestamp);
	//fflush(stdout);
	CPUEvent_schedule(&myEvent, 1000, &myHandler);
}

void EMUL_hardwareUpdate(word32 timestamp) {
	printf("EMUL_hardwareUpdate: %u\n", (unsigned int) timestamp);
	fflush(stdout);
}



int main(int argc, char *argv[]) {
	long l;
	int i;
	short s;
	char *ptr, ptr1[256];
	word32 w32;
	size_t st;

	printf("LONG   : %d\n", sizeof(l));
	printf("INT    : %d\n", sizeof(i));
	printf("SHORT  : %d\n", sizeof(s));
	printf("PTR    : %d\n", sizeof(ptr));
	printf("PTR1   : %d\n", sizeof(ptr1));
	printf("word32 : %d\n", sizeof(w32));
	printf("size_t : %d\n", sizeof(st));
	fflush(stdout);

	CPUEvent_initialize();

	//CPUEvent_schedule(&myEvent, 1000, &myHandler);

	if (k2_initialize()) {

		CPU_reset();
		CPU_setTrace(0);
		CPU_run();
	}

	k2_expunge();

	if (hardfile)
		fclose(hardfile);
	return 0;
}

