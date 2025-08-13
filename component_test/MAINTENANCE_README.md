# Maintenance Mode Documentation

## Overview
Maintenance mode adalah fitur testing yang dapat diakses saat robot dalam mode SLEEP. Mode ini memungkinkan testing individual komponen robot melalui Serial Monitor.

## Cara Menggunakan

### 1. Masuk ke Sleep Mode
Robot harus dalam mode SLEEP untuk mengakses maintenance mode. Kirim command `sleep` melalui MQTT atau tunggu robot masuk sleep mode secara otomatis.

### 2. Buka Serial Monitor
- Baud Rate: 115200
- Line Ending: Newline (\n)

### 3. Kirim Test Commands
Ketik salah satu command berikut di Serial Monitor:

## Available Commands

### test_hands
- **Fungsi**: Test servo tangan kiri dan kanan
- **Test Sequence**: 
  1. Standby position
  2. Idle movement (5 detik)
  3. Happy movement (3 detik)
  4. Mad movement (3 detik)
  5. Sad movement (3 detik)
  6. Dance movement (5 detik)
  7. Return to standby
- **Total Duration**: ~23 detik

### test_neck
- **Fungsi**: Test servo leher
- **Test Sequence**:
  1. Standby position
  2. Idle neck movement (8 detik)
  3. Answer neck movement (5 detik)
  4. Sad neck movement (5 detik)
  5. Thinking neck movement (8 detik)
  6. DetectWakeword neck movement (8 detik)
  7. Return to standby
- **Total Duration**: ~36 detik

### test_eyes
- **Fungsi**: Test OLED display mata
- **Test Sequence**:
  1. Sleep eyes (3 detik)
  2. Idle eyes (5 detik)
  3. Happy eyes (3 detik)
  4. Mad eyes (3 detik)
  5. Sad eyes (3 detik)
  6. Question eyes (3 detik)
  7. Excited eyes (3 detik)
  8. Return to standby
- **Total Duration**: ~26 detik

### test_all
- **Fungsi**: Jalankan semua test secara berurutan
- **Sequence**: test_hands → test_neck → test_eyes
- **Total Duration**: ~87 detik

### stop
- **Fungsi**: Stop test yang sedang berjalan dan kembali ke standby

### help
- **Fungsi**: Tampilkan daftar command yang tersedia

## Usage Example

```
=== MAINTENANCE MODE READY ===
Available commands:
--- MAINTENANCE COMMANDS ---
test_hands  - Test left and right hand servos
test_neck   - Test neck servo movements
test_eyes   - Test OLED eye display
test_all    - Run all tests sequentially
stop        - Stop current test and return to standby
help        - Show this help menu
---------------------------

> test_hands
Received command: test_hands
=== TESTING HANDS SERVOS ===
Testing left and right hand servos...
1. Setting to standby position...
2. Testing idle movement...
3. Testing happy movement...
...
=== HANDS TEST COMPLETED ===
```

## Troubleshooting

### Command tidak diterima
- Pastikan robot dalam mode SLEEP
- Pastikan Serial Monitor setting correct (115200 baud, newline)
- Pastikan command ditulis dengan benar (lowercase)

### Test tidak berjalan
- Check koneksi servo
- Check power supply
- Review Serial output untuk error messages

### Component tidak bergerak
- Check wiring connections
- Verify servo power
- Check pin assignments dalam kode

## Technical Notes

- Maintenance mode hanya aktif saat `currentState == SLEEP`
- Commands tidak case-sensitive (otomatis lowercase)
- Setiap test sequence memiliki delay yang cukup untuk observasi
- Serial output memberikan real-time feedback untuk setiap step
- Test dapat di-stop kapan saja dengan command `stop`
