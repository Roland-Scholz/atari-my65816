		org	$E000

		XDEF kernel

;----------------------------------------------------------------------------
; Equations
;----------------------------------------------------------------------------
NEGATIVE	equ	$80
OVERFLOW	equ	$40		; 
ACC		equ	$20		; Accu 8/16-bit
IX		equ	$10		; Index 8/16-bit
DECIMAL_FLAG	equ	$08		; Decimal Flag
INTDIS		equ	$04		; IRQ disable
ZERO		equ	$02		; zero flag
CARRY		equ	$01		; Carry


ioout		equ	$D800
ioin		equ	$D900

EOL		equ	$0d
chrin		equ	serin
chrout		equ	serout


;----------------------------------------------------------------------------
; Direct-page definitions
;----------------------------------------------------------------------------
serbyte		equ	$00				;1-byte serin accumulator
ptr		equ	$01				;2-byte general pointer
memptr		equ	$03				;3-byte memory pointer
gethex_A	equ	$06
chksum		equ	$07

MOVENEG		equ	$0200				;4-byte MVP + operands + RTS
MOVEBNK		equ	$0201
MOVERTS		equ	$0203				;RTS
ADRCNT		equ	$0204
JSLOP		equ	$0205				;1-byte JSL-opcode
JSLADR		equ	$0206				;3-byte JSL address
JSLRTS		equ	$0209				;1-byte RTS-opcode


;----------------------------------------------------------------------------
; Macro definitions
;----------------------------------------------------------------------------
A8I8		MACRO
		sep	#ACC+IX
		longa	off
		longi	off
		ENDM
		
A8		MACRO
		sep	#ACC
		longa	off
		ENDM

I8		MACRO
		sep	#IX
		longi	off
		ENDM

A16I16		MACRO
		rep	#ACC+IX
		longa	on
		longi	on
		ENDM
		
A16		MACRO
		rep	#ACC
		longa	on
		ENDM

I16		MACRO
		rep	#IX
		longi	on
		ENDM
		
;----------------------------------------------------------------------------
; Start of Kernel
;----------------------------------------------------------------------------
kernel		sei
		rep	#CARRY+DECIMAL_FLAG		
		xce					;switch to native mode

		A16
		lda	#$01ff				;stack to $1ff
		tcs	
		lda	#0
		tcd					;direct to 0
		
		A8					;acc 	8-bit
		I16					;index 16-bit
		pha
		plb					;databank = 0


		ldx	#$02ff
kernel1		stz	$0,x
		dex
		bpl	kernel1
		
		lda	#$54				;MVN
		sta	MOVENEG		
		lda	#$60		
		sta	MOVERTS				;RTS

menu
menustart:	
		ldx	#MSGSTART
		jsr	print
		
menuadr:	jsr	newline
		lda	#'F'
		jsr	chrout
		php
		pla
		jsr	printhex
		jsr	space
		lda	#'B'
		jsr	chrout
		phb
		pla
		jsr	printhex
		jsr	space
		lda	#'K'
		jsr	chrout
		phk
		pla
		jsr	printhex
		jsr	space
		lda	#'D'
		jsr	chrout
		tdc
		xba
		jsr	printhex
		xba
		jsr	printhex
		jsr	space
		lda	#'S'
		jsr	chrout
		tsc
		xba
		jsr	printhex
		xba
		jsr	printhex
		jsr	space

		lda	<memptr+2
		jsr	printhex
		lda	#':'
		jsr	chrout
		lda	<memptr+1
		jsr	printhex
		lda	<memptr
		jsr	printhex
		lda	#'>'
		jsr	chrout
		jsr	space
	
menuloop:	jsr	getupper
		ldx	#menuvec-menukey-1
			
menuloop1:	cmp	menukey,x
		beq	menufound
		dex	
		bpl	menuloop1
		jmp	menuadr
menufound:	lda	#0
		xba
		txa	
		asl	
		tax	
		jsr	(menuvec,x)
		jmp	menuadr

;----------------------------------------------------------------------------
; Menu routines
;----------------------------------------------------------------------------
setdatabank:	lda	<memptr+2
		jsr	printhex
		lda	#':'
		jsr	chrout
		jsr	space
		jsr	inputhex
		sta	<memptr+2
		rts
;
;
;
changemem:	lda	[memptr]
		jsr	printhex
		lda	#':'
		jsr	chrout
		jsr	space
		jsr	inputhex
		sta	[memptr]
		rts
;
;
;
hexdownload:	I8
		ldx	#MSGDOWNL
		jsr	print
	
download:	jsr	chrin		; wait for leading ":"
		cmp	#':'
		bne	download
	
		lda	#0
		sta	<chksum
	
		jsr	gethex		; number of bytes in line
		tax	
		jsr	getadr		; addess in ptr
		jsr	gethex		; record-type 0=data record
		bne	download1
			
		tay			; A = 0!
download2:	jsr	gethex		; read byte
		sta	(ptr),y		; store at address
		iny	
		dex			; decement length
		bne	download2	; line not completely read
	
		jsr	gethex		; line finished
		lda	<chksum		; read checksum
		beq	download	; checksum OK, next line
		jsr	debug		; print debug
		jmp	download	; read next line
	
download1:	;jsr	time		; read last checksum and return
		ldx	#MSGFINISH
		jsr	chrin
		jmp	kernel
	
debug:		lda	<ptr+1
		jsr	printhex
		lda	<ptr
		jsr	printhex
		jsr	space
		lda	<chksum
		jsr	printhex
		jmp	newline

MSGDOWNL:	.byte	"downloading...",EOL ,0
MSGFINISH:	.byte	"... finished.", EOL ,0
;
; Routine to fill memory with prior user interaction
; fills from: memptr to: ptr
; 
fillmem:	
		ldx	#msgfill1
		jsr	print
	
		jsr	enteradr
		cmp	#'x'
		beq	fillend
	
		ldx	#msgfill2
		jsr	print
	
		jsr	inputhex
		sta	<ptr+1
		jsr	inputhex
		sta	<ptr
	
		ldx	#msgfill3
		jsr	print
	
		jsr	inputhex
		pha
		
		A16
		sec
		lda	<ptr
		sbc	<memptr
		bcc	fillend		

;		stz	[memptr],y
		beq	fillend
		
		ldx	memptr
		txy
		iny
		
fillend:	A8
		rts
		
msgfill1:	.byte	"fill from: ", 0
msgfill2:	.byte	" to ", 0
msgfill3:	.byte	" with: ",0

msgcopy1:	.byte	"copy from: ", 0
msgcopy2:	.byte	" dest: ", 0
;
;
;
gotoadr:	lda 	#$22		;JSL opcode
		sta	JSLOP
		lda	#$60
		sta	JSLRTS
		
		lda	<memptr
		sta	JSLADR
		lda	<memptr+1
		sta	JSLADR+1
		lda	<memptr+2
		sta	JSLADR+2

		;wdm	6
		jmp	JSLOP	
;
;
;		
memdump:
		jsr	newline
		ldx	#16
memdump3:	jsr	memdump16
		dex	
		bne	memdump3
		rts	
			
memdump16:	lda	<memptr+2
		jsr	printhex
		lda	#':'
		jsr	chrout
			
		lda	<memptr+1
		jsr	printhex
		lda	<memptr
		jsr	printhex
			
		jsr	space
	
		ldy	#0
memdump1:	cpy 	#8
		bne	memdump7
		jsr 	space
		jsr	space
memdump7:	lda	[memptr],y
		jsr	printhex
		jsr	space
		iny	
		cpy	#16
		bne	memdump1
			
		jsr	space
		lda	#'|'
		jsr	chrout
	
		ldy	#0
memdump5:	lda	[memptr],y
;		cmp	#TABU		; if >= TABU
;		bcs	memdump4	; print space
		cmp	#32		; if < space, print space
		bcs	memdump2
memdump4:	lda	#' '
memdump2:	jsr	chrout
		iny	
		cpy	#16
		bne	memdump5
	
		lda	#'|'
		jsr	chrout
	
		clc
		tya	
		adc	<memptr
		sta	<memptr
		bcc	memdump8
		inc	<memptr+1
		bne	memdump8
		inc	<memptr+2
memdump8:	jmp	newline
;
;
;
setaddress:
		ldx	#MSGADDR
		jsr	print
		jmp	enteradr

MSGADDR:	.byte	"address: ",0
;
;
;
prthelp:	pla	
		pla	
		jmp	menu
;
;
;		
enteradr:	ldx	#0
		stx	<memptr
		lda	#4
		sta	ADRCNT
		
enteradrC:	jsr	chrin
		cmp	#EOL
		beq	enteradrEnd
	
		cmp	#'x'
		beq	enteradrEnd
	
enteradrD:	jsr	chrout
		cmp	#':'
		bcs	enteradrE
		cmp	#'0'
		bcc	enteradrC
	
		and	#%00001111
		bcs	enteradrF
	
enteradrE:	cmp	#'g'
		bcs	enteradrC
		cmp	#'a'
		bcc	enteradrC
	
		sbc	#'7'
enteradrF:	asl
		asl
		asl
		asl
		ldx	#4
enteradrG:	asl
		rol	<memptr
		rol	<memptr+1
		dex
		bne	enteradrG

		dec	ADRCNT
		bne	enteradrC
enteradrEnd:	rts
;----------------------------------------------------------------------------
; Print routines
;----------------------------------------------------------------------------
newline:	pha	
		lda	#EOL
		bne	space1
	
space:		pha	
		lda	#32
	
space1:		jsr	chrout
		pla	
		rts	

printhex:	pha	
		pha	
		lsr	
		lsr	
		lsr	
		lsr	
		jsr	printnibble
		pla	
		and	#15
		jsr	printnibble
		pla	
		rts	
	
printnibble:	clc	
		adc	#48
		cmp	#58
		bcc	printnibble1
		adc	#6
printnibble1:	jmp	chrout
	

print:		pha	
		phx
		
print2:		lda	$0,x
		beq	print1
		jsr	chrout
		inx
		bra	print2
		
print1:		plx
		pla
		rts
		

;----------------------------------------------------------------------------
; Input routines
;----------------------------------------------------------------------------
inputhex:	jsr	inputnibble
		asl	
		asl	
		asl	
		asl	
		sta	<gethex_A
		jsr	inputnibble
		;clc	
		ora	<gethex_A
		sta	<gethex_A
		rts	
	
inputnibble:	jsr	getupper
		jsr	chrout
		sec	
		sbc	#48
		cmp	#10
		bcc	inputnibble1
		sbc	#7
inputnibble1:	rts
;
;
;		
getupper:	jsr	chrin
		cmp	#97
		bcc	getupperex	
		cmp	#123
		bcs	getupperex
		sbc	#31
getupperex	rts	
;
;
;	
getadr:		jsr	gethex
		sta	<ptr+1		
		jsr	gethex
		sta	<ptr
		rts	
;
;
;	
gethex:		jsr	getnibble
		asl	
		asl	
		asl	
		asl	
		sta	<gethex_A
		jsr	getnibble
		ora	<gethex_A
		sta	<gethex_A
	
		clc	
		adc	<chksum
		sta	<chksum
		lda	<gethex_A
		rts	
	
getnibble:	jsr	getupper
		sec	
		sbc	#48
		cmp	#10
		bcc	getnibble1
		sbc	#7
getnibble1:	rts	

;----------------------------------------------------------------------------
; reads a byte from serin in ACC and serbyte
;----------------------------------------------------------------------------
		
serin		phx
		phy
		php
		A8I8
		
serin2		lda	ioin				;wait for startbit (=0)
		lsr
		bcs	serin2

		jsr	rs232baud2			; 6 wait 1 1/2 baudrate

		ldx	#8				; 2
serin1		jsr	rs232baud			; 6	297 + 3+2+5+2+3 = 312
		lda	ioin				; 3
		lsr					; 2
		ror	<serbyte			; 5
		dex					; 2 
		bne	serin1				; 3
		
serin3		lda	ioin				;wait until ioin = 1
		lsr
		bcc	serin3
		
		lda	<serbyte
		
		plp
		ply
		plx
		rts
	
;----------------------------------------------------------------------------
; writes acc to serout
;----------------------------------------------------------------------------	
serout		phx
		phy
		php
		A8I8		

		ldx	#0				;startbit = 0
		stx	ioout
		jsr	rs232baud

		ldx	#8
serout1		sta	ioout
		jsr	rs232baud
		ror
		dex
		bne	serout1
		
		ldx	#1
		stx	ioout
		jsr	rs232baud
		ror
		
		plp
		ply
		plx
		rts
		
;----------------------------------------------------------------------------
; wait 19200 baud
;----------------------------------------------------------------------------	
rs232baud2	ldy	#29		
		bne	rs232baud1
rs232baud	ldy	#58				; 2	2 + 58*5 -1 + 6 = 297
rs232baud1	dey					; 2
		bne	rs232baud1			; 3
		rts					; 6
		
;----------------------------------------------------------------------------
; Kernel messages
;----------------------------------------------------------------------------	
menukey:	.byte	"?BCDFGKLMNST"

menuvec:	.word	prthelp
		.word	setdatabank
		.word	changemem
		.word	hexdownload
		.word	fillmem
		.word	gotoadr
		.word	prthelp		;disass16
		.word	prthelp		;disass8
		.word	memdump
		.word	setaddress
		.word	prthelp		;setbreak
		.word	prthelp		;copymem

MSGSTART:	.byte	EOL, EOL
		.byte	"*** 65816 - 6Mhz Homebrew by R. Scholz", EOL, EOL
		.byte	"? - This help list", EOL
		.byte	"B - Set data bank", EOL
		.byte	"C - Change byte", EOL
		.byte	"D - Download Intel-Hex file", EOL
		.byte	"F - Fill memory", EOL
		.byte	"G - Goto address", EOL
		.byte	"K - Disassemble 16", EOL
		.byte	"L - Disassemble 8", EOL
		.byte	"M - Memory dump", EOL
		.byte	"N - New address", EOL
		.byte 	"S - Set breakpoint", EOL
		.byte	"T - Transfer memory", EOL
		.byte	0
