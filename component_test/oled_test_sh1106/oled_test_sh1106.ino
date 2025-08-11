#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>


#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
// Adafruit_SH110X display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//reference state
int ref_eye_height = 40;
int ref_eye_width = 45;
int ref_space_between_eye = 10;
int ref_corner_radius = 15;
//current state of the eyes
int left_eye_height = ref_eye_height;
int left_eye_width = ref_eye_width;
int left_eye_x = 32;
int left_eye_y = 32;
int right_eye_x = 32 + ref_eye_width + ref_space_between_eye;
int right_eye_y = 32;
int right_eye_height = ref_eye_height;
int right_eye_width = ref_eye_width;

// Animation state
bool isAnimating = false;
unsigned long animationStartTime = 0;
int currentStep = 0;
const int totalSteps = 20; // Number of animation steps
const unsigned long frameInterval = 50; // 50ms per frame, ~1s total

void draw_eyes(bool update = true) {
  display.clearDisplay();
  //draw from center
  int x = int(left_eye_x - left_eye_width / 2);
  int y = int(left_eye_y - left_eye_height / 2);
  display.fillRoundRect(x, y, left_eye_width, left_eye_height, ref_corner_radius, SH110X_WHITE);
  x = int(right_eye_x - right_eye_width / 2);
  y = int(right_eye_y - right_eye_height / 2);
  display.fillRoundRect(x, y, right_eye_width, right_eye_height, ref_corner_radius, SH110X_WHITE);
  if (update) {
    display.display();
  }
}

void center_eyes(bool update = true) {
  //move eyes to the center of the display, defined by SCREEN_WIDTH, SCREEN_HEIGHT
  left_eye_height = ref_eye_height;
  left_eye_width = ref_eye_width;
  right_eye_height = ref_eye_height;
  right_eye_width = ref_eye_width;

  left_eye_x = SCREEN_WIDTH / 2 - ref_eye_width / 2 - ref_space_between_eye / 2;
  left_eye_y = SCREEN_HEIGHT / 2;
  right_eye_x = SCREEN_WIDTH / 2 + ref_eye_width / 2 + ref_space_between_eye / 2;
  right_eye_y = SCREEN_HEIGHT / 2;

  draw_eyes(update);
}

void blink(int speed = 12) {
  draw_eyes();

  for (int i = 0; i < 3; i++) {
    left_eye_height = left_eye_height - speed;
    right_eye_height = right_eye_height - speed;
    draw_eyes();
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_height = left_eye_height + speed;
    right_eye_height = right_eye_height + speed;

    draw_eyes();
    delay(1);
  }
}

void sleep() {
  left_eye_height = 2;
  right_eye_height = 2;
  draw_eyes(true);
}

void wakeup() {
  sleep();

  for (int h = 0; h <= ref_eye_height; h += 2) {
    left_eye_height = h;
    right_eye_height = h;
    draw_eyes(true);
  }
}

void happy_eye() {
  center_eyes(false);
  //draw inverted triangle over eye lower part
  int offset = ref_eye_height / 2;
  for (int i = 0; i < 10; i++) {
    display.fillTriangle(left_eye_x - left_eye_width / 2 - 1, left_eye_y + offset, left_eye_x + left_eye_width / 2 + 1, left_eye_y + 5 + offset, left_eye_x - left_eye_width / 2 - 1, left_eye_y + left_eye_height + offset, SH110X_BLACK);
    //display.fillRect(left_eye_x-left_eye_width/2-1, left_eye_y+5, left_eye_width+1, 20,SH110X_BLACK);

    display.fillTriangle(right_eye_x + right_eye_width / 2 + 1, right_eye_y + offset, right_eye_x - left_eye_width / 2 - 1, right_eye_y + 5 + offset, right_eye_x + right_eye_width / 2 + 1, right_eye_y + right_eye_height + offset, SH110X_BLACK);
    //display.fillRect(right_eye_x-right_eye_width/2-1, right_eye_y+5, right_eye_width+1, 20,SH110X_BLACK);
    offset -= 2;
    display.display();
    delay(1);
  }

  display.display();
  delay(1000);
}

void angry_eye() {
  center_eyes(false);

  // Koordinat segitiga kiri
  int ax = 0, ay = 10;
  int bx = 65, by = 10;
  int cx = 65, cy = 40;

  // Koordinat segitiga kanan (mirror)
  int maxX = SCREEN_WIDTH - 1;
  int ax_r = maxX - ax;
  int bx_r = maxX - bx;
  int cx_r = maxX - cx;

  for (int yOffset = -13; yOffset <= 0; yOffset++) {
    display.clearDisplay();
    draw_eyes(false);  // Gambar mata dulu sebagai latar

    // Gambar segitiga kiri (animasi alis turun)
    display.fillTriangle(ax, ay + yOffset, bx, by + yOffset, cx, cy + yOffset, SH110X_BLACK);

    // Gambar segitiga kanan (mirror)
    display.fillTriangle(ax_r, ay + yOffset, bx_r, by + yOffset, cx_r, cy + yOffset, SH110X_BLACK);

    display.display();
    delay(1);
  }

  display.display();
  delay(1000);
}

void sad_eye() {
  center_eyes(false);

  // Koordinat segitiga kiri
  int ax = 5, ay = 10;
  int bx = 65, by = 10;
  int cx = 5, cy = 40;

  // Koordinat segitiga kanan (mirror)
  int maxX = SCREEN_WIDTH - 1;
  int ax_r = maxX - ax;
  int bx_r = maxX - bx;
  int cx_r = maxX - cx;

  for (int yOffset = -13; yOffset <= 0; yOffset++) {
    display.clearDisplay();
    draw_eyes(false);  // Gambar mata dulu sebagai latar

    // Gambar segitiga kiri (animasi alis turun)
    display.fillTriangle(ax, ay + yOffset, bx, by + yOffset, cx, cy + yOffset, SH110X_BLACK);

    // Gambar segitiga kanan (mirror)
    display.fillTriangle(ax_r, ay + yOffset, bx_r, by + yOffset, cx_r, cy + yOffset, SH110X_BLACK);

    display.display();
    delay(1);
  }

  display.display();
  delay(1000);
}

void bored_eye() {
  center_eyes(false);

  int finalX = 5;
  int finalY = 5;
  int width = 65;
  int height = 25;

  for (int yOffset = -15; yOffset <= 0; yOffset++) {
    display.clearDisplay();
    draw_eyes(false);  // Gambar mata sebagai latar

    // Gambar rect kiri dari atas turun ke posisi final
    display.fillRect(finalX, finalY + yOffset, width, height, SH110X_BLACK);

    // Gambar rect kanan mirror dari kiri
    int mirrorX = SCREEN_WIDTH - 1 - (finalX + width - 1);  // mirror dari sisi kanan
    display.fillRect(mirrorX, finalY + yOffset, width, height, SH110X_BLACK);

    display.display();
    delay(1);  // Tambah delay untuk kelihatan animasi
  }

  display.display();
  delay(1000);
}

void excited_eye() {
  center_eyes(false);

  int startY = 78;  // posisi awal (di bawah)
  int endY = 65;    // posisi akhir

  for (int y = startY; y >= endY; y--) {
    display.clearDisplay();
    draw_eyes(false);  // Jika ingin muncul di atas mata

    // Gambar lingkaran putih naik dari bawah
    display.fillCircle(35, y, 35, SH110X_BLACK);

    int mirrored_x = (SCREEN_WIDTH - 1) - 35;
    display.fillCircle(mirrored_x, y, 35, SH110X_BLACK);

    display.display();
    delay(1);  // Sesuaikan untuk kecepatan animasi
  }

  display.display();
  delay(1000);
}

void suspicious_eye() {
  center_eyes(false);

  int finalX = 5;
  int finalY = 10;
  int finalY2 = 10;
  int width = 65;
  int height = 15;

  for (int yOffset = -20; yOffset <= 0; yOffset++) {
    display.clearDisplay();
    draw_eyes(false);  // Gambar mata sebagai latar

    // Gambar rect kiri dari atas turun ke posisi final
    display.fillRect(finalX, finalY + yOffset, width, height, SH110X_BLACK);

    // Gambar rect kanan mirror dari kiri
    int mirrorX = SCREEN_WIDTH - 1 - (finalX + width - 1);  // mirror dari sisi kanan
    display.fillRect(mirrorX, finalY + yOffset, width, height, SH110X_BLACK);

    display.display();
    delay(1);  // Tambah delay untuk kelihatan animasi
  }

  for (int yOffset = -20; yOffset <= 0; yOffset++) {
    display.clearDisplay();
    draw_eyes(false);  // Gambar mata sebagai latar

    // Gambar rect kiri dari atas turun ke posisi final
    display.fillRect(finalX, finalY + yOffset, width, height, SH110X_BLACK);

    // Gambar rect kanan mirror dari kiri
    int mirrorX = SCREEN_WIDTH - 1 - (finalX + width - 1);  // mirror dari sisi kanan
    display.fillRect(mirrorX, finalY + yOffset, width, height, SH110X_BLACK);

    display.display();
    delay(1);  // Tambah delay untuk kelihatan animasi
  }

  display.display();
  delay(1000);
}

void test_eye() {
  center_eyes(false);

  int finalX = 5;
  int finalY = 10;
  int width = 65;
  int height = 15;
  int finalY2 = 35;

  //draw inverted triangle over eye lower part
  int offset = ref_eye_height / 2;
  for (int i = 0; i < 10; i++) {
    // display.fillCircle(35, 55, 35, SH110X_BLACK);
    //display.fillRect(left_eye_x-left_eye_width/2-1, left_eye_y+5, left_eye_width+1, 20,SH110X_BLACK);

    // int mirrored_x = (SCREEN_WIDTH - 1) - 35;
    // display.fillCircle(mirrored_x, 55, 35, SH110X_BLACK);
    //display.fillRect(right_eye_x-right_eye_width/2-1, right_eye_y+5, right_eye_width+1, 20,SH110X_BLACK);

    // Gambar rect kiri dari atas turun ke posisi final
    display.fillRect(finalX, finalY, width, height, SH110X_BLACK);

    // Gambar rect kanan mirror dari kiri
    int mirrorX = SCREEN_WIDTH - 1 - (finalX + width - 1);  // mirror dari sisi kanan
    display.fillRect(mirrorX, finalY, width, height, SH110X_BLACK);

    // Gambar rect kiri dari atas turun ke posisi final
    display.fillRect(finalX, finalY2, width, height + 15, SH110X_BLACK);

    // Gambar rect kanan mirror dari kiri
    display.fillRect(mirrorX, finalY2, width, height + 15, SH110X_BLACK);
    offset -= 2;
    display.display();
    delay(1);
  }

  display.display();
  delay(1000);
}

void triangle() {
  display.clearDisplay();

  int finalX = 5;
  int finalY = 5;
  int width = 65;
  int height = 15;

  int finalY2 = 35;

  // Gambar rect kiri dari atas turun ke posisi final
  display.fillRect(finalX, finalY, width, height, SH110X_WHITE);

  // Gambar rect kanan mirror dari kiri
  int mirrorX = SCREEN_WIDTH - 1 - (finalX + width - 1);  // mirror dari sisi kanan
  display.fillRect(mirrorX, finalY, width, height, SH110X_WHITE);

  // Gambar rect kiri dari atas turun ke posisi final
  display.fillRect(finalX, finalY2, width, height + 15, SH110X_WHITE);

  // Gambar rect kanan mirror dari kiri
  display.fillRect(mirrorX, finalY2, width, height + 15, SH110X_WHITE);

  display.display();  // Tampilkan segitiga
}

void questioning() {
  display.clearDisplay();
  display.fillRoundRect(95, 1, 7, 15, 3, SH110X_WHITE);     // left block
  display.fillRoundRect(110, 1, 7, 25, 3, SH110X_WHITE);    // right long block
  display.fillRoundRect(95, 1, 20, 7, 3, SH110X_WHITE);     // middle top block
  display.fillRoundRect(100, 20, 15, 7, 3, SH110X_WHITE);   // middle mid block
  display.fillRoundRect(100, 20, 7, 15, 3, SH110X_WHITE);   // middle bottom block
  display.fillRoundRect(100, 45, 7, 7, 3, SH110X_WHITE);    // the dot
  display.fillRoundRect(7, 25, 35, 30, 10, SH110X_WHITE);   // left eye
  display.fillRoundRect(48, 18, 37, 32, 10, SH110X_WHITE);  // right eye
  display.display();
}

void animate_eyes_to_questioning() {
  // Initial state (from draw_eyes)
  float start_left_x = 9;
  float start_left_y = 12;
  float start_left_width = 45;
  float start_left_height = 40;
  float start_right_x = 64;
  float start_right_y = 12;
  float start_right_width = 45;
  float start_right_height = 40;
  float start_radius = 15;

  // Final state (from questioning)
  float end_left_x = 7;
  float end_left_y = 25;
  float end_left_width = 35;
  float end_left_height = 30;
  float end_right_x = 48;
  float end_right_y = 18;
  float end_right_width = 37;
  float end_right_height = 32;
  float end_radius = 10;

  // Check if enough time has passed for the next frame
  unsigned long currentTime = millis();
  if (currentTime - animationStartTime >= frameInterval) {
    // Calculate interpolation factor (0.0 to 1.0)
    float t = (float)currentStep / totalSteps;

    // Interpolate parameters
    float left_x = start_left_x + (end_left_x - start_left_x) * t;
    float left_y = start_left_y + (end_left_y - start_left_y) * t;
    float left_width = start_left_width + (end_left_width - start_left_width) * t;
    float left_height = start_left_height + (end_left_height - start_left_height) * t;
    float right_x = start_right_x + (end_right_x - start_right_x) * t;
    float right_y = start_right_y + (end_right_y - start_right_y) * t;
    float right_width = start_right_width + (end_right_width - start_right_width) * t;
    float right_height = start_right_height + (end_right_height - start_right_height) * t;
    float radius = start_radius + (end_radius - start_radius) * t;

    // Draw the frame
    display.clearDisplay();
    display.fillRoundRect((int)left_x, (int)left_y, (int)left_width, (int)left_height, (int)radius, SH110X_WHITE);
    display.fillRoundRect((int)right_x, (int)right_y, (int)right_width, (int)right_height, (int)radius, SH110X_WHITE);
    display.display();

    // Update animation state
    currentStep++;
    animationStartTime = currentTime;

    // Check if animation is complete
    if (currentStep > totalSteps) {
      isAnimating = false;
      currentStep = 0;
      questioning();  // Draw final state with question mark
    }
  }
}

void questionMark() {
  display.clearDisplay();

  display.fillRoundRect(95, 1, 7, 15, 3, SH110X_WHITE);    // left block
  display.fillRoundRect(110, 1, 7, 25, 3, SH110X_WHITE);   // right long block
  display.fillRoundRect(95, 1, 20, 7, 3, SH110X_WHITE);    // middle top block
  display.fillRoundRect(100, 20, 15, 7, 3, SH110X_WHITE);  // middle mid block
  display.fillRoundRect(100, 20, 7, 15, 3, SH110X_WHITE);  // middle bottom block
  display.fillRoundRect(100, 45, 7, 7, 3, SH110X_WHITE);   // the dot

  display.fillRoundRect(7, 25, 35, 30, 10, SH110X_WHITE);
  display.fillRoundRect(48, 18, 37, 32, 10, SH110X_WHITE);

  display.display();
}

void saccade(int direction_x, int direction_y) {
  //quick movement of the eye, no size change. stay at position after movement, will not move back,  call again with opposite direction
  //direction == -1 :  move left
  //direction == 1 :  move right

  int direction_x_movement_amplitude = 8;
  int direction_y_movement_amplitude = 6;
  int blink_amplitude = 8;

  for (int i = 0; i < 1; i++) {
    left_eye_x += direction_x_movement_amplitude * direction_x;
    right_eye_x += direction_x_movement_amplitude * direction_x;
    left_eye_y += direction_y_movement_amplitude * direction_y;
    right_eye_y += direction_y_movement_amplitude * direction_y;

    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    draw_eyes();
    delay(1);
  }

  for (int i = 0; i < 1; i++) {
    left_eye_x += direction_x_movement_amplitude * direction_x;
    right_eye_x += direction_x_movement_amplitude * direction_x;
    left_eye_y += direction_y_movement_amplitude * direction_y;
    right_eye_y += direction_y_movement_amplitude * direction_y;

    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;

    draw_eyes();
    delay(1);
  }
}

void move_right_big_eye() {
  move_big_eye(1);
}
void move_left_big_eye() {
  move_big_eye(-1);
}
void move_big_eye(int direction) {
  //direction == -1 :  move left
  //direction == 1 :  move right

  int direction_oversize = 1;
  int direction_movement_amplitude = 2;
  int blink_amplitude = 5;

  for (int i = 0; i < 3; i++) {
    left_eye_x += direction_movement_amplitude * direction;
    right_eye_x += direction_movement_amplitude * direction;
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    if (direction > 0) {
      right_eye_height += direction_oversize;
      right_eye_width += direction_oversize;
    } else {
      left_eye_height += direction_oversize;
      left_eye_width += direction_oversize;
    }

    draw_eyes();
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_x += direction_movement_amplitude * direction;
    right_eye_x += direction_movement_amplitude * direction;
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    if (direction > 0) {
      right_eye_height += direction_oversize;
      right_eye_width += direction_oversize;
    } else {
      left_eye_height += direction_oversize;
      left_eye_width += direction_oversize;
    }
    draw_eyes();
    delay(1);
  }
  delay(1000);

  for (int i = 0; i < 3; i++) {
    left_eye_x -= direction_movement_amplitude * direction;
    right_eye_x -= direction_movement_amplitude * direction;
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    if (direction > 0) {
      right_eye_height -= direction_oversize;
      right_eye_width -= direction_oversize;
    } else {
      left_eye_height -= direction_oversize;
      left_eye_width -= direction_oversize;
    }
    draw_eyes();
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_x -= direction_movement_amplitude * direction;
    right_eye_x -= direction_movement_amplitude * direction;
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    if (direction > 0) {
      right_eye_height -= direction_oversize;
      right_eye_width -= direction_oversize;
    } else {
      left_eye_height -= direction_oversize;
      left_eye_width -= direction_oversize;
    }
    draw_eyes();
    delay(1);
  }

  center_eyes();
}

void setup() {
  display.begin(SCREEN_ADDRESS, true);  // Address 0x3C default

  //for usb communication
  Serial.begin(9600);

  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SH110X_WHITE);  // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.println(F(" "));
  display.display();
  delay(300);
  // sleep();
  draw_eyes();
}

void loop() {
  // wakeup();
  // delay(300);

  // center_eyes(true);
  // delay(300);

  // move_right_big_eye();
  // delay(300);

  // move_left_big_eye();
  // delay(300);

  // blink(10);
  // delay(300);

  // blink(20);
  // delay(300);

  // happy_eye();
  // delay(300);

  // blink(10);
  // delay(300);

  // blink(20);
  // delay(300);

  // angry_eye();
  // delay(300);

  // blink(10);
  // delay(300);

  // blink(20);
  // delay(300);

  // sad_eye();
  // delay(300);

  // blink(10);
  // delay(300);

  // blink(20);
  // delay(300);

  // bored_eye();
  // delay(300);

  // blink(10);
  // delay(300);

  // blink(20);
  // delay(300);

  // excited_eye();
  // delay(300);

  // test_eye();
  // delay(300);

  // triangle();
  // delay(300);

  // questionMark();
  // delay(300);

  // questioning();
  // delay(300);

  static unsigned long lastStateChange = 0;
  static bool showInitialEyes = true;
  unsigned long currentTime = millis();

  // Switch between initial eyes and animation every 2 seconds
  if (currentTime - lastStateChange >= 2000) {
    if (showInitialEyes) {
      isAnimating = true;
      animationStartTime = currentTime;
      currentStep = 0;
    } else {
      draw_eyes();
    }
    showInitialEyes = !showInitialEyes;
    lastStateChange = currentTime;
  }

  // Run animation if active
  if (isAnimating) {
    animate_eyes_to_questioning();
  }
}