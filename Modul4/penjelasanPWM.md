# Modul 4 – PWM

## Penjelasan Program

Program ini menggunakan potensiometer sebagai input ADC untuk mengontrol brightness (kecerahan) LED melalui PWM (Pulse Width Modulation). LED hanya akan menyala pada rentang PWM 50–200, sementara di luar rentang tersebut LED akan mati sepenuhnya.

---

## Library dan Setup Awal

### 1. Include Library Arduino

```cpp
#include <Arduino.h>
```
**Penjelasan:** Library standar Arduino yang berisi fungsi-fungsi dasar seperti `digitalWrite()`, `analogRead()`, `analogWrite()`, dan lainnya.

---

## Konfigurasi Pin

### 2. Deklarasi Pin Input (Potensiometer)

```cpp
const int potPin = A0;
```
**Penjelasan:** Pin A0 adalah pin analog yang terhubung dengan potensiometer. Pin ini digunakan untuk membaca nilai analog (tegangan) dari potensiometer dalam rentang 0–1023.

### 3. Deklarasi Pin Output (LED)

```cpp
const int ledPin = 9;
```
**Penjelasan:** Pin 9 adalah pin PWM digital yang digunakan untuk mengontrol LED. Pin ini dapat menghasilkan sinyal PWM dengan nilai 0–255 untuk mengatur brightness (kecerahan) LED.

---

## Variabel Global

### 4. Variabel untuk Nilai ADC

```cpp
int nilaiADC = 0;
```
**Penjelasan:** Menyimpan nilai digital dari potensiometer yang dibaca dari pin A0 dalam rentang 0–1023.

### 5. Variabel untuk Nilai PWM

```cpp
int pwm = 0;
```
**Penjelasan:** Menyimpan hasil konversi nilai ADC menjadi nilai PWM dalam rentang 0–255 untuk mengontrol kecerahan LED.

---

## Fungsi Setup

### 6. Mengkonfigurasi Pin LED sebagai Output

```cpp
void setup() {
  pinMode(ledPin, OUTPUT);
```
**Penjelasan:** Fungsi `pinMode()` mengatur pin 9 sebagai output. Ini memberi tahu Arduino bahwa pin ini akan digunakan untuk mengirim sinyal (bukan menerima).

### 7. Inisialisasi Komunikasi Serial

```cpp
  Serial.begin(9600);
}
```
**Penjelasan:** Membuka komunikasi serial dengan baud rate 9600 untuk menampilkan data nilai ADC dan PWM ke Serial Monitor komputer.

---

## Fungsi Loop

### 8. Membaca Nilai Potensiometer (ADC)

```cpp
void loop() {
  nilaiADC = analogRead(potPin);
```
**Penjelasan:** 
- Fungsi `analogRead()` membaca tegangan analog dari pin A0 (potensiometer)
- Nilai dibaca dalam rentang **0–1023** (resolusi 10-bit ADC)
- 0 = 0V (GND)
- 1023 = 5V (VCC)
- Nilai disimpan di variabel `nilaiADC`

### 9. Konversi ADC ke PWM

```cpp
  pwm = map(nilaiADC, 0, 1023, 0, 255);
```
**Penjelasan:**
- Fungsi `map()` mengkonversi rentang nilai ADC ke rentang nilai PWM
- **Syntax:** `map(value, fromLow, fromHigh, toLow, toHigh)`
- **Penjelasan tiap parameter:**
  - `nilaiADC` = nilai ADC yang dibaca (0–1023)
  - `0` = nilai ADC minimum
  - `1023` = nilai ADC maksimum
  - `0` = nilai PWM minimum
  - `255` = nilai PWM maksimum

**Contoh konversi:**
- Jika `nilaiADC = 0` → `pwm = 0`
- Jika `nilaiADC = 512` → `pwm ≈ 127`
- Jika `nilaiADC = 1023` → `pwm = 255`

### 10. Kondisi LED (Pembatasan Rentang 50–200)

```cpp
  if (pwm >= 50 && pwm <= 200) {
    analogWrite(ledPin, pwm);
```
**Penjelasan:**
- Kondisi `if` mengecek apakah nilai PWM berada dalam rentang 50–200
- Jika **ya**, LED menyala dengan nilai PWM sesuai hasil konversi
- Operator `&&` berarti **dan** (kedua kondisi harus terpenuhi)

**Logika:**
- `pwm >= 50` = PWM lebih besar atau sama dengan 50
- `pwm <= 200` = PWM lebih kecil atau sama dengan 200

### 11. LED Mati (Nilai PWM di Luar Rentang)

```cpp
  } else {
    analogWrite(ledPin, 0); // LED mati
  }
```
**Penjelasan:**
- Jika PWM kurang dari 50 **atau** lebih dari 200, LED akan dipaksa mati dengan `analogWrite(ledPin, 0)`
- Nilai PWM 0 berarti tidak ada sinyal PWM, sehingga LED mati total

### 12. Menampilkan Data Nilai ADC

```cpp
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
```
**Penjelasan:** Menampilkan label "ADC: " diikuti dengan nilai ADC yang dibaca. Contoh: "ADC: 512"

### 13. Menampilkan Data Nilai PWM

```cpp
  Serial.print(" | PWM: ");
  Serial.println(pwm);
```
**Penjelasan:** 
- Menampilkan pemisah " | " dan label "PWM: "
- Menampilkan nilai PWM yang dihasilkan
- Fungsi `println()` menambahkan baris baru setelah nilai
- Contoh output lengkap: "ADC: 512 | PWM: 127"

### 14. Penundaan

```cpp
  delay(50);
}
```
**Penjelasan:**
- Menunda eksekusi 50 milidetik sebelum pembacaan berikutnya
- Memberikan waktu pembacaan sensor yang stabil
- Mencegah data yang ditampilkan berubah terlalu cepat

---

## Ringkasan Alur Program

1. **Pembacaan:** Baca nilai analog potensiometer (0–1023)
2. **Konversi:** Ubah nilai ADC menjadi PWM (0–255) dengan `map()`
3. **Pengecekan Kondisi:** Cek apakah PWM dalam rentang 50–200
4. **Kontrol LED:** 
   - Jika dalam rentang → LED menyala dengan kecerahan sesuai PWM
   - Jika di luar rentang → LED mati
5. **Monitoring:** Tampilkan data ADC dan PWM ke Serial Monitor
6. **Delay:** Tunggu 50ms sebelum loop berikutnya

---

## Penjelasan PWM (Pulse Width Modulation)

### Cara Kerja PWM

PWM bekerja dengan menghasilkan sinyal digital yang menghidupkan dan mematikan LED sangat cepat. Perbandingan waktu **ON** dan **OFF** disebut **duty cycle**.

**Contoh:**
- PWM 0 → LED OFF 100% (LED mati)
- PWM 127 → LED ON 50%, OFF 50% (LED setengah terang)
- PWM 255 → LED ON 100% (LED terang maksimal)

Karena switching (on/off) sangat cepat, mata manusia hanya melihat LED dengan tingkat kecerahan rata-rata, bukan on/off yang berubah-ubah.

### Rentang PWM Arduino

Arduino memiliki 8-bit resolusi PWM, artinya nilai yang dapat digunakan adalah **0 hingga 255**.

---

## Apa yang Terjadi pada Berbagai Nilai PWM

| PWM | Status | Kecerahan |
|-----|--------|-----------|
| 0   | Mati   | 0% (Mati) |
| 50  | Nyala (Awal Rentang) | ~20% (Redup) |
| 127 | Nyala | ~50% (Sedang) |
| 200 | Nyala (Akhir Rentang) | ~78% (Terang) |
| 255 | Nyala | 100% (Maksimal) |

---

## Output yang Diharapkan

Saat menjalankan program dan memutar potensiometer:

**Serial Monitor menampilkan:**
```
ADC: 0 | PWM: 0
ADC: 204 | PWM: 40
ADC: 255 | PWM: 63
ADC: 409 | PWM: 101
ADC: 512 | PWM: 127
ADC: 614 | PWM: 151
ADC: 819 | PWM: 201
ADC: 1023 | PWM: 255
```

**Gerakan LED:**
- **Putar potensiometer 0–20%:** ADC 0–200, PWM 0–49 → LED **mati**
- **Putar potensiometer 20–78%:** ADC 200–800, PWM 50–200 → LED **menyala dengan kecerahan bertingkat**
- **Putar potensiometer 78–100%:** ADC 800–1023, PWM 201–255 → LED **mati**

---

## Keuntungan Pembatasan Rentang PWM

1. **Kontrol Presisi:** LED hanya nyala pada rentang yang diperlukan (50–200)
2. **Keselamatan:** Melindungi LED dari konsumsi arus yang terlalu tinggi (PWM 0–50)
3. **Performa:** Menghindari flicker atau masalah teknis pada PWM rendah dan tinggi
4. **Intuitif:** Hanya bagian tengah potensiometer yang mengontrol kecerahan LED

---
