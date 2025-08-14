// #include <ESP32Servo.h>

// static const int rightServoPin = 2;
// Servo servoRight;

// int positions[] = {0, 180};
// int begin = 0;

// unsigned long prevMillis = 0;
// const unsigned long interval = 300;

// void setup() {
//   Serial.begin(9600);
//   servoRight.attach(rightServoPin);
//   servoRight.write(90);
//   prevMillis = millis();
// }

// void loop() {
//   // if (millis() - prevMillis >= interval) {
//   //   begin++;
//   //   if (begin >= sizeof(positions)/sizeof(positions[0])) {
//   //     begin = 0;
//   //   }

//   //   servoRight.write(positions[begin]);
//   //   Serial.print("Posisi servo: ");
//   //   Serial.println(positions[begin]);

//   //   prevMillis = millis();
//   // }
// }

#include <ESP32Servo.h>

Servo myservo;  // Buat objek servo

// Variabel untuk kontrol servo
int currentPos = 90;   // Posisi saat ini
int targetPos = 90;   // Posisi target (default di tengah)
int servoSpeed = 20;  // Kecepatan gerakan (ms per langkah)
int stepSize = 1;     // Besar langkah setiap gerakan

// Variabel untuk timing non-blocking
unsigned long previousMillis = 0;
const long interval = 15;  // Interval dasar untuk perhitungan kecepatan

void setup() {
  myservo.attach(15);          // Servo terhubung ke pin 9
  myservo.write(currentPos);  // Set posisi awal
  Serial.begin(115200);
  Serial.println("Servo Smooth Control dengan millis()");
  Serial.println("Masukkan posisi (0-180) di Serial Monitor");
}

void loop() {
  // Baca input serial untuk mengubah posisi target
  if (Serial.available() > 0) {
    targetPos = Serial.parseInt();
    targetPos = constrain(targetPos, 0, 180);
    Serial.print("Bergerak ke: ");
    Serial.println(targetPos);

    // Bersihkan buffer serial
    while (Serial.available() > 0) Serial.read();
  }

  // Kontrol servo non-blocking
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= servoSpeed) {
    previousMillis = currentMillis;

    if (currentPos != targetPos) {
      // Hitung arah gerakan
      int direction = (targetPos > currentPos) ? 1 : -1;

      // Gerakkan servo
      currentPos += direction * stepSize;

      // Pastikan tidak melebihi target
      if ((direction == 1 && currentPos > targetPos) || (direction == -1 && currentPos < targetPos)) {
        currentPos = targetPos;
      }

      myservo.write(currentPos);

      // Debug posisi jika diperlukan
      // Serial.println(currentPos);
    }
  }

  // Lakukan tugas lain di sini tanpa terganggu
  // ...
}