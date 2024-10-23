#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <FS.h>

const char* ssid = "YOUR_SSID";         // Replace with your WiFi SSID
const char* password = "YOUR_PASSWORD"; // Replace with your WiFi password

ESP8266WebServer server(80);

struct Plant {
  String type;
  String name;
  String scientificName;
  String family;
  String genus;
  String kingdom;
  String order;
  String tribe;
  int recommendedMoistureLevel;
  int id;
  int moistureSensorPinDO; 
  int moistureSensorPinAO;
  int waterPlantPin;
};

Plant plants[1];

void setup() {
  Serial.begin(115200);
  delay(10);

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  File file = SPIFFS.open("/nodeMCU/plants.json", "r");
  if (!file) {
    Serial.println("Failed to open file");
    return;
  }

  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, buf.get());

  JsonArray plantArray = doc.as<JsonArray>();
  for (size_t i = 0; i < plantArray.size(); i++) {
    JsonObject plantData = plantArray[i];
    plants[i].type = plantData["type"].as<String>();
    plants[i].name = plantData["name"].as<String>();
    plants[i].scientificName = plantData["scientificName"].as<String>();
    plants[i].family = plantData["family"].as<String>();
    plants[i].genus = plantData["genus"].as<String>();
    plants[i].kingdom = plantData["kingdom"].as<String>();
    plants[i].order = plantData["order"].as<String>();
    plants[i].tribe = plantData["tribe"].as<String>();
    plants[i].recommendedMoistureLevel = plantData["recommendedMoistureLevel"].as<int>();
    plants[i].id = plantData["id"].as<int>();
     plants[i].moistureSensorPinAO = plantData["moistureSensorPinAO"].as<int>(); // Analog pin
   plants[i].moistureSensorPinDO = plantData["moistureSensorPin"].as<int>();
    plants[i].waterPlantPin = plantData["waterPlantPin"].as<int>();
  }

  file.close();

  for (int i = 0; i < plantArray.size(); i++) {
    pinMode(plants[i].moistureSensorPin, INPUT);
    pinMode(plants[i].waterPlantPin, OUTPUT);
    digitalWrite(plants[i].waterPlantPin, LOW);
  }

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println(WiFi.localIP());

  server.on("/plants", HTTP_GET, getAllPlants);
  server.on("/waterplant/<id>", HTTP_POST, waterPlant);
  server.on("/addplant", HTTP_POST, addPlant);

  server.begin();
  Serial.println("Server started.");
}

void loop() {
  server.handleClient();
}

void getAllPlants() {
  File file = SPIFFS.open("/nodeMCU/plants.json", "r");
  if (!file) {
    server.send(500, "text/plain", "Failed to open file");
    return;
  }

  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, buf.get());

  JsonArray plantArray = doc.as<JsonArray>();
  for (size_t i = 0; i < plantArray.size(); i++) {
    JsonObject plantData = plantArray[i];

    int moistureLevel = analogRead(plants[i].moistureSensorPinAO); 

    float moisturePercentage = (moistureLevel / 1023.0) * 100; 

    plantData["currentMoistureLevel"] = moisturePercentage;

    plantData.remove("moistureSensorPin");
    plantData.remove("moistureSensorPinAO");
    plantData.remove("waterPlantPin");
  }

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}



void waterPlant() {
  if (server.arg("id") == "") {
    server.send(400, "text/plain", "Missing ID");
    return;
  }

  int id = server.arg("id").toInt();
  for (int i = 0; i < 1; i++) {
    if (plants[i].id == id) {
      digitalWrite(plants[i].waterPlantPin, HIGH);
      delay(5000);
      digitalWrite(plants[i].waterPlantPin, LOW);
      
      String response = "Watered plant: " + plants[i].name;
      server.send(200, "text/plain", response);
      return;
    }
  }
  
  server.send(404, "text/plain", "Plant not found");
}

void addPlant() {
  if (server.args() == 0) {
    server.send(400, "text/plain", "Body not received");
    return;
  }

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, server.arg(0));
  if (error) {
    server.send(400, "text/plain", "Invalid JSON");
    return;
  }

  JsonObject plantData = doc.as<JsonObject>();

  Plant newPlant;
  newPlant.type = plantData["type"].as<String>();
  newPlant.name = plantData["name"].as<String>();
  newPlant.scientificName = plantData["scientificName"].as<String>();
  newPlant.family = plantData["family"].as<String>();
  newPlant.genus = plantData["genus"].as<String>();
  newPlant.kingdom = plantData["kingdom"].as<String>();
  newPlant.order = plantData["order"].as<String>();
  newPlant.tribe = plantData["tribe"].as<String>();
  newPlant.recommendedMoistureLevel = plantData["recommendedMoistureLevel"].as<int>();
  newPlant.id = plantData["id"].as<int>();
  newPlant.moistureSensorPin = plantData["moistureSensorPin"].as<int>();
  newPlant.waterPlantPin = plantData["waterPlantPin"].as<int>();

  plants[0] = newPlant;

  server.send(200, "text/plain", "Plant added successfully");
}