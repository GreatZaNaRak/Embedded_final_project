#include <MicroGear.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

// constants won't change. They're used here to 
// set pin numbers:
#define RXpin D7
#define TXpin D8

const char* ssid     = "PIE";
const char* password = "pichaya0001";
String data;

#define APPID   "STM32"
#define KEY     "1BdMxLiTzKNQFY0"
#define SECRET  "xQ8n3AztOekNi4tytfQ13uDgT"

#define ALIAS   "Budda"
#define TargetWeb "Target_web"

WiFiClient client;
MicroGear microgear(client);
SoftwareSerial uart(RXpin, TXpin);

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) 
{
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
    char res[msglen];
    for (int i = 0; i < msglen; i++){
      res[i] = (char)msg[i];
    }
    String result = String(res).substring(0, msglen);
    //Serial.println(result);
    if(result == "Y"){
      uart.write("Y");
    }
    else if(result == "O") {
      uart.write("O");
    }
    else if(result == "R") {
      uart.write("R");
    }
    else if(result == "G") {
      uart.write("G");
    }
    
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) 
{
  Serial.println("Connected to NETPIE...");
  microgear.setAlias(ALIAS);
}

void setup() 
{
    /* Event listener */
    microgear.on(MESSAGE,onMsghandler);
    microgear.on(CONNECTED,onConnected);

    Serial.begin(115200);
    uart.begin(115200);
    Serial.println("Starting...");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
       delay(250);
       Serial.print(".");
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    microgear.init(KEY,SECRET,ALIAS);
    microgear.connect(APPID);
}

void loop() 
{
  if(microgear.connected()) 
  {
    microgear.loop();
    Serial.println("connect...");
    data = uart.readString();
    microgear.chat(TargetWeb,data);
    Serial.println("Send!");
  } 
  else 
  {
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
  }
  Serial.println(data);
  delay(500);
}
