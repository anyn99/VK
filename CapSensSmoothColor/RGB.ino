byte state = 255;
byte calc;

byte RGBchange(){
    
  
  analogWrite(RLEDPin, (state - 128));
  analogWrite(GLEDPin, (
    
  calc = calc >> 1;
  if (calc % 2 == 1) {
    digitalWrite(BLEDPin, 127);
    }
  else digitalWrite(BLEDPin, LOW);
  
  state--;
  if (state == 0) state = 7;
  
  return state;
 }

