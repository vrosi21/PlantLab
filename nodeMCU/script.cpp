#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "YOUR_SSID";         // Replace with your WiFi SSID
const char* password = "YOUR_PASSWORD"; // Replace with your WiFi password

ESP8266WebServer server(80); // Create a web server object on port 80

// Define the plant structure
struct Plant {
  String type;
  String name;
  String latinName;
  String family;
  int recommendedMoistureLevel;
  int id;
  int moistureSensorPin;
  int waterPlantPin;
};

// Create a list of plants
Plant plants[] = {
  {"TypeA", "Fikret", "Fikretus", "FamilyA", 30, 1, A0, D7}, // Example plant Fikret
  // You can add more plants here with their respective attributes
};

const int numPlants = sizeof(plants) / sizeof(plants[0]); // Number of plants

void setup() {
  Serial.begin(115200);
  delay(10);

  // Set up pins for each plant
  for (int i = 0; i < numPlants; i++) {
    pinMode(plants[i].moistureSensorPin, INPUT); // Analog moisture sensor pin
    pinMode(plants[i].waterPlantPin, OUTPUT);     // Output pin for pump control
    digitalWrite(plants[i].waterPlantPin, LOW);   // Set pump initially off
  }

  // Connect to WiFi
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
  Serial.println(WiFi.localIP());  // Print the IP address

  // Define routes
  server.on("/plants", HTTP_GET, getAllPlants);      // GET /plants
  server.on("/waterplant/<id>", HTTP_POST, waterPlant); // POST /waterplant/<id>

  // Start the server
  server.begin();
  Serial.println("Server started.");
}

void loop() {
  // Handle incoming client requests
  server.handleClient();
}

// Function to handle GET /plants request
void getAllPlants() {
  String response = "[";
  for (int i = 0; i < numPlants; i++) {
    response += "{\"type\":\"" + plants[i].type + "\",";
    response += "\"name\":\"" + plants[i].name + "\",";
    response += "\"latinName\":\"" + plants[i].latinName + "\",";
    response += "\"family\":\"" + plants[i].family + "\",";
    response += "\"recommendedMoistureLevel\":" + String(plants[i].recommendedMoistureLevel) + ",";
    response += "\"id\":" + String(plants[i].id) + "}";
    if (i < numPlants - 1) {
      response += ",";
    }
  }
  response += "]";

  // Respond with the list of plants in JSON format
  server.send(200, "application/json", response);
}

// Function to handle POST /waterplant/<id> request
void waterPlant() {
  if (server.arg("id") == "") {
    server.send(400, "text/plain", "Missing ID");
    return;
  }

  int id = server.arg("id").toInt(); // Get the plant ID from the request
  for (int i = 0; i < numPlants; i++) {
    if (plants[i].id == id) {
      digitalWrite(plants[i].waterPlantPin, HIGH); // Turn on pump
      delay(5000);                                 // Run pump for 5 seconds
      digitalWrite(plants[i].waterPlantPin, LOW);  // Turn off pump after 5 seconds
      
      // Respond to confirm the action
      String response = "Watered plant: " + plants[i].name;
      server.send(200, "text/plain", response);
      return;
    }
  }
  
  // If the ID does not match any plant
  server.send(404, "text/plain", "Plant not found");
}
