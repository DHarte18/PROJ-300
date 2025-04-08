#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

//MAC Address of your receiver 
uint8_t broadcastAddress[] = {0xf0, 0x24, 0xf9, 0x7a, 0x39, 0x14};

//Variable to store signal to be sent
float active1;
float active2;
float active3;

//Variables to store incoming readings
float incomingFloor;
float incomingControlArms;
float incomingPosition;

//Variable to store if sending data was successful
String success;

//Structure that holds readings
typedef struct struct_message {
  float Floor;
  float ControlArms;
  float Position;
} struct_message;

//Storage for incoming and outgoing readings
struct_message incomingReadings;
struct_message outgoingReadings;

//Variable to store info about the peer
esp_now_peer_info_t peerInfo;

//Callback function for when data is sent
void OnDataSent (const uint8_t* mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Failed :(";
  }
}

//Callback function for when data is recieved
void OnDataRecv (const uint8_t* mac, const uint8_t* incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes recieved: ");
  Serial.println(len);
  incomingFloor = incomingReadings.Floor;
  incomingControlArms = incomingReadings.ControlArms;
  incomingPosition = incomingReadings.Position;
  Serial.println("Floor is at ");
  Serial.println(incomingReadings.Floor);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  //Initialise ESP_NOW
  if(esp_now_init() != ESP_OK) {
    Serial.println("Error initialising ESP_NOW");
    return;
  }
  //Once ESP_NOW is initialised, we will send CB to get the status of the transmitted packet
  esp_now_register_send_cb(OnDataSent);
  //Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  //Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  //Register for a callback function that will be called when data recieved
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  //Set values to be sent
  outgoingReadings.Floor = 20;
  outgoingReadings.ControlArms = 50;
  outgoingReadings.Position = 33,33;

  //Send message w/ ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*) &outgoingReadings, sizeof(outgoingReadings));
  if (result == ESP_OK) {
    Serial.println("Sent successfully");
  } else {
    Serial.println("Error sending");
  } delay(10000);
}
