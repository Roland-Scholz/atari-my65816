# atari-my65816
Sources for my65816 FPGA computer

I am mainly working on this project because I want to fool around with FPGA
The aim was to implement a 65816 machine which uses a OS like the one found in the Atari XL/XE.
Apart from the basic (kernel)-OS, nothing is compatible or even like the original Atari XL/XE series.

- homemade 65816 FPGA core, running at 100Mhz, not state compatible
- 8MB SD-RAM, running at 100Mhz
- RGB (Scart) output
- Resolutions:
	- 320x200 graphics 256 out of 512 colors
	- 640x200 graphics 2 / 512 colors
	-  80x25  Text 2/512 colors
	-  80x25  Text 16 forground, 16 background colors
	-  8 sprites 24x200 pixel 16/256 colors2
- 2 x RS232: Terminal 115200 baud, Mouse 1200 Baud
- PS/2 keyboard interface
- SD-Card interface
- 4x7seg display
- beeper
- 1 reset, 4 buttons

- Atari-like kernel/monitor, Fat16-DOS
- VTL-2 with 24-bit enhancements, load and save command

