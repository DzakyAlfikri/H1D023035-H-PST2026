# Pertanyaan PWM

## 1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi analogWrite()

LED dapat diatur kecerahannya menggunakan fungsi `analogWrite()` karena Arduino menghasilkan sinyal PWM (Pulse Width Modulation). PWM bekerja dengan menghidupkan dan mematikan LED secara sangat cepat dengan perbandingan waktu ON dan OFF tertentu (duty cycle).

Jika nilai PWM kecil, waktu ON lebih singkat sehingga LED terlihat redup. Jika nilai PWM besar, waktu ON lebih lama sehingga LED terlihat lebih terang. Ketika PWM bernilai maksimum (255), LED menyala penuh, dan ketika bernilai 0 LED mati.

---

## 2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?

Nilai ADC berasal dari pembacaan analog 10-bit (0–1023), sedangkan PWM Arduino menggunakan resolusi 8-bit (0–255). Karena itu diperlukan konversi menggunakan fungsi `map()` agar nilai ADC dapat digunakan sebagai output PWM.

**Hubungannya adalah:**

- ADC 0 → PWM 0 (LED mati)
- ADC 512 → PWM sekitar 127 (LED setengah terang)
- ADC 1023 → PWM 255 (LED terang maksimal)

Artinya nilai ADC diperkecil skalanya dari 1024 level menjadi 256 level.

---

## 3. Modifikasi Program (LED hanya menyala pada PWM 50–200)

### Program Lengkap:

```cpp
#include <Arduino.h>

const int potPin = A0;
const int ledPin = 9;

int nilaiADC = 0;
int pwm = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  nilaiADC = analogRead(potPin);
  
  pwm = map(nilaiADC, 0, 1023, 0, 255);
  
  // LED hanya menyala pada rentang sedang
  if (pwm >= 50 && pwm <= 200) {
    analogWrite(ledPin, pwm);
  } else {
    analogWrite(ledPin, 0); // LED mati
  }
  
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print(" | PWM: ");
  Serial.println(pwm);
  
  delay(50);
}
```

---
