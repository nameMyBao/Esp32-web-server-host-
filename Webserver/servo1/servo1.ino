#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* ssid = "Mai Lan Cuc Truc";
const char* password = "khanhbaotai";

Servo servo;
int servoPin = 4;
int servoPosition = 90;

WebServer server(80);

void setup() {
  Serial.begin(115200);

  servo.attach(servoPin); //connect the servo 
  servo.write(servoPosition);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](){
    String html = "<html><body>";
    html += "<h1>Servo Control</h1>";
    html += "<input type=\"range\" min=\"0\" max=\"180\" value=\"" + String(servoPosition) + "\" step=\"1\" onchange=\"updateServo(this.value)\" />";
    html += "<script>function updateServo(pos) { var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/servo?pos=' + pos, true); xhttp.send(); }</script>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/servo", HTTP_GET, []() 
  {
    String inputPos = server.arg("pos") ;
    int pos = inputPos.toInt();
    servoPosition = pos;
    servo.write(pos);
    server.send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}





