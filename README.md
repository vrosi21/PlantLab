# PlantLab

Welcome to **PlantLab**, an IoT-based plant management system that allows you to monitor and care for your plants efficiently. This project uses a NodeMCU to connect to Wi-Fi and provide an API for plant management.

## Features

- Monitor moisture levels of plants using soil moisture sensors.
- Control a water pump to irrigate plants when moisture levels are low.
- Retrieve plant data, including scientific names and recommended moisture levels, via a RESTful API.
- A frontend web application for easy interaction and visualization of plant data.

## Project Structure

    PlantLab/
    │
    ├── nodeMCU/
    │ ├── PlantLab.ino # Arduino script for NodeMCU
    │ └── ...
    │
    ├── frontend/ # Web application for plant management
    │ ├── index.html # Main HTML file
    │ └── ...
    │
    └── README.md # Project documentation

## Getting Started

### Prerequisites

- **Arduino IDE**: Make sure you have the Arduino IDE installed on your computer.
- **NodeMCU**: You will need a NodeMCU board to run the Arduino script.
- **ESP8266WiFi and ESP8266WebServer libraries**: These libraries are necessary for the script to work with the NodeMCU.

### Installation

1. Clone this repository to your local machine:
   [link text](git clone https://github.com/vrosi21/PlantLab.git cd PlantLab/nodeMCU)

2. Open the `PlantLab.ino` file in Arduino IDE.

3. Replace `YOUR_SSID` and `YOUR_PASSWORD` in the script with your Wi-Fi credentials.

4. Upload the script to your NodeMCU board.

5. Access the web application by navigating to `http://<YOUR_NODEMCU_IP>/plants` in your web browser.

## Usage

- Use the `/plants` endpoint to get the list of all plants and their details.
- Use the `/waterplant/<id>` endpoint to water a specific plant by its ID.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Acknowledgements

- Special thanks to the contributors and open-source community for their support.
