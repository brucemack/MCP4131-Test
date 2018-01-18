// This is a test program for the MCP4131 digital potentiometer
// Bruce MacKinnon KC1FSZ 15 January 2018
//
// Please see: http://ww1.microchip.com/downloads/en/DeviceDoc/22060a.pdf
//
// NOTE: I've been having a lot of problems getting this chip to work using
// the SPI library on the Teensy 3.2.  I'm not completely sure what the issue 
// is, but when I manipulate the pins to simulate the SPI sequence it works 
// perfectly.  
//
// CHANGE THESE ACCORDING TO WHAT PINS YOU ARE USING:
//
const int ssPin = 10;
const int mosiPin = 11;
const int clkPin = 13;

void setup() {

  Serial.begin(9600);
  delay(50);
 
  pinMode (ssPin,OUTPUT);
  pinMode (mosiPin,OUTPUT);
  pinMode (clkPin,OUTPUT);

  digitalWrite(ssPin,HIGH); 
  digitalWrite(clkPin,HIGH); 
}

// This is just a simple loop that sequences the pot through the full range of 
// values.  Address 0x00 on the MCP4131 is the potentiometer value. 
//
void loop() {
  for (int i = 0; i < 128; i++) {
   Serial.println(i);    
   spiWrite(0,i);
   delay(500);
  }
}

void spiWriteBit(int bit) {
  // Setup the data
  digitalWrite(mosiPin,bit);
  // The slave samples when clock goes low
  digitalWrite(clkPin,0);
  digitalWrite(clkPin,1);
}

void spiWriteByte(int b) {
  int work = b;
  for (int i = 0; i < 8; i++) {
    // Always focus on the MSB 
    spiWriteBit((work & 0x80) ? 1 : 0);
    work = work << 1;
  }
}

// NOTE: There are no delays of any kind.  The assumption is that the chip 
// can keep up with the maxium data rate.
//
void spiWrite(int address, int value) {
  // Take the SS pin low to select the chip:
  digitalWrite(ssPin,LOW);
  // Address
  spiWriteByte(address);
  // Data
  spiWriteByte(value);
  // Take the SS pin high to de-select the chip:
  digitalWrite(ssPin,HIGH); 
}


