// master
//  MAC 94:B9:7E:7B:AA:1C Negro
//  MAC 94:B9:7E:7B:A5:4C  negro2
//
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#define MESSAGE_LENGTH 128
#define EOL "\n"

static const char *PMK_KEY_STR = "CONTRASENYA1";
static const char *LMK_KEY_STR = "CONTRASENYA2";

uint8_t MAC_RECEIVER[] = {0x94, 0xB9, 0x7E, 0x7B, 0xA5, 0x4C};
esp_now_peer_info_t peerInfo;

typedef struct dataStruct
{
  char cadena[MESSAGE_LENGTH];
} dataStruct;

dataStruct datos;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0)
  {
    // success = "Delivery Success :)";
  }
  else
  {
    // success = "Delivery Fail :(";
  }
}

void setup()
{

  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  // Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  // Serial.println("ESP emisora");

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

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // esp_now_set_pmk((uint8_t *)PMK_KEY_STR);

  // Register peer
  memcpy(peerInfo.peer_addr, MAC_RECEIVER, 6);
  peerInfo.channel = 0;

  // for (uint8_t i = 0; i < 16; i++) {
  //   peerInfo.lmk[i] = LMK_KEY_STR[i];
  // }
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    // Serial.println("Failed to add peer");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Serial.println("Leyendo...");
  String stringToSend = "Sync";
  // Serial.println("Enviando..." + stringToSend);
  stringToSend.toCharArray(datos.cadena, MESSAGE_LENGTH);
  esp_err_t result = esp_now_send(MAC_RECEIVER, (uint8_t *)&datos, sizeof(datos));
}

void loop()
{
  // Send message via ESP-NOW

  while (Serial.available())
  {

    if (Serial.available() > 0)
    {

      // Serial.println("Leyendo...");
      String stringToSend = Serial.readStringUntil('\n');
      // Serial.println("Enviando..." + stringToSend);
      stringToSend.toCharArray(datos.cadena, MESSAGE_LENGTH);
      esp_err_t result = esp_now_send(MAC_RECEIVER, (uint8_t *)&datos, sizeof(datos));

      if (result == ESP_OK)
      {
        // Serial.println("Sent with success");
      }
      else
      {
        // Serial.println("Error sending the data");
      }
    }
  }

  // Send message via ESP-NOW
}
