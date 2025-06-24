#include <WiFi.h>
#include <esp_now.h>

#define LED_BUILTIN 2

// New-style OnDataRecv callback
void OnDataRecv(const esp_now_recv_info_t *recvInfo, const uint8_t *incomingData, int len) {
  String msg = "";
  for (int i = 0; i < len; i++) {
    msg += (char)incomingData[i];
  }
  Serial.print("Received data from: ");
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           recvInfo->src_addr[0], recvInfo->src_addr[1], recvInfo->src_addr[2],
           recvInfo->src_addr[3], recvInfo->src_addr[4], recvInfo->src_addr[5]);
  Serial.println(macStr);
  Serial.print("Message: ");
  Serial.println(msg);

  if (msg == "ON") {
    digitalWrite(LED_BUILTIN, LOW);  // Turn ON (for active-low logic)
  } else if (msg == "OFF") {
    digitalWrite(LED_BUILTIN, HIGH); // Turn OFF
  }
}

void InitESPNow() {
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback
  esp_now_register_recv_cb(OnDataRecv);
}

void setup() {
  Serial.begin(115200);

  // Set up WiFi in station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Not strictly necessary, but ensures clean start

  // Set up LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // OFF initially

  // Initialize ESP-NOW
  InitESPNow();
}

void loop() {
  // No need to do anything here
}
