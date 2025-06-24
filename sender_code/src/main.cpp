#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <espnow.h>

uint8_t receiverMac[] = {0x30, 0x83, 0x98, 0x9E, 0x5D, 0x98}; // Replace with the receiver's MAC address

ESP8266WebServer server(80); // Create a web server on port 80
const char *ssid = "SENDER";
const char *password = "12345678";

void initEspNow();
{
  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.println("ESP-NOW initialized successfully");
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(receiverMac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void sendCommand(const char *cmd)
{
  esp_now_send(receiverMac, (uint8_t *)cmd, strlen(cmd));
}

void handleOn()
{
  sendCommand("ON");
  server.send(200, "text/html", "Command ON sent");
}
void handleOff()
{
  sendCommand("OFF");
  server.send(200, "text/html", "Command OFF sent");
}

void handleRoot()
{
  String html = "<html><body>";
  html += "<h1>ESP8266 Web Control</h1>";
  html += "<button onclick=\"sendCmd('/on')\">Turn ON</button>";
  html += "<button onclick=\"sendCmd('/off')\">Turn OFF</button>";
  html += "<div id='status'></div>";
  html += "<script>";
  html += "function sendCmd(url) {";
  html += "  fetch(url).then(r => r.text()).then(t => document.getElementById('status').innerText = t);";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}
void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Ensure WiFi is disconnected
  initEspNow();
  WiFi.softAP(ssid, password); // Start the ESP8266 in AP mode
  Serial.println("ESP8266 started in AP mode");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Web server started");
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
}

void loop()
{
  server.handleClient();              // Handle incoming web requests
  esp_now_send(receiverMac, NULL, 0); // Keep the ESP-NOW connection alive
  delay(1000);                        // Adjust delay as needed
}
