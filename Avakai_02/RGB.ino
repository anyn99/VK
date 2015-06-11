


void RGBchange(byte state){
    
  byte calc = state;
  
  if (calc % 2 == 1) {
    analogWrite(RLEDPin, 220);
    }
  else digitalWrite(RLEDPin, HIGH);

  calc = calc >> 1;
  if (calc % 2 == 1) {
    analogWrite(GLEDPin, 220);
    }
  else digitalWrite(GLEDPin, HIGH);
    
  calc = calc >> 1;
  if (calc % 2 == 1) {
    analogWrite(BLEDPin, 220);
    }
  else digitalWrite(BLEDPin, HIGH);
  
 
 }

