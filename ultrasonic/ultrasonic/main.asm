;
; Smart Dustbin
;
; Group 31
;
;


.include "m328pdef.inc"
	.org 0x00
	.def G1 = r16
	.def G2	= r17

	cbi PORTB,0
	cbi PORTB,1

start:
	ldi G1 , 0b00000010 // PB0 port input  0-input, 1- output
	out DDRB , G1
	ldi G2 , 0b11111111
	out DDRD , G2
	ldi G2 , 0b11111111
	out DDRC , G2

	sbi PORTB	,	1
	rcall	delay_timer0
	cbi PORTB	,	1

	rcall echo_PW

	mov R19 , R28
	out PORTD , R19

	rcall con

	rcall delay_ms
	rjmp start

//---------------------------------------------
con:
	cpi R28	, 0b00001000
	brlo	low0
	brge	con2
	ret
con2:
	cpi R28	, 0b00100000
	brlo	mid60
	brge	hig100
	ret
low0:
	ldi R19	, 0b0000001
	out PORTC , R19
	ret

mid60:
	ldi R19	, 0b00000010
	out PORTC , R19
	ret

hig100:
	ldi R19	, 0b00000100
	out PORTC , R19
	ret
//-----------------------------------------------------------


;===============================================================
echo_PW:
;-------
    LDI   R20, 0b00000000
    STS   TCCR1A, R20     ;Timer 1 normal mode
    LDI   R20, 0b11000101 ;set for rising edge detection &
    STS   TCCR1B, R20     ;prescaler=1024, noise cancellation ON
    ;-----------------------------------------------------------
l1: IN    R21, TIFR1
    SBRS  R21, ICF1
    RJMP  l1              ;loop until rising edge is detected
    ;-----------------------------------------------------------
    LDS   R16, ICR1L      ;store count value at rising edge
    ;-----------------------------------------------------------
    OUT   TIFR1, R21      ;clear flag for falling edge detection
    LDI   R20, 0b10000101
    STS   TCCR1B, R20     ;set for falling edge detection
    ;-----------------------------------------------------------
l2: IN    R21, TIFR1
    SBRS  R21, ICF1
    RJMP  l2              ;loop until falling edge is detected
    ;-----------------------------------------------------------
    LDS   R28, ICR1L      ;store count value at falling edge
    ;-----------------------------------------------------------
    SUB   R28, R16        ;count diff R22 = R22 - R16
    OUT   TIFR1, R21      ;clear flag for next sensor reading
    RET


;===============================================================
;delay subroutines
;===============================================================
delay_timer0:             ;10 usec delay via Timer 0
;------------
    CLR   R20
    OUT   TCNT0, R20      ;initialize timer0 with count=0
    LDI   R20, 20
    OUT   OCR0A, R20      ;OCR0 = 20
    LDI   R20, 0b00001010
    OUT   TCCR0B, R20     ;timer0: CTC mode, prescaler 8
    ;-----------------------------------------------------------
l0: IN    R20, TIFR0      ;get TIFR0 byte & check
    SBRS  R20, OCF0A      ;if OCF0=1, skip next instruction
    RJMP  l0              ;else, loop back & check OCF0 flag
    ;-----------------------------------------------------------
    CLR   R20
    OUT   TCCR0B, R20     ;stop timer0
    ;-----------------------------------------------------------
    LDI   R20, (1<<OCF0A)
    OUT   TIFR0, R20      ;clear OCF0 flag
    RET

;===============================================================
delay_ms:
;--------
    LDI   R21, 255
l6: LDI   R22, 255
l7: LDI   R23, 50
l8: DEC   R23
    BRNE  l8
    DEC   R22
    BRNE  l7
    DEC   R21
    BRNE  l6
    RET