# Michi Robot Hardware

This repository contains the hardware control code for the Michi Robot project, built on ESP32 microcontroller platform.

## 📁 Project Structure

```
Hardware/
├── main/                    # Main robot control system
├── CameraWebServer/         # ESP32-CAM web server implementation
├── component_test/          # Individual component testing modules
└── README.md               # This file
```

## 🤖 Main Robot Controller (`main/`)

The main folder contains the core robot control system that manages all robot functionalities including voice interaction, movement control, and IoT connectivity.

### Key Features:
- **Voice Recognition**: Wake word detection and conversation recording
- **Audio Playback**: Multi-state audio system with emotional responses
- **MQTT Communication**: IoT connectivity for remote control
- **State Machine**: Comprehensive robot behavior management
- **Product Detection**: YOLO-based object recognition integration
- **Servo Control**: Multi-axis movement (head, hands, eyes)

### Main Files:
- `main.ino` - Main program entry point and state machine
- `action.cpp/h` - Robot movement and animation control
- `eyes.cpp/h` - Eye movement and expression control
- `hands.cpp/h` - Hand/arm movement control
- `neck.cpp/h` - Head/neck positioning control
- `maintenance.cpp/h` - System maintenance and diagnostics

### Hardware Requirements:
- ESP32 DevKit
- I2S Microphone (INMP441)
- I2S Audio Amplifier (MAX98357A)
- MicroSD Card Module
- Servo Motors (SG90/MG90S)
- LED indicators
- WiFi connectivity

### Dependencies:
```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Audio.h>
#include <driver/i2s_std.h>
#include <SD.h>
```

## 📷 Camera Web Server (`CameraWebServer/`)

ESP32-CAM implementation for real-time video streaming and web-based camera control.

### Features:
- **Live Video Streaming**: Real-time MJPEG stream
- **Web Interface**: Browser-based camera controls
- **Image Capture**: Still photo capture functionality
- **Camera Settings**: Adjustable resolution, quality, and effects
- **Remote Access**: WiFi-based camera access

### Files:
- `CameraWebServer.ino` - Main camera server code
- `app_httpd.cpp` - HTTP server implementation
- `camera_index.h` - Web interface HTML/CSS/JS
- `camera_pins.h` - ESP32-CAM pin definitions
- `partitions.csv` - Memory partition configuration

### Hardware Requirements:
- ESP32-CAM module
- OV2640 camera sensor
- MicroSD card (optional)
- External antenna (recommended)

## 🧪 Component Testing (`component_test/`)

Individual testing modules for validating each hardware component separately.

### Test Modules:

#### `audio_player_sdcard/`
- **Purpose**: Test SD card audio playback functionality
- **Features**: Random song playback, volume control, file listing
- **Hardware**: ESP32 + I2S audio + SD card module

#### `oled_test_sh1106/`
- **Purpose**: Test OLED display functionality
- **Features**: Display text, graphics, and animations
- **Hardware**: ESP32 + SH1106 OLED display

#### `right_hand/`
- **Purpose**: Test hand/arm servo motor control
- **Features**: Individual servo testing and positioning
- **Hardware**: ESP32 + servo motors

#### `sound_test_i2s_new/`
- **Purpose**: Test I2S audio recording and playback
- **Features**: Microphone input testing, audio processing
- **Hardware**: ESP32 + I2S microphone + I2S amplifier

## 🚀 Getting Started

### Prerequisites:
1. **Arduino IDE** with ESP32 board package installed
2. **Required Libraries**:
   ```
   - ESP32Servo
   - PubSubClient
   - ArduinoJson
   - ESP32-audioI2S-master
   - Adafruit_SH110X (for OLED tests)
   ```

### Installation:
1. Clone this repository
2. Install required libraries via Arduino Library Manager
3. Select appropriate ESP32 board in Arduino IDE
4. Configure WiFi credentials in the code
5. Upload the desired sketch to your ESP32

### Configuration:
1. **WiFi Setup**: Update SSID and password in the code
2. **Pin Configuration**: Verify pin assignments match your hardware
3. **MQTT Broker**: Configure MQTT server settings for IoT features
4. **Audio Files**: Place audio files on SD card in appropriate folders

## 📋 Pin Configuration

### Main Robot (ESP32 DevKit):
```cpp
// I2S Audio Input
#define I2S_WS    25
#define I2S_SD    33
#define I2S_SCK   32

// I2S Audio Output
#define I2S_DOUT  18
#define I2S_BCLK  5
#define I2S_LRC   17

// SD Card
#define SD_CS     12
#define SPI_MOSI  23
#define SPI_MISO  19
#define SPI_SCK   13

// Status LED
#define LED_PIN   2
```

### ESP32-CAM:
- Camera pins are defined in `camera_pins.h`
- Standard ESP32-CAM pinout configuration

## 🔧 Usage

### Main Robot:
1. Power on the ESP32
2. Wait for WiFi connection
3. Say the wake word to activate voice interaction
4. Use MQTT commands for remote control
5. Monitor serial output for debugging

### Camera Module:
1. Upload CameraWebServer sketch to ESP32-CAM
2. Connect to the camera's IP address via web browser
3. Use web interface to control camera settings
4. Access live video stream

### Component Testing:
1. Upload individual test sketches
2. Open Serial Monitor for test results
3. Follow specific test instructions for each component

## 📡 MQTT Commands

The robot accepts various MQTT commands on topic `testtopic/mwtt`:

```json
{"response": "happy"}     // Trigger happy emotion
{"response": "sad"}       // Trigger sad emotion
{"response": "mad"}       // Trigger mad emotion
{"response": "dance"}     // Start dance mode
{"response": "sleep"}     // Enter sleep mode
{"response": "product"}   // Enter product detection mode
{"command": "ProductName"} // Detect specific product
```

## 🎵 Audio File Structure

SD card should contain the following folder structure:
```
/cut/          # Dance music files
/happy/        # Happy emotion sounds
/mad/          # Mad emotion sounds
/sad/          # Sad emotion sounds
/greetings/    # Greeting sounds
/detecting/    # Wake word detection sounds
/hearing/      # Listening indication sounds
/product/      # Product-specific audio files
/buffer/       # Temporary recording storage
```

## 🛠️ Troubleshooting

### Common Issues:
1. **Audio Problems**: Check I2S connections and SD card format
2. **WiFi Issues**: Verify credentials and signal strength
3. **Servo Jitter**: Check power supply adequacy
4. **MQTT Connection**: Verify broker settings and network connectivity
5. **Memory Errors**: Reduce buffer sizes or optimize code

### Debug Tips:
- Monitor Serial output for detailed logging
- Use component tests to isolate hardware issues
- Check pin connections against configuration
- Verify library versions compatibility

## 🤝 Contributing

1. Test your changes with component tests first
2. Follow existing code style and documentation
3. Update this README for any structural changes
4. Submit pull requests with clear descriptions

## 📄 License

This project is part of the Michi Robot ecosystem. Please refer to the main project repository for licensing information.

## 📞 Support

For technical support and questions:
- Check the component tests for hardware validation
- Review serial output for debugging information
- Consult the main project documentation for system integration

---

*Last updated: August 2025*
