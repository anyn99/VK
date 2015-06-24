Sending Music from one AVakai to another.

MusicTestApp is an Smartphone App to simulate the sending Avakai for debugging.

Sent datastream is as follow:

sendarray[0] = 0x0F   //to indicate it is a music message
sendarray[1] = 1 byte // holds first note to play, currently only notes 1-3
sendarray[2] = 1 byte // holds LSB of key press duration (how long light is switched on)
sendarray[3] = 1 byte // holds MSB of key press duration 
sendarray[4] = 1 byte // holds LSB of pause until next note is played
sendarray[5] = 1 byte // holds MSB of pause until next note is played
sendarray[6] = 1 byte // holds second note to be played
---and so on

Due to limitation of RFduino's bluetooth stack, it is only possible to send and immediatley play up to 4 notes in a row in this way, longer arrays get stuck in the buffer and lead to weird behaviour.
For sending longer melodies, the code has to be adopted to send continously arrays.