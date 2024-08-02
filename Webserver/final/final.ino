#include<WiFi.h> 
#include<WebServer.h> 
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>  
#include <ESP32Servo.h>

const char* ssid = "sample"; 
const char* password = "123456789";
IPAddress local_ip (192,168,49,15); 
IPAddress gateway (192,168,49,1); 
IPAddress subnet (255,255,255,0); 

WebServer server(80); 

Servo servo;
int servoPin = 4;
int servoPosition = 90;
String LED = "off";
String STRING = "off";
String SERVO = "off";
String message= "";
uint8_t LedPin = 2; 
bool LedStatus = LOW; 
int pinCS = 5; // Attach CS to this pin, DIN to MOSI and CLK to SCK
Max72xxPanel matrix = Max72xxPanel(pinCS, 1, 1);

void event_OnConnect () { 
LedStatus = LOW; 
Serial.println("Chan So 2 ESP32 Dang Tat"); 
server.send (200, "text/html", HTML(LedStatus) ) ;

} 

void event_ledon() { 
  LedStatus = HIGH; 
  Serial.println("Chan So 2 ESP32 Dang Bat"); 
  server.send(200, "text/html", HTML(LedStatus) );
}

void event_ledoff() { 
  LedStatus = LOW; 
  Serial.println("Chan So 2 ESP32 Dang Tat"); 
  server.send(200, "text/html", HTML(LedStatus) );
}
void draw(String mes)
{       
  String mesi= mes;
  int spacer = 1;                            // This will scroll the string
  int width = 5 + spacer;                    // The font width is 5 pixels
  for ( int i = 0 ; i < width * mesi.length() + width - 1 - spacer; i++ ) {
    matrix.fillScreen(0);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < mesi.length() ) {
        matrix.drawChar(x, y, mesi[letter], 1, 0, 1);
      }
      letter--;
      x -= width;
      }

    matrix.write(); // Send bitmap to display
    delay(100);
  }


}


void event_NotFound() { 
  server. send(400, "text/html", "Not Found" );
}
void event_LED(){
  LED = "on";
  STRING="off";
  SERVO="off";
  server.send(200, "text/html", HTML(LedStatus) );
}
void event_STRING() {
  LED = "off";
  STRING = "on";
  SERVO="off";
  server.send(200, "text/html", HTML(LedStatus) );
} 
void servohandler(){

   String inputPos = server.arg("pos") ;
    int pos = inputPos.toInt();
    servoPosition = pos;
    servo.write(pos);
    Serial.println (pos);
    server.send(200, "text/plain", "OK");
    event_SERVO();

}
void event_SERVO(){
  STRING = "off";
  LED = "off";
  SERVO ="on";
  server.send(200, "text/html", HTML(LedStatus) );
}
 void dataHandler(){
  String msg = server.arg("message");   //message from POST data
  Serial.print("Received: ");          //print to serial terminal
  Serial.println(msg);
  msg.toUpperCase();                  //all incoming string is converted to uppercase since no font for small cases
  message = msg; 
  event_STRING();

}

void event_WELCOME(){
  STRING = "off";
  LED="off";
  SERVO="off";
  server.send(200, "text/html", HTML(LedStatus) );
  
}

void setup() { 

// pat your setup code he 
Serial.begin (115200); 
pinMode (LedPin, OUTPUT); 
servo.attach(servoPin); //connect the servo 
servo.write(servoPosition);

WiFi.softAP(ssid, password); 
WiFi.softAPConfig(local_ip, gateway, subnet) ; 
delay (100); 


server.on("/", event_OnConnect); 
server.on("/ledon", event_ledon); 
server.on("/ledoff", event_ledoff); 
server.on("/LED", event_LED);
server.on("/SERVO", event_SERVO);
server.on("/STRING", event_STRING);
server.on("/welcome", event_WELCOME);
server.on("/dataHandler",HTTP_POST,dataHandler);
server.on("/servohandler",HTTP_GET,servohandler);

server.onNotFound (event_NotFound); 

server.begin();
Serial.println("Server is Starting"); 

}

void loop() {
  server.handleClient();
 // int len = message.length();         //get message length
  //if(len > 100) return;  
           //limit messages to 100 characters
  draw(message);       
  delay(100);     
  if (LedStatus)
  {
    digitalWrite(LedPin,HIGH);
  }
  else
  {digitalWrite(LedPin,LOW);}
}
  
  String HTML (uint8_t led)
  {
   String str ="<!DOCTYPE html> <html>\n";
    str +="<head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; 
    str +="<title>LED Control</title>\n";
    str +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    str +=".button1 { background-color: #4CAF50; border: none; color: white; height: 60px; width: 640px;";
    str +="text-decoration: none; font-size: 30px; cursor: pointer;}";
    str +=".button2 {background-color: #527053; border: none; color: white; height: 60px; width: 640px;";
    str +="text-decoration: none; font-size: 30px; cursor: pointer; } #parent { display: flex; align-items: center;  justify-content: center;}";
    str +="body{margin-top: 50px;} h1 {color: #444444; margin: 50px auto 30px;} h3 {color: #444444; margin-bottom: 50px; }\n";
    str +=".button {display: block; width: 80px; background-color: #3498db; border: none; color: white;padding: 13px 30px; text-decoration: none;font-size: 25px; margin: 0px auto 35px; cursor: pointer;border-radius: 4px;}\n";
    str +=".button-on {background-color: red;}\n";
    str +=".button-on:active {background-color: blue; }\n";
    str +=".button-off {background-color: gray; }\n";
    str +=".button-off:active {background-color: black; }\n";
    str +=".container {background-color: #b6a6f5; width: 300px; border-radius: 10px; margin: auto;padding: 10px;}\n";
    str +="p {font-size: 14px; color: #888;margin-bottom: 10px;}\n";
    str += "</style>\n";
    str +"</head>\n";
    str +="<body><h1>ESP32 Web Server</h1>";   
    if (LED=="off" && STRING=="off" && SERVO=="off")
    {
      str +="<div id=\"parent\">";
      str +="<p><a href=\"/LED\"><button class=\"button1\">LED</button></a></p>";
      str +="<p><a href=\"/STRING\"><button class=\"button1\">STRING</button></a></p>";
      str +="<p><a href=\"/SERVO\"><button class=\"button1\">SERVO</button></a></p> </div>";
    } else if (LED=="on") {
      str +="<div id=\"parent\">";
      str +="<p><a href=\"/welcome\"><button class=\"button2\">LED</button></a></p>";
      str +="<p><a href=\"/STRING\"><button class=\"button1\">STRING</button></a></p>";
      str +="<p><a href=\"/SERVO\"><button class=\"button1\">SERVO</button></a></p> </div>";
      if (led)
      {
        str +="<p>LED Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>\n";
        }
        
        else 
        { str +="<p>LED Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>\n";}
          
       
    } else if (STRING=="on") {
      str +="<div id=\"parent\">";
      str +="<p><a href=\"/LED\"><button class=\"button1\">LED</button></a></p>";
      str +="<p><a href=\"/welcome\"><button class=\"button2\">STRING</button></a></p>";
      str +="<p><a href=\"/SERVO\"><button class=\"button1\">SERVO</button></a></p> </div>";

      str += " <p>Enter text and click Send</p>\n";
      str += "<form action='/dataHandler' method='POST'>\r\n";
      str += " <label for='message'>Message:</label> <input type='text' name='message' id='message'>  </p>\r\n ";
      str += "<input type='submit' id='send' value='Send' />\r\n";
      str += "</form> \r\n "; 
    }  

else if (SERVO=="on") {
      str +="<div id=\"parent\">";
      str +="<p><a href=\"/LED\"><button class=\"button1\">LED</button></a></p>";
      str +="<p><a href=\"/STRING\"><button class=\"button1\">STRING</button></a></p>";
      str +="<p><a href=\"/welcome\"><button class=\"button2\">SERVO</button></a></p> </div>";

      str += "<input type=\"range\" min=\"0\" max=\"180\" value=\"" + String(servoPosition) + "\" step=\"1\" onchange=\"updateServo(this.value)\" />";
      str += "<script>function updateServo(pos) { var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/servohandler?pos=' + pos, true); xhttp.send(); }</script>";
      str += "</body></html>";
    }
    str+= "</body>\n";
    str+= "</html>\n";
    return str;
  
  }
  
  
  
  
  
  
  
  
  
  
  
