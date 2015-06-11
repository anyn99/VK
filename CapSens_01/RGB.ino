


void RGBchange(byte state){
    
  byte calc = state;
  
  if (calc % 2 == 1) {
    digitalWrite(RLEDPin, LOW);
    }
  else analogWrite(RLEDPin, 30);

  calc = calc >> 1;
  if (calc % 2 == 1) {
    digitalWrite(GLEDPin, LOW);
    }
  else analogWrite(GLEDPin, 30);
    
  calc = calc >> 1;
  if (calc % 2 == 1) {
    digitalWrite(BLEDPin, LOW);
    }
  else analogWrite(BLEDPin, 30);
    
 
 }

