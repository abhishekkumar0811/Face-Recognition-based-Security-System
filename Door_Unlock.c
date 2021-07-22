#include<string.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);

String ssid     = "Simulator Wifi";  // SSID to connect to
String password = ""; // Our virtual wifi has no password
String host     = "api.thingspeak.com"; // Open API
const int httpPort   = 80;
String uri     = "/channels/1377057/fields/1.json?api_key=Z15ECQBO0MARG8S5&results=2";
int motor = 10;
int buz = 9;


int setupESP8266(void) {
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 1;
    
  // Connect to 123D Circuits Simulator Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 2;
  
  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 3;
  
  return 0;
}

void anydata(void) {
  
  
  // Construct our HTTP call
  String httpPacket = "GET " + uri + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();
  
  // Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10); // Wait a little for the ESP to respond if (!Serial.find(">")) return -1;

  // Send our http request
  Serial.print(httpPacket);
  delay(10); // Wait a little for the ESP to respond
  String ch="";
  ch = Serial.readString();
  Serial.println(ch);
  int len=ch.length();
  int point  = len-7;
  Serial.println(ch[point]);
  if(ch[point]=='y')
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("Welcome");
    Serial.println("Welcome");
    digitalWrite(motor,HIGH);
    delay(2000);
    digitalWrite(motor,LOW);
  }
  else if(ch[point]=='n')
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("Access denied");
    Serial.println("Access denied");
    digitalWrite(buz,HIGH);
    delay(2000);
    digitalWrite(buz,LOW);
  }
  else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("Secure Home");
    Serial.println("Secure Home");
    digitalWrite(buz,LOW);
  }
    
  if (!Serial.find("SEND OK\r\n")) return;
  
  
}


void setup() {
  
  lcd.begin(16,2);
  setupESP8266();
  pinMode(motor,OUTPUT);
  pinMode(buz,OUTPUT);
               
}

void loop() {
  anydata();  
  delay(5000);
}