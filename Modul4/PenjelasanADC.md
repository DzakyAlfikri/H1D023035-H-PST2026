# Modul 4 – ADC

## Penjelasan Program

Program ini menggunakan potensiometer sebagai input ADC untuk mengontrol servo motor. Meskipun potensiometer memiliki rentang ADC penuh (0–1023), servo dibatasi hanya bergerak dalam rentang sudut 30°–150°.

---

## Library dan Setup Awal

### 1. Include Library Servo

```cpp
#include <Servo.h> // library untuk servo motor
```
**Penjelasan:** Library Servo menyediakan fungsi untuk mengontrol servo motor dengan mudah.

### 2. Membuat Objek Servo

```cpp
Servo myservo; // membuat objek servo
```
**Penjelasan:** Deklarasi objek servo bernama `myservo` yang akan digunakan untuk mengontrol servo motor.

---

## Konfigurasi Pin

### 3. Deklarasi Pin Input (Potensiometer)

```cpp
const int potensioPin = A0;   // Menggunakan pin Analog A0
```
**Penjelasan:** Pin A0 adalah pin analog yang terhubung dengan potensiometer. Pin analog dapat membaca tegangan dari 0–5V dan mengkonversinya menjadi nilai digital 0–1023.

### 4. Deklarasi Pin Output (Servo)

```cpp
const int servoPin = 9;       // Menggunakan pin Digital 9 (PWM)
```
**Penjelasan:** Pin 9 adalah pin PWM (Pulse Width Modulation) yang digunakan untuk mengontrol servo motor. PWM menghasilkan sinyal berdenyut yang dapat membuat servo bergerak ke sudut tertentu.

---

## Variabel Global

### 5. Variabel untuk Posisi Servo

```cpp
int pos = 0; // Variabel untuk menyimpan hasil pemetaan sudut
```
**Penjelasan:** Menyimpan hasil konversi nilai ADC menjadi sudut servo (30–150 derajat).

### 6. Variabel untuk Nilai ADC

```cpp
int val = 0; // Variabel untuk menyimpan nilai mentah ADC
```
**Penjelasan:** Menyimpan nilai digital dari potensiometer sebelum dikonversi menjadi sudut.

---

## Fungsi Setup

### 7. Menghubungkan Servo ke Pin

```cpp
void setup() {
  myservo.attach(servoPin); 
```
**Penjelasan:** Fungsi `attach()` menghubungkan objek servo `myservo` ke pin digital 9 (pin PWM). Tanpa ini, servo tidak bisa dikendalikan.

### 8. Inisialisasi Komunikasi Serial

```cpp
  Serial.begin(9600); // Baud rate standar 9600
}
```
**Penjelasan:** Membuka komunikasi serial dengan baud rate 9600 untuk monitoring data ke Serial Monitor.

---

## Fungsi Loop

### 9. Membaca Nilai Potensiometer (ADC)

```cpp
void loop() {
  val = analogRead(potensioPin);
```
**Penjelasan:** 
- Fungsi `analogRead()` membaca tegangan analog dari pin A0
- Nilai dibaca dalam rentang **0–1023** (10-bit ADC)
- 0 = 0V (GND)
- 1023 = 5V (VCC)
- Nilai disimpan di variabel `val`

### 10. Konversi ADC ke Sudut Servo (Pembatasan 30°–150°)

```cpp
  pos = map(val, 0, 1023, 30, 150);
```
**Penjelasan:**
- Fungsi `map()` mengkonversi rentang nilai ADC ke rentang sudut servo
- **Syntax:** `map(value, fromLow, fromHigh, toLow, toHigh)`
- **Penjelasan tiap parameter:**
  - `val` = nilai ADC yang dibaca (0–1023)
  - `0` = nilai ADC minimum
  - `1023` = nilai ADC maksimum
  - `30` = sudut minimum servo (pembatasan)
  - `150` = sudut maksimum servo (pembatasan)

**Contoh:**
- Jika `val = 0` → `pos = 30°`
- Jika `val = 511` → `pos ≈ 90°` (pertengahan)
- Jika `val = 1023` → `pos = 150°`

Dengan demikian, meskipun potensiometer memiliki rentang penuh 0–1023, servo hanya akan bergerak antara 30° dan 150°.

### 11. Menggerakkan Servo

```cpp
  myservo.write(pos);
```
**Penjelasan:** 
- Fungsi `write()` menggerakkan servo ke sudut yang ditentukan dalam variabel `pos`
- Servo akan bergerak secara mulus menuju posisi target
- Nilai `pos` harus dalam rentang 0–180 derajat (standar servo)

### 12. Menampilkan Data ke Serial Monitor

```cpp
  Serial.print("ADC Potensio: ");
  Serial.print(val); 
```
**Penjelasan:** Menampilkan label "ADC Potensio: " diikuti dengan nilai ADC yang dibaca. Contoh output: "ADC Potensio: 512"

```cpp
  Serial.print(" | Sudut Servo: ");
  Serial.println(pos);
```
**Penjelasan:** Menampilkan sudut servo saat ini. Fungsi `println()` menambahkan baris baru setelah nilai. Contoh output lengkap: "ADC Potensio: 512 | Sudut Servo: 90"

### 13. Penundaan

```cpp
  delay(15);
}
```
**Penjelasan:** 
- Menunda eksekusi 15 milidetik sebelum pembacaan berikutnya
- Memberikan waktu servo untuk bergerak ke posisi target
- Mencegah pembacaan yang terlalu cepat yang dapat menyebabkan servo bergetar

---
