#include <driver/i2s_std.h>

// Pin I2S untuk INMP441
#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32

#define I2S_PORT_NUM I2S_NUM_0
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 1024

// Channel handle
i2s_chan_handle_t rx_handle = NULL;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("INMP441 Audio Level Test");

  // Channel configuration
  i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_PORT_NUM, I2S_ROLE_MASTER);
  i2s_new_channel(&chan_cfg, NULL, &rx_handle);

  // Standard configuration
  i2s_std_config_t std_cfg = {
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(SAMPLE_RATE),
    .slot_cfg = {
      .data_bit_width = I2S_DATA_BIT_WIDTH_32BIT,
      .slot_bit_width = I2S_SLOT_BIT_WIDTH_32BIT,
      .slot_mode = I2S_SLOT_MODE_MONO,
      .slot_mask = I2S_STD_SLOT_LEFT,
      .ws_width = I2S_SLOT_BIT_WIDTH_32BIT,
      .ws_pol = false,
      .bit_shift = true,
      .msb_right = true,
    },
    .gpio_cfg = {
      .mclk = GPIO_NUM_NC,
      .bclk = static_cast<gpio_num_t>(I2S_SCK),
      .ws = static_cast<gpio_num_t>(I2S_WS),
      .dout = GPIO_NUM_NC,
      .din = static_cast<gpio_num_t>(I2S_SD),
      .invert_flags = {
        .mclk_inv = false,
        .bclk_inv = false,
        .ws_inv = false,
      },
    },
  };

  i2s_channel_init_std_mode(rx_handle, &std_cfg);
  i2s_channel_enable(rx_handle);
}

void loop() {
  int32_t buffer[BUFFER_SIZE];
  size_t bytes_read;

  // Baca data dari mikrofon
  i2s_channel_read(rx_handle, (void *)buffer, sizeof(buffer), &bytes_read, portMAX_DELAY);
  int samples_read = bytes_read / sizeof(int32_t);

  // Hitung nilai rata-rata absolut
  uint64_t total = 0;
  for (int i = 0; i < samples_read; i++) {
    total += abs(buffer[i] >> 14);  // geser agar lebih kecil
  }

  int avg = total / samples_read;

  // Tampilkan bar
  int bar_length = map(avg, 0, 3000, 0, 50);  // sesuaikan nilai max agar sensitif
  Serial.print("Volume: ");
  for (int i = 0; i < bar_length; i++) {
    Serial.print("|");
  }
  Serial.println();

  delay(100);
}