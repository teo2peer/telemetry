// master
//  MAC 94:B9:7E:7B:AA:1C Negro
//  MAC 94:B9:7E:7B:A5:4C  negro2
//
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

//! FIXME MArchas 255 = marcha anterior o E

#define MESSAGE_LENGTH 105
#define EOL "\n"

static const char *PMK_KEY_STR = "CONTRASENYA1";
static const char *LMK_KEY_STR = "CONTRASENYA2";

uint8_t MAC_SENDER[] = {0x94, 0xB9, 0x7E, 0x7B, 0xAA, 0x1C};
esp_now_peer_info_t peerInfo;

typedef struct dataStruct
{
  char cadena[MESSAGE_LENGTH];
} dataStruct;

dataStruct datos;

// Callback when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&datos, incomingData, sizeof(datos));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  Serial.println(datos.cadena);
}

void setup()
{

  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  // Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  int a = esp_wifi_set_protocol(WIFI_IF_AP, WIFI_PROTOCOL_LR);
  WiFi.setSleep(false);
  // Serial.println("ESP Reciver");

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    // Serial.println("Error initializing ESP-NOW");
    return;
  }
  else
  {
    // Serial.println("ESP-NOW... OK!");
  }

  // Register peer
  memcpy(peerInfo.peer_addr, MAC_SENDER, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    // Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
}