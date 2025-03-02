#include <WiFi.h>
#include <Servo.h>
#include <WebServer.h>
#include <WifiClient.h>


static const int servoPin = 4;

Servo servo1;

int pos1 = 0;
int pos2 = 0;
String valueString = String(5);
String header;
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeout = 2000;



const char *ssid = "Becunnamki";
const char *password = "12345678";
IPAddress local_IP(192,168,1,2);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);


/*void handleRoot(){
  
}*/ //What if we dont need this?//

void setup() { //keep unchanged//


    Serial.begin(115200);
    servo1.attach(servoPin);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
  Serial.println("");
  
    
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

}

Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  } 

void loop(){
  WifiClient client=server.available();
   if (client) // For new clients {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = " ";

    while (client.connected() && currentTime - previousTime <= timeout){
      currentTime = millis();
      if (client.available()){
        char c = client.read();
        Serial.write(c);
        header += c;

        if(c== '\n'){
          if (currentLine.length() == 0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");

            client.println("</head><body><h1>ESP32 with Servo</h1>");
            client.println("<p>Position: <span id=\"servoPos\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
            
            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");
           
            client.println("</body></html>"); 

            if(header.indexOf("Get /?value=")>=0){
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);

              servo1.write(valueString.toInt());
              Serial.println(valueString);

              client.println();

              break;
            } else {
              currentLine = " ";
            }
            else if (c != '\r'){
              currentLine += c;
            }
          }
        }
      } header = " ";
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
    }
  }
}







