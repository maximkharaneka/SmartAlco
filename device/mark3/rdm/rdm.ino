
#include <SoftwareSerial.h>
SoftwareSerial RFID(4, 3); // RX and TX
int newtag[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int data1 = 0;
int iter = 0;
volatile int Volume;


void setup()
{
  RFID.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
  
  pinMode(2, INPUT); //initializes digital pin 2 as an input
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached
  
  pinMode(9, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(5, OUTPUT);
  
  for (int z = 0 ; z < 14 ; z++) // read the rest of the tag
    {
      newtag[z] = 0;
    }
    
    
}
 
void loop()
{
  
counter();
rfid();
vol();
knob();
Serial.println (";\r\n"); 
}

void rpm () 
{ 
Volume++;
} 

void counter(){
  Serial.print (iter++, DEC); 
  Serial.print(";"); 
}


void rfid()
{
  if (RFID.available() > 0) 
  {
    //Serial.println("aviable"); 
    delay(100); // needed to allow time for the data to come in from the serial buffer.
    for (int z = 0 ; z < 14 ; z++) // read the rest of the tag
    {
      data1 = RFID.read();
      newtag[z] = data1;
    }
    RFID.flush(); // stops multiple reads
  }  
  for (int z = 0 ; z < 14 ; z++) // read the rest of the tag
{Serial.print(newtag[z], DEC);}
Serial.print(";"); 
}
void vol(){
  Volume=0;
  sei(); 
  delay (1000);
  cli(); 
  Serial.print (Volume, DEC); 
}
void knob(){
  int sensorValue = analogRead(A0); 
  if(sensorValue>10){
    Serial.print(";");
    Serial.print(sensorValue); 
  }
}
