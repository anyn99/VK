.def  Di0      =r12
.def  Di1      =r13
.def  Di2      =r14
.def  Di3      =r15

.equ  Status      =GPIOR0  ;Flag register
.equ  _NewTouch    =0

.equ  Touch1A      =3    ;PortD
.equ  Touch1B      =4
;.equ  Touch2A      =5    
;.equ  Touch2B      =6

.equ  Touch1L      =$0100
.equ  Touch1H      =$0101
.equ  Touch2L      =$0102
.equ  Touch2H      =$0103

;--------------------------------------------------
Touch:
 sbi  PortB, Touch1B
 sbi  PortB, Touch2B
 clr  XH
 clr  XL
 ldi  Temp,  0b01001010
 ldi   TempH, 0b00000010
 ldi  Temp3, 0b00100110
 out  DDRB, TempH

_TouchLoop:
 out  DDRB, Temp
 out  DDRB, TempH
 out  DDRB, Temp3
 out  DDRB, TempH
 adiw  XH:XL, 1
 cpi  XH, $04
 breq  _TouchEnd1
 
 sbis  PinB, Touch1B
 movw  Di1:Di0, XH:XL

 sbis  PinB, Touch2B
 movw  Di3:Di2, XH:XL
 rjmp  _TouchLoop

_TouchEnd1:
 sbi  DDRB, Touch1A      ;apply GND on touch terminal 1A
 cbi  PortB, Touch1B      ;prepare low level on terminal 1B
 sbi  DDRB, Touch1B      ;apply GND on touch terminal 1B
 sbi  DDRB, Touch2A      ;apply GND on touch terminal 2A
 cbi  PortB, Touch2B      ;prepare low level on terminal 2B
 sbi  DDRB, Touch2B      ;apply GND on touch terminal 2B
 sts  Touch1L, Di0
 sts  Touch1H, Di1
 sts  Touch2L, Di2
 sts  Touch2H, Di3
 sbi  Status, _NewTouch
 ret


