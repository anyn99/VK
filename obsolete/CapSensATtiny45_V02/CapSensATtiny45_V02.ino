
//SDA = PB0
//SCL = PB2
/*
PinOut ATtiny45:
Pin5 = Ardu 0 = PB0
Pin6 = Ardu 1 = PB1
Pin7 = Ardu 2 = PB2
Pin2 = Ardu 3 = PB3
Pin3 = Ardu 4 = PB4
*/


#include <TinyWireS.h>


#define I2C_SLAVE_ADDRESS 0x26 // the 7-bit address

byte buffer[16];
byte pos=0;

short value1=0;
short value2=0;

short val1=0;
short val2=0;

/**
 * This is called for each read request we receive, never put more than one byte of data (with TinyWireS.send) to the 
 * send-buffer when using this callback
 */
void requestEvent()
{
     
    TinyWireS.send(buffer[pos]);
    // Increment the position on each read, and loop back to zero
    pos++;
    if (pos >= sizeof(buffer))
    {
        pos = 0;
    }
   
}


/**
 * The I2C data received -handler
 *
 * This needs to complete before the next incoming transaction (start, data, restart/stop) on the bus does
 * so be quick, set flags for long running tasks to be called from the mainloop instead of running them directly,
 */


void setup()
{
    memset(buffer,0,sizeof(buffer)); //set buffer to zero
    
    TinyWireS.begin(I2C_SLAVE_ADDRESS);
    TinyWireS.onRequest(requestEvent);

}



void loop() {
  
    
    
   val1 =  CapSens(1,3,30); //(SendPin,ReceivePin,samples)
   val2 =  CapSens(1,4,30);
   
   value1 = 0x4321;
   value2 = 0x8765;
   // write values in buffer
   // first value1 in buffer[0-1], then value2 in buffer[2-3]
   buffer[0] = (byte) (value1 & 0xff);         //lowbyte
   buffer[1] = (byte) ((value1 >> 8) & 0xff);  //highbyte
   buffer[2] = (byte) (value2 & 0xff);
   buffer[3] = (byte) ((value2 >> 8) & 0xff);
    
 
    //TinyWireS_stop_check();
}
