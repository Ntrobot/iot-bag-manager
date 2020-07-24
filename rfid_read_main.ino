#include <SPI.h> // for communication with the module
#include <MFRC522.h> // import library for the RFID module
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define SS_PIN 21
#define RST_PIN 22
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 
/*
byte FoundTag; // Variable to check if a card was found
byte ReadTag; // Variable used to store anti-collision value
byte TagSerialNumber[MAX_LEN]; //Store full UID
*/

// Init array that will store new NUID 
byte nuidPICC[7];

//item IDs
byte PencilCaseID[4] = {0x03, 0x52, 0xCB, 0x21};
byte WorksheetFolderID[4] = {0x11, 0x4B, 0xCE, 0x2E};
byte MathsNotebookID[4] = {0x04, 0x8E, 0xAC, 0xCA};
byte MathsTextbookID[4] = {0x04, 0x46, 0x31, 0xCA};
byte ScienceNotebookID[4] = {0x04, 0x26, 0x31, 0xCA};
byte JapaneseTextbookID[4] = {0x04, 0xE9, 0x31, 0xCA};
byte PEKitID[4] = {0x9A, 0x12, 0x54, 0x83};

//item bools
bool PencilCase = false;
bool WorksheetFolder = false;
bool MathsNotebook = false;
bool MathsTextbook = false;
bool ScienceNotebook = false;
bool JapaneseTextbook = false;
bool PEKit = false;

/*
//Day indicators
WidgetLED Monday_indicator(V1);
WidgetLED Tuesday_indicator(V2);
WidgetLED Wednesday_indicator(V3);
WidgetLED Thursday_indicator(V4);
WidgetLED Friday_indicator(V5);
*/

//Setup for pixel LEDs

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            16

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      8

//Brightness of the LED show
#define BRIGHTNESS 5

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 70; // delay for half a second

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


//Wifi Setup
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "a0037d9acdbe4c9297c0bc778e795239";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "wx03-dca77b";
char pass[] = "b37sdDQa";


//Time Table
int table_delay = 200;
  BLYNK_WRITE(V2) {
  switch (param.asInt())
  {
    case 1: // Item 1 - Monday
      Serial.println("Item 1 selected");
      Blynk.virtualWrite(V1, "clr");
      Blynk.virtualWrite(V1, "add", 2, "Pencil Case", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 1, "Worksheet Folder", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 6, "Japanese Textbook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 7, "PE Kit", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 3, "Maths Textbook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 4, "Maths Notebook", "Missing");
      //Things that are not needed
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 5, "Science Notebook", "Missing");
      Blynk.virtualWrite(V1, "deselect", 5);      
      break;    
    case 2: // Item 2 Tuesday
      Serial.println("Item 2 selected");
      Blynk.virtualWrite(V1, "clr");
      Blynk.virtualWrite(V1, "add", 2, "Pencil Case", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 1, "Worksheet Folder", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 5, "Science Notebook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 3, "Maths Textbook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 4, "Maths Notebook", "Missing");
      //Things that are not needed
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 6, "Japanese Textbook", "Missing");
      Blynk.virtualWrite(V1, "deselect", 6);
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 7, "PE Kit", "Missing");
      Blynk.virtualWrite(V1, "deselect", 7); 
      break;
    case 3: // Item 3 Wednesday
      Serial.println("Item 3 selected");
      Blynk.virtualWrite(V1, "clr");
      Blynk.virtualWrite(V1, "add", 2, "Pencil Case", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 1, "Worksheet Folder", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 6, "Japanese Textbook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 3, "Maths Textbook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 4, "Maths Notebook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 5, "Science Notebook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 7, "PE Kit", "Missing");
      break;
    case 4: // Item 4 Thursday
      Serial.println("Item 4 selected");
      Blynk.virtualWrite(V1, "clr");
      Blynk.virtualWrite(V1, "add", 2, "Pencil Case", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 1, "Worksheet Folder", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 3, "Maths Textbook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 4, "Maths Notebook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 5, "Science Notebook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 6, "Japanese Textbook", "Missing");
      //Things that are not needed
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 7, "PE Kit", "Missing");
      Blynk.virtualWrite(V1, "deselect", 7);
      break;
    case 5: // Item 5 Friday
      Serial.println("Item 5 selected");
      Blynk.virtualWrite(V1, "clr");
      Blynk.virtualWrite(V1, "add", 2, "Pencil Case", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 1, "Worksheet Folder", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 3, "Maths Textbook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 4, "Maths Notebook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 6, "Japanese Textbook", "Missing");
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 5, "Science Notebook", "Missing");
      //Things that are not needed
      delay(table_delay);
      Blynk.virtualWrite(V1, "add", 7, "PE Kit", "Missing");
      Blynk.virtualWrite(V1, "deselect", 7);
      break;
    default:
      Serial.println("Unknown item selected");
      Blynk.virtualWrite(V1, "clr");
  }
}

void setup() {  
  //Initialise communication
  SPI.begin();
  Serial.begin(9600);
  rfid.PCD_Init(); // Init MFRC522
  Blynk.begin(auth, ssid, pass);
 
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

  //Setup for neopixel LEDs
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.

  //Begin LED Animation
  delay(1500);
  LED_ANIMATION();

  delay(1000);

}

void loop() {

/* For the second RFID library
  // Check to see if a Tag was detected
  // If yes, then the variable FoundTag will contain "MI_OK"
  FoundTag = nfc.requestTag(MF1_REQIDL, TagData);

  if (FoundTag == MI_OK) {
    delay(200);
  }
  //Get anti collision value - the original ID to read the information properly
  ReadTag = nfc.antiCollision(TagData);
  memcpy(TagSerialNumber, TagData, 4); //Copy the information in the full id variable

  //Print the card's information
  Serial.println("Tag detected.");
  Serial.print("Serial Number: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(TagSerialNumber[i], HEX);
    Serial.print(", ");
  }
  Serial.println("");
  Serial.print();
*/
  //Blynk widgets
  Blynk.run();

/*
  //Monday
  Blynk.virtualWrite(V1, "add", 0, "Monday", " ");
  Blynk.virtualWrite(V1, "add", 1, "Japanese Textbook", "Missing");
*/

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  //Print Card Information
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

    if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    //Comparing the stored UIDs with scanned UID
    //Pencil case
  for(int i=0; i < 4; i++){
    if (PencilCaseID[i] != nuidPICC[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      PencilCase = not PencilCase;
      LED_FLASH_Pencil_Case();
      if (PencilCase == true){
        Blynk.virtualWrite(V1, "update", 2, "Pencil Case", "Included");
        Blynk.virtualWrite(V1, "deselect", 2);
      }
      else{
        Blynk.virtualWrite(V1, "update", 2, "Pencil Case", "Missing");
      }
      Serial.println("Pencil case found");
    } 
  }
  
  //WorksheetFolder
  for(int i=0; i < 4; i++){
    if (WorksheetFolderID[i] != nuidPICC[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      WorksheetFolder = not WorksheetFolder;
      LED_FLASH_WorksheetFolder();
      if (WorksheetFolder == true){
        Blynk.virtualWrite(V1, "update", 1, "Worksheet Folder", "Included");
        Blynk.virtualWrite(V1, "deselect", 1);
      }
      else{
        Blynk.virtualWrite(V1, "update", 1, "Worksheet Folder", "Missing");
      }
      Serial.println("Worksheet folder found");
    } 
  }

  //MathsNotebook
  for(int i=0; i < 4; i++){
    if (MathsNotebookID[i] != nuidPICC[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      MathsNotebook = not MathsNotebook;
      LED_FLASH_MathsNotebook();
      if (MathsNotebook == true){
        Blynk.virtualWrite(V1, "update", 4, "Maths Notebook", "Included");
        Blynk.virtualWrite(V1, "deselect", 4);
      }
      else{
        Blynk.virtualWrite(V1, "update", 4, "Maths Notebook", "Missing");
      }
      Serial.println("Maths notebook Found");
    } 
  }
  
  //MathsTextbook
  for(int i=0; i < 4; i++){
    if (MathsTextbookID[i] != nuidPICC[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      MathsTextbook = not MathsTextbook;
      LED_FLASH_MathsNotebook();
      if (MathsTextbook == true){
        Blynk.virtualWrite(V1, "update", 3, "Maths Textbook", "Included");
        Blynk.virtualWrite(V1, "deselect", 3);
      }
      else{
        Blynk.virtualWrite(V1, "update", 3, "Maths Textbook", "Missing");
      }
      Serial.println("Maths textook found");
    } 
  }
  
  //ScienceNotebook
  for(int i=0; i < 4; i++){
    if (ScienceNotebookID[i] != nuidPICC[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      ScienceNotebook = not ScienceNotebook;
      LED_FLASH_ScienceNotebook();
      if (ScienceNotebook == true){
        Blynk.virtualWrite(V1, "update", 5, "Science Notebook", "Included");
        Blynk.virtualWrite(V1, "deselect", 5);
      }
      else{
        Blynk.virtualWrite(V1, "update", 5, "Science Notebook", "Missing");
      }
      Serial.println("Science notebook found");
    } 
  }
  
  //JapaneseTextbook
  for(int i=0; i < 4; i++){
    if (JapaneseTextbookID[i] != nuidPICC[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      JapaneseTextbook = not JapaneseTextbook;
      LED_FLASH_JapaneseTextbook();
      
      if (JapaneseTextbook == true){
        Blynk.virtualWrite(V1, "update", 6, "Japanese Textbook", "Included");
        Blynk.virtualWrite(V1, "deselect", 6);
      }
      else{
        Blynk.virtualWrite(V1, "update", 6, "Japanese Textbook", "Missing");
      }

      Serial.println("Japanese textbook found");
    } 
  }
  //PEKit
    for(int i=0; i < 4; i++){
    if (PEKitID[i] != nuidPICC[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      PEKit = not PEKit;
      LED_FLASH_PEKit();
      
      if (PEKit == true){
        Blynk.virtualWrite(V1, "update", 7, "PE Kit", "Included");
        Blynk.virtualWrite(V1, "deselect", 7);
      }
      else{
        Blynk.virtualWrite(V1, "update", 7, "PE Kit", "Missing");
      }

      Serial.println("PE Kit found");
    } 
    }  
  }
  else Serial.println(F("Card read previously."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  //Compare card id with saved ids
  // Check if detected Tag has the right Serial number we are looking for 
  /* For the second RFID library
  //Pencil case
  for(int i=0; i < 4; i++){
    if (PencilCaseID[i] != TagSerialNumber[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      PencilCase = boolean_alternator(PencilCase);
      LED_FLASH(1);
      Serial.println("Pencil case found")
    } 
  }
  
  //WorksheetFolder
  for(int i=0; i < 4; i++){
    if (WorksheetFolderID[i] != TagSerialNumber[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      WorksheetFolder = boolean_alternator(WorksheetFolder);
      LED_FLASH(2);
      Serial.println("Worksheet folder found")
    } 
  }

  //MathsNotebook
  for(int i=0; i < 4; i++){
    if (MathsNotebookID[i] != TagSerialNumber[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      MathsNotebook = boolean_alternator(MathsNotebook);
      LED_FLASH(3);
      Serial.println("Maths notebook Found")
    } 
  }
  
  //MathsTextbook
  for(int i=0; i < 4; i++){
    if (MathsTextbookID[i] != TagSerialNumber[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      MathsTextbook = boolean_alternator(MathsTextbook);
      LED_FLASH(4);
      Serial.println("Maths textook found")
    } 
  }
  
  //ScienceNotebook
  for(int i=0; i < 4; i++){
    if (ScienceNotebookID[i] != TagSerialNumber[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      ScienceNotebook = boolean_alternator(ScienceNotebook);
      LED_FLASH(5);
      Serial.println("Science notebook found")
    } 
  }
  
  //JapaneseTextbook
  for(int i=0; i < 4; i++){
    if (JapaneseTextbookID[i] != TagSerialNumber[i]) {
      break; // if not equal, then break out of the "for" loop
    }
    if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
      JapaneseTextbook = boolean_alternator(JapaneseTextbook);
      LED_FLASH(6);
      Serial.println("Japanese textbook found")
    } 
  }
  */

  /*
  //Blynk virtual pin
  BLYNK_WRITE(V1) { //Button Widget is writing to pin V1 and requests a bag check
  int pinData = param.asInt();
  check_bag()
  }
  */
} 

/*
void boolean_alternator(content_state){ //for alternating from true to false or vice versa - needed for content state
  bool content_state;
  if (content_state == false){
    return true;
  }
  else{
    return false;
  }
}
*/

/*
void check_bag(){ //gets called when requested by Blynk to check the bag.
  if (PencilCase and WorksheetFolder and MathsNotebook and MathsTextbookID and JapaneseTextbookID){
    Monday_indicator.on();
    break;
  }
  else{
    Monday_indicator.off();
  }
  if (PencilCase and WorksheetFolder and MathsNotebook and MathsTextbookID and JapaneseTextbookID){
    Tuesday_indicator.on()
    break;
  }
  else{
    Tuesday_indicator.off()
  }
}
*/

//LED Show codes
void LED_FLASH_Pencil_Case(){
    for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,100,0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
  delay(800);
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}
void LED_FLASH_WorksheetFolder(){
    for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(80,80,80)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
  delay(800);
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); 

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}
void LED_FLASH_MathsNotebook(){
    for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,42,62)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
  delay(800);
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); 

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}
void LED_FLASH_ScienceNotebook(){
    for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(82,36,0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
  delay(800);
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); 

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}
void LED_FLASH_JapaneseTextbook(){
    for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(77,6,31)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
  delay(800);
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); 

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}
void LED_FLASH_PEKit(){
    for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(100,0,0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
  delay(800);
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}

//Blynk reset and unpack button
BLYNK_WRITE(V3) { //Button Widget is writing to pin V3

  int pinData = param.asInt();
  //Make all boos false
  PencilCase = false;
  WorksheetFolder = false;
  MathsNotebook = false;
  MathsTextbook = false;
  ScienceNotebook = false;
  JapaneseTextbook = false;
  PEKit = false;
  //Initialise table
  Blynk.virtualWrite(V1, "clr");
  //LED SHOW
  delay(50);
  LED_ANIMATION();
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}


//_______________________________________________
//Code for LED light show
//Call Function
void LED_ANIMATION(){
  colorWipe(pixels.Color(255, 0, 0), 50); // Red
  colorWipe(pixels.Color(0, 255, 0), 50); // Green
  colorWipe(pixels.Color(0, 0, 255), 50); // Blue
  colorWipe(pixels.Color(0, 0, 0, 255), 50); // White

  whiteOverRainbow(20,75,5);  

  //pulseWhite(5); 

  // fullWhite();
  // delay(2000);

  //rainbowFade2White(3,3,1);

  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); 

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}

void pulseWhite(uint8_t wait) {
  for(int j = 0; j < 256 ; j++){
      for(uint16_t i=0; i<pixels.numPixels(); i++) {
          pixels.setPixelColor(i, pixels.Color(0,0,0, neopix_gamma[j] ) );
        }
        delay(wait);
        pixels.show();
      }

  for(int j = 255; j >= 0 ; j--){
      for(uint16_t i=0; i<pixels.numPixels(); i++) {
          pixels.setPixelColor(i, pixels.Color(0,0,0, neopix_gamma[j] ) );
        }
        delay(wait);
        pixels.show();
      }
}


void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for(int k = 0 ; k < rainbowLoops ; k ++){
    
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel

      for(int i=0; i< pixels.numPixels(); i++) {

        wheelVal = Wheel(((i * 256 / pixels.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);

        pixels.setPixelColor( i, pixels.Color( redVal, greenVal, blueVal ) );

      }

      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) {
          fadeVal++;
      }

      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
          fadeVal--;
      }

        pixels.show();
        delay(wait);
    }
  
  }



  delay(500);


  for(int k = 0 ; k < whiteLoops ; k ++){

    for(int j = 0; j < 256 ; j++){

        for(uint16_t i=0; i < pixels.numPixels(); i++) {
            pixels.setPixelColor(i, pixels.Color(0,0,0, neopix_gamma[j] ) );
          }
          pixels.show();
        }

        delay(2000);
    for(int j = 255; j >= 0 ; j--){

        for(uint16_t i=0; i < pixels.numPixels(); i++) {
            pixels.setPixelColor(i, pixels.Color(0,0,0, neopix_gamma[j] ) );
          }
          pixels.show();
        }
  }

  delay(500);


}

void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) {
  
  if(whiteLength >= pixels.numPixels()) whiteLength = pixels.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;


  while(true){
    for(int j=0; j<256; j++) {
      for(uint16_t i=0; i<pixels.numPixels(); i++) {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
          pixels.setPixelColor(i, pixels.Color(0,0,0, 255 ) );
        }
        else{
          pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
        }
        
      }

      if(millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if(head == pixels.numPixels()){
          loopNum++;
        }
        lastTime = millis();
      }

      if(loopNum == loops) return;
    
      head%=pixels.numPixels();
      tail%=pixels.numPixels();
        pixels.show();
        delay(wait);
    }
  }
  
}
void fullWhite() {
  
    for(uint16_t i=0; i< pixels.numPixels(); i++) {
        pixels.setPixelColor(i, pixels.Color(0,0,0, 255 ) );
    }
      pixels.show();
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}

