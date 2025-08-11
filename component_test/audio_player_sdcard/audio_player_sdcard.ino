#include "Arduino.h"
#include "Audio.h"
#include "SD.h"
#include "FS.h"

// microSD Card Reader connections
#define SD_CS 12
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 13

// I2S Connections
#define I2S_DOUT 18
#define I2S_BCLK 5
#define I2S_LRC 17

Audio audio;
unsigned long lastAudioTime = 0;
bool isPlaying = false;
File root;
int totalSongs = 0;
String *songList = nullptr;
int currentSongIndex = -1;

void listAllSongs() {
  root = SD.open("/cut");
  totalSongs = 0;

  // First pass: count the number of songs
  File file = root.openNextFile();
  while (file) {
    if (!file.isDirectory()) {
      totalSongs++;
    }
    file = root.openNextFile();
  }
  root.close();

  // Allocate memory for song list
  if (songList != nullptr) {
    delete[] songList;
  }
  songList = new String[totalSongs];

  // Second pass: store song names with full path
  root = SD.open("/cut");
  int index = 0;
  file = root.openNextFile();
  while (file && index < totalSongs) {
    if (!file.isDirectory()) {
      songList[index] = "/cut/" + String(file.name());  // Store with full path
      index++;
    }
    file = root.openNextFile();
  }
  root.close();

  Serial.print("Found ");
  Serial.print(totalSongs);
  Serial.println(" songs:");
  for (int i = 0; i < totalSongs; i++) {
    Serial.println(songList[i]);
  }
}

void playRandomSong() {
  if (totalSongs == 0) {
    Serial.println("No songs available!");
    return;
  }

  int randomIndex = random(totalSongs);
  currentSongIndex = randomIndex;
  String songPath = songList[currentSongIndex];

  Serial.print("Now playing: ");
  Serial.println(songPath);

  audio.stopSong();
  
  // Add delay to ensure proper stop
  delay(100);
  
  if (audio.connecttoFS(SD, songPath.c_str())) {
    isPlaying = true;
    Serial.println("Audio started successfully");
  } else {
    Serial.println("Failed to start audio");
    isPlaying = false;
  }
}

void playNextSong() {
  if (totalSongs == 0) {
    Serial.println("No songs available!");
    return;
  }

  currentSongIndex = (currentSongIndex + 1) % totalSongs;
  String songPath = songList[currentSongIndex];

  Serial.print("Now playing: ");
  Serial.println(songPath);

  audio.stopSong();
  audio.connecttoFS(SD, songPath.c_str());
  isPlaying = true;
}

void stopPlayback() {
  audio.stopSong();
  isPlaying = false;
  Serial.println("Playback stopped");
}

void handleSerialInput() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equalsIgnoreCase("play")) {
      if (!isPlaying) {
        playRandomSong();
      } else {
        Serial.println("Already playing");
      }
    } else if (command.equalsIgnoreCase("next")) {
      playNextSong();
    } else if (command.equalsIgnoreCase("stop")) {
      stopPlayback();
    } else if (command.equalsIgnoreCase("list")) {
      listAllSongs();
    } else {
      Serial.println("Unknown command. Available commands: play, next, stop, list");
    }
  }
}

void setup() {
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  Serial.begin(115200);
  Serial.println("Starting Audio Player...");

  if (!SD.begin(SD_CS)) {
    Serial.println("Error accessing microSD card!");
    while (true)
      ;
  }

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(15);  // Keep volume moderate to prevent errors
  audio.setTone(0, 0, 0);  // Disable tone processing to reduce load
  audio.forceMono(true);   // Force mono to reduce processing
  audio.setBalance(0);     // Set balance to center

  listAllSongs();
  randomSeed(analogRead(0));  // Seed the random number generator

  Serial.println("Ready. Send commands:");
  Serial.println("play - Start random playback");
  Serial.println("next - Play next song");
  Serial.println("stop - Stop playback");
  Serial.println("list - List all available songs");
}

void loop() {
  unsigned long now = millis();

  // Call audio.loop() every 1ms
  if (now - lastAudioTime >= 1) {
    lastAudioTime = now;
    audio.loop();
  }

  handleSerialInput();
}

// Optional audio event callbacks
void audio_info(const char *info) {
  Serial.print("info        ");
  Serial.println(info);
  
  // Check for common audio errors
  if (strstr(info, "Stream lost") || strstr(info, "Error") || strstr(info, "Failed")) {
    Serial.println("Audio error detected - resetting playback");
    isPlaying = false;
    audio.stopSong();
  }
}
void audio_id3data(const char *info) {
  Serial.print("id3data     ");
  Serial.println(info);
}
void audio_eof_mp3(const char *info) {
  Serial.print("eof_mp3     ");
  Serial.println(info);
  if (isPlaying) {
    playRandomSong();  // Play another random song when current one ends
  }
}