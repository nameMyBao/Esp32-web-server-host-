#include <WiFi.h>
#include <WifiClient.h>
#include <WebServer.h>
#include <ESP32Servo.h>

/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);
Wifi.softAP(ssid, password);
Wifi.softAPConfig(local_ip, gateway, subnet);


Servo nigga;
static const int servo1Pin = 14;
String header;

String valueString = String(5);
int pos1 = 0;
int pos2 = 0;


void setup() {
  Serial.begin(115200);
  nigga.attach(servo1Pin);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
    server.on("/", handle_OnConnect);
          server.on("/90_degree", handle_90_degree);
          server.on("/180_degree", handle_180_degree);
          server.onNotFound(handle_NotFound);   
        
  
  server.begin();
  Serial.println("HTTP_server_started");

}
void loop() {
  server.handleClient();
   servo1.write(angle);
    delay(20);
 }  

void handleServo() {
  if(server.arg("do") == "s1less" )
  {
    angle -=1;
    
      if(angle <= angle_min)
      {
        angle = angle_min;
      }
  }else 
  {
    angle +=1;   
     
      if(angle >= angle_max)
      {
        angle = angle_max;
      } 
  }
  handleRoot();
}


void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>SERVO Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>LED1 Status: 90</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: 0</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: 180</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: 0</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}