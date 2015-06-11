
	
	
	uint8_t a, b, x, y;

  ATOMIC_BLOCK(ATOMIC_FORCEON){
    a = DDRB  & ~(MASK_REFPINS | MASK_SENSEPINS);
    b = PORTB & ~(MASK_REFPINS | MASK_SENSEPINS);
    y = MASK_SENSEPINS;                        // input mask
    do{
      DDRB = a;                         // tristate
      PORTB = b | MASK_SENSEPINS;
      DDRB = a | MASK_SENSEPINS;               // Bx = strong high
      DDRB = a;                         // tristate
      PORTB = b;
      DDRB = a | MASK_REFPINS;               // Ax = strong low
      if( ++total >= MAX_CYCLE )                    // timeout
        return -1;
      x = y & PINB;  
    }while( !x );                        // all inputs done
    DDRB = a | MASK_REFPINS | MASK_SENSEPINS;       // discharge
  }
  
  
  
  --------------------------------
  
  
  
  cli();
    DDRB  &= ~_BV(PB3); //Beide Pins als Eingänge
	DDRB  &= ~_BV(PB4);
    PORTB &= ~_BV(PB3); //Pullups deaktivieren
	PORTB &= ~_BV(PB4);
    
    DDRB  |= _BV(PB4); //Beide Pins als Ausgänge
    DDRB  |= _BV(PB3); //Werden auf "LOW" gezogen durch PORT Befehl vorher?
	
    _delay_us(5);		//warten bis sicher "LOW"
    DDRB &= ~_BV(PB4);	//Beide Pins wieder als Eingänge
    DDRB &= ~_BV(PB3);

    PORTB |= _BV(PB3);	//Pullup aktiviert am SensePin
	
    while ( !(PINB & _BV(PB3))) { //Schleife bis SensePin "HIGH" wird
	
        DDRB |=  _BV(PB3);	//Setze SensePin als Ausgang
        _delay_us(5);		//Warte bis SensePin "HIGH" ist	-> Touch-Kondensator geladen
        DDRB &= ~_BV(PB3);	//Setze SensePin als Eingang
        DDRB |=  _BV(PB4);	//Setze RefPin als Ausgang -> auf "LOW"-Level
        _delay_us(20);		//Warte -> Touch-Kondensator entlädt sich in Cs
        DDRB &= ~_BV(PB4);	//Setze RefPin als Eingang -> "tristate"
    
		total++;			//Zähler hochsetzen
	}
    DDRB |= _BV(PB4);		//Beide Pins als Ausgänge	
    DDRB |= _BV(PB3);	
    PORTB &= ~_BV(PB4); 	//Setze beide Pins auf "LOW"
	PORTB &= ~_BV(PB3);
	sei();