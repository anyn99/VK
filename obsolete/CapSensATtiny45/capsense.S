
.def  Di0      =r12
.def  Di1      =r13
.def  Di2      =r14
.def  Di3      =r15

.equ  Status      =GPIOR0  ;Flag register
.equ  _NewTouch    =0

.equ  Touch1A      =2    ;PortD
.equ  Touch1B      =3
.equ  Touch2A      =5    
.equ  Touch2B      =6

.equ  Touch1L      =$0100
.equ  Touch1H      =$0101
.equ  Touch2L      =$0102
.equ  Touch2H      =$0103


;--------------------------------------------------
Touch:
 sbi  PortD, Touch1B    ;setzt Pin 3 auf "HIGH" (Pullup on??)
 sbi  PortD, Touch2B	;setzt Pin 6 auf "HIGH"
 clr  XH				;löscht XH
 clr  XL				;löscht XL
 ldi  Temp,  0b01001010	;lade in jeweilige Register
 ldi  TempH, 0b00000010
 ldi  Temp3, 0b00100110
 out  DDRD, TempH		;setzt PD1 als AUsgang

_TouchLoop:
 out  DDRD, Temp		;setzt PD1, PD3 und PD6 als Ausgänge
 out  DDRD, TempH		;setzt PD1 als Ausgang	
 out  DDRD, Temp3		;setzt PD1, PD2, PD5 als Ausgänge
 out  DDRD, TempH		;setzt PD1 als Ausgang
 adiw  XH:XL, 1			;Zählregister +1
 cpi  XH, $04			;vergleiche XH mit 0b00000100
 breq  _TouchEnd1		;falls ja, Ende
 
 sbis  PinD, Touch1B	;wenn Pin 3 gesetzt ist, überspringe
 movw  Di1:Di0, XH:XL	;schreibe Zähler in Register

 sbis  PinD, Touch2B	;wenn Pin 3 gesetzt ist, überspringe
 movw  Di3:Di2, XH:XL	;schreibe Zähler in Register
 rjmp  _TouchLoop		;springe zurück

_TouchEnd1:
 sbi  DDRD, Touch1A      ;apply GND on touch terminal 1A
 cbi  PortD, Touch1B      ;prepare low level on terminal 1B
 sbi  DDRD, Touch1B      ;apply GND on touch terminal 1B
 sbi  DDRD, Touch2A      ;apply GND on touch terminal 2A
 cbi  PortD, Touch2B      ;prepare low level on terminal 2B
 sbi  DDRD, Touch2B      ;apply GND on touch terminal 2B
 sts  Touch1L, Di0
 sts  Touch1H, Di1
 sts  Touch2L, Di2
 sts  Touch2H, Di3
 sbi  Status, _NewTouch
 ret

