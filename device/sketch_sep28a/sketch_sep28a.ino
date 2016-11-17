#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.
MFRC522::MIFARE_Key key;

int i;
int newtag[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int data1 = 0;

volatile int NbTopsFan; //measuring the rising edges of the signal
volatile int Volume;
int Calc;
int Vol; int iter;
int hallsensor = 2; //The pin location of the sensor
void rpm () //This is the function that the interupt calls 
{ 
//NbTopsFan++; //This function measures the rising and falling edge of the hall effect sensors signal
Volume++;

} 


void setup()
{
 
  Serial.begin(9600);  // start serial to PC 
  iter=0;
  pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
//digitalWrite(hallsensor, HIGH); // turn on pullup resistors

attachInterrupt(0, rpm, RISING); //and the interrupt is attached

SPI.begin();               // Init SPI bus
        mfrc522.PCD_Init();        // Init MFRC522 card
              
        // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
        for (byte i = 0; i < 6; i++) {
                key.keyByte[i] = 0xFF;
        }
}


void loop()
{
  NbTopsFan = 0; //Set NbTops to 0 ready for calculations
sei(); //Enables interrupts
delay (1000); //Wait 1 second
cli(); //Disable interrupts
//Calc = (NbTopsFan * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour 
//Vol = Volume / 7.5;
  
Serial.print (iter++, DEC); Serial.print(";"); 


        // Look for new cards, and select one if present
        if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
            delay(50);
           // return;
        }
        
        // Now a card is selected. The UID and SAK is in mfrc522.uid.
        
        // Dump UID
        //Serial.print(F("Card UID:"));
        for (byte i = 0; i < mfrc522.uid.size; i++) {
                Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                Serial.print(mfrc522.uid.uidByte[i], HEX);
        } 
       Serial.print(";");
  
  


//for (int z = 0 ; z < 14 ; z++) // read the rest of the tag
  //  {Serial.print(newtag[z], DEC);    }Serial.print(";"); 
    
Serial.print (Volume, DEC); //Prints the number calculated above
Serial.println (";\r\n"); 
Volume=0;
Serial.println("");
}


