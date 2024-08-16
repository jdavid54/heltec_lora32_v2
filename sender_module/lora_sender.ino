/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/
/*
ESP32 Chip model = ESP32-D0WDQ6 Rev 300
This chip has 2 cores
ESP32ChipID=40159F842178


id : 40159f842178	
license : 0x0657D7F9,0xF3709C1E,0x52EE464A,0x125E1167	
Production : Chngdu	
Manufacture Data : 2022/05/09 09:33:57
*/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27

#define CS 18
#define RST 14
#define IRQ 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3c
//packet counter
int counter = 0;

// activation de l'affichage
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

//////////////////////////////////////SETUP////////////////////////////////////////////////////////
void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER ");
  display.display();
  
  Serial.println("LoRa Sender Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, CS);
  //setup LoRa transceiver module
  LoRa.setPins(CS, RST, IRQ);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
    // Set Lora parameters  
  //LoRa.setTxPower(17, PA_OUTPUT_PA_BOOST_PIN);//Supported values are between 2 and 17 for PA_OUTPUT_PA_BOOST_PIN
  //LoRa.setSpreadingFactor(10);
  //LoRa.setSignalBandwidth(62.5E3);
  //LoRa.setCodingRate4(7); //min 5- max 8
  //setPreambleLength(long length);
  //setSyncWord(int sw);
  //LoRa.enableCrc();
  // LoRa.disableCrc();
  //enableInvertIQ();
  //disableInvertIQ()  
  }
  
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);
}
///////////////////////////LOOP//////////////////////////////////////////////
void loop() {
   
  Serial.print("Sending packet: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("Packet received #");
  LoRa.print(counter);
  LoRa.endPacket();
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("LORA SENDER");
  display.setCursor(0,20);
  display.setTextSize(1);
  display.print("LoRa packet sent.");
  display.setCursor(0,30);
  display.print("Counter:");
  display.setCursor(50,30);
  display.print(counter);      
  display.display();

  counter++;
  
  delay(10000);
}
