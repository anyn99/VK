byte state = 6;
byte calc;

byte RGBchange(){
    
  calc = state;
  
  if (calc % 2 == 1) {
    digitalWrite(RLEDPin, LOW);
    }
  else digitalWrite(RLEDPin, HIGH);

  calc = calc >> 1;
  if (calc % 2 == 1) {
    digitalWrite(GLEDPin, LOW);
    }
  else digitalWrite(GLEDPin, HIGH);
    
  calc = calc >> 1;
  if (calc % 2 == 1) {
    digitalWrite(BLEDPin, LOW);
    }
  else digitalWrite(BLEDPin, HIGH);
  
  state--;
  if (state == 0) state = 7;
  
  return state;
 }

