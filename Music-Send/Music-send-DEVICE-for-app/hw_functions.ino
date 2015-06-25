

//------------------------------------------------------------
void RGBchange(byte state)
{
    
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


//------------------------------------------------------------
void sound (byte nr)
{
    /*
    Serial.print("Playing Nr. ");
    Serial.println(nr);
    */
    
    unsigned int STimer;
    
    if (nr>3) nr -= 3; 
    
    //Serial.println(nr);
    
    if (nr == 1) 
    {
      STimer=millis();
      digitalWrite(wav0,LOW); 
      while ( (millis() - STimer) < 60);
      digitalWrite(wav0,HIGH);
    }
    
    if (nr == 2) 
    {
      STimer=millis();
      digitalWrite(wav1,LOW); 
      while ( (millis() - STimer) < 60);
      digitalWrite(wav1,HIGH);
    
    }
    
    if (nr == 3) 
    {
      STimer=millis();
      digitalWrite(wav0,LOW); 
      digitalWrite(wav1,LOW); 
      while ( (millis() - STimer) < 60);
      digitalWrite(wav0,HIGH);
      digitalWrite(wav1,HIGH);
    }
    
} 


//------------------------------------------------------------
/*
void vibrate(unsigned int mil){
  int timer = millis();
  digitalWrite(vibro, HIGH);
  while (millis()-timer<mil);
  digitalWrite(vibro, LOW);
}
*/
