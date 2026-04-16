# Praktikum I2C + UART Potensiometer dengan LCD

1. Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian tersebut!
2. Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan pin kanan tertukar!
3. Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai output) sehingga data tidak hanya ditampilkan di LCD tetapi juga di Serial Monitor dengan format:
   - ADC: 0
   - Volt: 0.00 V
   - Persen: 0%
   - ADC: 0  0% | setCursor(0, 0) dan Bar (level) | setCursor(0, 1)
   
   Berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
4. Lengkapi table berikut berdasarkan pengamatan pada Serial Monitor

---

## Jawaban Pertanyaan

### 1) Cara kerja komunikasi I2C antara Arduino dan LCD

Komunikasi I2C menggunakan **2 kabel utama**:

* SDA → Data (Arduino A4)
* SCL → Clock (Arduino A5)

**Alur kerjanya:**

1. Arduino sebagai **Master**
2. LCD I2C sebagai **Slave** dengan alamat `0x27`
3. Arduino mengirim perintah melalui library:

```cpp
lcd.setCursor()
lcd.print()
```

4. Library `LiquidCrystal_I2C` mengubah perintah tersebut menjadi data I2C
5. Data dikirim melalui:

   * SDA → data karakter
   * SCL → sinkronisasi clock

6. LCD menerima data lalu menampilkan ke layar

**Alur sederhana:**

Arduino → Wire.h → SDA/SCL → LCD I2C → tampil di layar

---

### 2) Apakah pin potensiometer harus seperti itu?

Tidak harus, tetapi **disarankan seperti itu**:

```
Kiri  -> GND
Tengah -> A0
Kanan -> 5V
```

**Jika pin kiri dan kanan tertukar:**

```
Kiri  -> 5V
Tengah -> A0
Kanan -> GND
```

**Yang terjadi:**

* Nilai ADC menjadi **terbalik**
* Putar kiri = nilai besar
* Putar kanan = nilai kecil

Namun **tidak merusak rangkaian**, hanya arah pembacaan saja yang terbalik.

---

### 3) Program Gabungan UART + I2C (Lengkap)

Program ini menampilkan:

* ADC
* Volt
* Persen
* Bar LCD
* Output ke Serial Monitor

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinPot = A0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {

  int adc = analogRead(pinPot);

  float volt = adc * (5.0 / 1023.0);

  int persen = map(adc, 0, 1023, 0, 100);

  int bar = map(adc, 0, 1023, 0, 16);

  // ================= SERIAL MONITOR =================
  Serial.print("ADC: ");
  Serial.print(adc);

  Serial.print("  Volt: ");
  Serial.print(volt, 2);

  Serial.print(" V");

  Serial.print("  Persen: ");
  Serial.print(persen);
  Serial.println("%");


  // ================= LCD BARIS 1 =================
  lcd.setCursor(0,0);
  lcd.print("ADC:");
  lcd.print(adc);
  lcd.print(" ");
  lcd.print(persen);
  lcd.print("%   ");


  // ================= LCD BARIS 2 =================
  lcd.setCursor(0,1);

  for(int i=0;i<16;i++){
    if(i < bar){
      lcd.print((char)255);
    }
    else{
      lcd.print(" ");
    }
  }

  delay(200);

}
```

### Penjelasan Tiap Baris Kode (README.md)

# Modul 3B – UART + I2C Potensiometer

## Library

**#include <Wire.h>**
Digunakan untuk komunikasi I2C

**#include <LiquidCrystal_I2C.h>**
Library untuk LCD I2C

**#include <Arduino.h>**
Library standar Arduino

## Inisialisasi LCD

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);
```
Membuat objek LCD dengan alamat 0x27, ukuran 16 kolom x 2 baris

## Pin Potensiometer

```cpp
const int pinPot = A0;
```
Deklarasi pin potensiometer di pin analog A0

## Setup

```cpp
Serial.begin(9600);
```
Memulai komunikasi serial dengan baud rate 9600

```cpp
lcd.init();
```
Inisialisasi LCD I2C

```cpp
lcd.backlight();
```
Menyalakan lampu latar (backlight) LCD

## Membaca ADC

```cpp
int adc = analogRead(pinPot);
```
Membaca nilai analog dari potensiometer (0–1023)

## Konversi ke Volt

```cpp
float volt = adc * (5.0 / 1023.0);
```
Mengubah nilai ADC menjadi tegangan (0–5V)

Rumus: Volt = ADC × 5 / 1023

## Konversi ke Persen

```cpp
int persen = map(adc, 0, 1023, 0, 100);
```
Mengubah nilai ADC (0–1023) menjadi persen (0–100%)

## Membuat Bar LCD

```cpp
int bar = map(adc, 0, 1023, 0, 16);
```
Mengubah nilai ADC menjadi panjang bar (0–16 karakter)

## Output Serial Monitor

```cpp
Serial.print("ADC: ");
Serial.print(adc);
```
Menampilkan label dan nilai ADC

```cpp
Serial.print("  Volt: ");
Serial.print(volt, 2);
Serial.print(" V");
```
Menampilkan tegangan dengan 2 desimal

```cpp
Serial.print("  Persen: ");
Serial.print(persen);
Serial.println("%");
```
Menampilkan persentase dan pindah baris

## LCD Baris 1 (Nilai)

```cpp
lcd.setCursor(0,0);
```
Menempatkan kursor di kolom 0, baris 0

```cpp
lcd.print("ADC:");
lcd.print(adc);
lcd.print(" ");
lcd.print(persen);
lcd.print("%   ");
```
Menampilkan nilai ADC dan persen di baris 1

Contoh: "ADC:512 50%   "

## LCD Baris 2 (Bar Graph)

```cpp
lcd.setCursor(0,1);
```
Menempatkan kursor di kolom 0, baris 1

```cpp
for(int i=0;i<16;i++){
```
Loop untuk setiap kolom LCD (0 sampai 15)

```cpp
if(i < bar){
  lcd.print((char)255);
}
else{
  lcd.print(" ");
}
```
Jika kolom lebih kecil dari bar, tampilkan blok penuh (karakter 255)
Jika tidak, tampilkan spasi kosong

## Delay

```cpp
delay(200);
```
Menunda 200 ms sebelum pembacaan berikutnya agar tampilan tidak terlalu cepat

---

### 4) Tabel Pengamatan

**Rumus perhitungan:**

```
Volt = ADC × 5 / 1023
Persen = (ADC / 1023) × 100
```

| ADC | Volt (V) | Persen (%) |
| --- | -------- | ---------- |
| 1   | 0.00 V   | 0 %        |
| 21  | 0.10 V   | 2 %        |
| 49  | 0.24 V   | 5 %        |
| 74  | 0.36 V   | 7 %        |
| 96  | 0.47 V   | 9 %        |

---
