#include <ESP8266WiFi.h>
#include <espnow.h>

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  String msg = "";
  for (int i = 0; i < len; i++) {
    msg += (char)incomingData[i];
  }
  Serial.print("Received data: ");
  Serial.println(msg);

  if (msg == "ON") {
    digitalWrite(LED_BUILTIN, LOW);  // LED ON (inverted logic on ESP8266)
  } else if (msg == "OFF") {
    digitalWrite(LED_BUILTIN, HIGH); // LED OFF
  }
}

void InitESPNow() {
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // LED initially OFF

  InitESPNow();
}

void loop() {
  // Nothing required here
}
