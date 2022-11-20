
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
#define SS 18
#define RST 14
#define DIO0 26


//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 915E6

//OLED pins
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
int buttonp =0;
String tof =".000000956"; 

//packet counter
int counter = 0;
String message;
String outgoing;    
byte localAddress = 0xaf;     // address of this device
byte destination = 0xff;      // destination to send to
int key =1;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void Button()
{
  buttonp =1;
}
void setup() {

  //initialize Serial Monitor

  Serial.begin(115200);

  //reset OLED display via software

  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  attachInterrupt(38,Button,FALLING);

  //initialize OLED

  Wire.begin(OLED_SDA, OLED_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

 

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER ");
  display.display();

 

 // Serial.println("LoRa Sender Test");

 

  //SPI LoRa pins

  SPI.begin(SCK, MISO, MOSI, SS);

  //setup LoRa transceiver module

  LoRa.setPins(SS, RST, DIO0);

 

  if (!LoRa.begin(BAND)) {

    //Serial.println("Starting LoRa failed!");

    while (1);

  }

  LoRa.setSyncWord(77);
  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(10);
  //LoRa.setSignalBandwidth(125E3);

  //Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);

}

 

void loop() {
  if(Serial.available()>0)
  {
    Serial.print("Sending packet: ");
    Serial.println(counter);
    message = Serial.readStringUntil('\n');

  //Send LoRa packet to receiver

    Serial.print("Message: ");
    Serial.println(message);

    String result = message;
 


    sendMessage(message);
    
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.print("Message Number:");
    display.setCursor(100,0);
    display.print(counter);
    display.setCursor(0,10);
    display.print("Message:");
    display.setCursor(55,10);
    display.print(message);      
    display.display();
  }
  onReceive(LoRa.parsePacket());

}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  //LoRa.write(counter);                 // add message ID
  //LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  //counter++;       
  //Serial.println("sent");                    // increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  //byte incomingMsgId = LoRa.read();     // incoming msg ID
 // byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  // if (incomingLength != incoming.length()) {   // check length for error
  //   Serial.println("error: message length does not match length");
  //   return;                             // skip rest of function
  // }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xcb) {
    //Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  Serial.print("0x" + String(sender, HEX));
  //Serial.println("Sent to: 0x" + String(recipient, HEX));
 // Serial.println("Message ID: " + String(incomingMsgId));
  //Serial.println("Message length: " + String(incomingLength));
  Serial.println("," + incoming);
  //Serial.println("RSSI: " + String(LoRa.packetRssi()));
  //Serial.println("Snr: " + String(LoRa.packetSnr()));
  //Serial.println();
  //Serial.println(incoming);
   
    display.clearDisplay();
    display.setCursor(0,30);
    display.setTextSize(1);
    display.print("Message: " + incoming);
    display.setCursor(0,40);
    display.print("Received from: 0x" + String(sender, HEX));
    
    display.display();

}
