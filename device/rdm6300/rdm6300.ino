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

#include <SoftwareSerial.h>
#include <avr/pgmspace.h>
//----
// Tags are stored in program flash memory
// 32k minus sketch size determines the amount of tags that can be stored
// Tags include the two CRC bytes (14 bytes total)
prog_char tag_0[] PROGMEM = "000000000000"; //add your tags here
prog_char tag_1[] PROGMEM = "000000000000";
prog_char tag_2[] PROGMEM = "000000000000";
prog_char tag_3[] PROGMEM = "000000000000";
prog_char tag_4[] PROGMEM = "000000000000";
prog_char tag_5[] PROGMEM = "000000000000";
prog_char tag_6[] PROGMEM = "000000000000";

PROGMEM const char *tag_table[] =
{   
  tag_0,
  tag_1,
  tag_2,
  tag_3,
  tag_4,
  tag_5,
  tag_6 };
//----

SoftwareSerial rfidReader(2,3); // Digital pins 2 and 3 connect to pins 1 and 2 of the RMD6300
String tagString;
char tagNumber[14];
boolean receivedTag;
int lockPIN=7; // pin 7 is controls the door

void setup() {
  
  pinMode(lockPIN,OUTPUT);
  Serial.begin(9600);
  rfidReader.begin(9600); // the RDM6300 runs at 9600bps
  Serial.println("\n\n\nRFID Reader...ready!");
 
}

void loop()
{
  receivedTag=false;
  while (rfidReader.available()){
    int BytesRead = rfidReader.readBytesUntil(3, tagNumber, 15);//EOT (3) is the last character in tag 
    receivedTag=true;
  }  
 
  if (receivedTag){
    tagString=tagNumber;
    Serial.println();
    Serial.print("Tag Number: ");
    Serial.println(tagString);
    
    if (checkTag(tagString)){
      Serial.print("Tag Authorized...");
      openDoor();
    }
    else{
      Serial.print("Unauthorized Tag: ");
      Serial.println(tagString);
      delay(1500); // a delay of 1500ms and a flush() seems to stop tag repeats
      rfidReader.flush();
    }
    memset(tagNumber,0,sizeof(tagNumber)); //erase tagNumber
  }
    
}

// ----
// checkTag function (give it the tag string complete with SOT and EOT)
// compares with tags in tag_table
// and returns true if the tag is in the list

 boolean checkTag(String tag){
   char testTag[14];
   
   for (int i = 0; i < sizeof(tag_table)/2; i++)
  {
    strcpy_P(testTag, (char*)pgm_read_word(&(tag_table[i])));
    if(tag.substring(1,13)==testTag){//substring function removes SOT and EOT
      return true;
      break;
    }
  }
   return false;
 }
 //----
 
 void openDoor(){
  Serial.print("Opening door...");
  digitalWrite(lockPIN,HIGH);
  delay(1000);
  Serial.println("Re-locking door");
  digitalWrite(lockPIN,LOW);
  delay(1500);// a delay of 1500ms and a flush() seems to stop tag repeats
  rfidReader.flush();
}


